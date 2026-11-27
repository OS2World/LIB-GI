/* Finalize OW2-linked GI modules for the original OS/2 LX ABI.
 *
 * Open Watcom emits ENTRY32 export bundles and leaves E32NOINTFIX set.
 * The original GI modules use ENTRY16 exports.  This tool rewrites the
 * entry table in ordinal order, marks the thunk objects Big instead of
 * 16:16 alias, adjusts the LX offsets, and sets the module flags.  It also
 * normalizes the wall-clock timestamp that wlib embeds in the import
 * libraries, so the 1.01 libraries are reproducible byte-for-byte.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 256

typedef struct {
    unsigned long ordinal;
    unsigned char btype;
    unsigned short obj;
    unsigned char flags;
    unsigned long offset;
} ENTRY;

static unsigned short rd16(const unsigned char *p)
{
    return (unsigned short)(p[0] | (p[1] << 8));
}

static unsigned long rd32(const unsigned char *p)
{
    return (unsigned long)p[0] |
           ((unsigned long)p[1] << 8) |
           ((unsigned long)p[2] << 16) |
           ((unsigned long)p[3] << 24);
}

static void wr32(unsigned char *p, unsigned long v)
{
    p[0] = (unsigned char)(v & 0xff);
    p[1] = (unsigned char)((v >> 8) & 0xff);
    p[2] = (unsigned char)((v >> 16) & 0xff);
    p[3] = (unsigned char)((v >> 24) & 0xff);
}

static int is_dll_name(const char *path)
{
    size_t n = strlen(path);
    if (n < 4) {
        return 0;
    }
    return path[n - 4] == '.' &&
           (path[n - 3] == 'D' || path[n - 3] == 'd') &&
           (path[n - 2] == 'L' || path[n - 2] == 'l') &&
           (path[n - 1] == 'L' || path[n - 1] == 'l');
}

static int patch_file(const char *path)
{
    FILE *f;
    long size;
    unsigned char *data;
    unsigned char *newtab;
    unsigned long e, ent_rel, objtab;
    size_t ent_start, p, end, newlen = 0, delta, i, j, written;
    unsigned long ordinal = 1;
    unsigned long ldrsize;
    unsigned long old_flags, new_flags;
    ENTRY entries[MAX_ENTRIES];
    size_t nentries = 0;
    int run_obj = 0, run_count = 0;
    unsigned char run_flags[MAX_ENTRIES];
    unsigned short run_off[MAX_ENTRIES];
    int dll = is_dll_name(path);

    static const int rel_fields[] = {
        0x40, 0x48, 0x50, 0x58, 0x60, 0x68,
        0x6C, 0x70, 0x78, 0x7C, 0x98
    };
    static const int abs_fields[] = { 0x4C, 0x80, 0x88 };

    f = fopen(path, "rb");
    if (f == NULL) {
        printf("%s: cannot open for read\n", path);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);
    if (size < 0x100) {
        fclose(f);
        printf("%s: file too small\n", path);
        return 1;
    }
    data = (unsigned char *)malloc((size_t)size);
    if (data == NULL) {
        fclose(f);
        printf("%s: out of memory\n", path);
        return 1;
    }
    if (fread(data, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        free(data);
        printf("%s: short read\n", path);
        return 1;
    }
    fclose(f);

    if (data[0] != 'M' || data[1] != 'Z') {
        free(data);
        printf("%s: not an MZ file\n", path);
        return 1;
    }
    e = rd32(data + 0x3C);
    if (e + 0x98 > (unsigned long)size || data[e] != 'L' || data[e + 1] != 'X') {
        free(data);
        printf("%s: not an LX file\n", path);
        return 1;
    }

    if (dll) {
        ent_rel = rd32(data + e + 0x5C);
        ent_start = (size_t)e + (size_t)ent_rel;
        if (ent_start >= (size_t)size) {
            free(data);
            printf("%s: bad entry-table offset\n", path);
            return 1;
        }

        p = ent_start;
        while (p + 2 <= (size_t)size) {
            unsigned char count = data[p];
            unsigned char btype = data[p + 1];
            unsigned short obj;
            unsigned int k;

            if (count == 0) {
                p++;
                break;
            }
            p += 2;
            if (btype == 0) {
                ordinal += count;
                continue;
            }
            if (p + 2 > (size_t)size) {
                free(data);
                printf("%s: truncated entry bundle\n", path);
                return 1;
            }
            obj = rd16(data + p);
            p += 2;

            for (k = 0; k < count; k++) {
                ENTRY *ent;
                if (nentries >= MAX_ENTRIES) {
                    free(data);
                    printf("%s: too many exports\n", path);
                    return 1;
                }
                ent = &entries[nentries++];
                ent->ordinal = ordinal;
                ent->btype = btype;
                ent->obj = obj;
                ent->flags = 0;
                ent->offset = 0;

                if (btype == 1) {
                    if (p + 3 > (size_t)size) {
                        free(data);
                        printf("%s: truncated entry16\n", path);
                        return 1;
                    }
                    ent->flags = data[p];
                    ent->offset = rd16(data + p + 1);
                    p += 3;
                } else if (btype == 2 || btype == 3) {
                    if (p + 5 > (size_t)size) {
                        free(data);
                        printf("%s: truncated entry32\n", path);
                        return 1;
                    }
                    ent->flags = data[p];
                    ent->offset = (btype == 2) ? rd16(data + p + 1) : rd32(data + p + 1);
                    p += 5;
                } else if (btype == 4) {
                    if (p + 7 > (size_t)size) {
                        free(data);
                        printf("%s: truncated entry\n", path);
                        return 1;
                    }
                    ent->flags = data[p];
                    ent->offset = rd32(data + p + 3);
                    p += 7;
                } else {
                    free(data);
                    printf("%s: unknown bundle type %d\n", path, btype);
                    return 1;
                }
                ordinal++;
            }
        }
        end = p;

        newtab = (unsigned char *)malloc((size_t)size);
        if (newtab == NULL) {
            free(data);
            printf("%s: out of memory for entry table\n", path);
            return 1;
        }

        for (i = 0; i < nentries; i++) {
            ENTRY *ent = &entries[i];
            if (ent->btype != 3) {
                free(newtab);
                free(data);
                printf("%s: ordinal %lu is not entry32\n", path, ent->ordinal);
                return 1;
            }
            if (ent->offset > 0xFFFF) {
                free(newtab);
                free(data);
                printf("%s: ordinal %lu offset exceeds 16 bits\n", path, ent->ordinal);
                return 1;
            }
            if (run_count > 0 && run_obj != ent->obj) {
                newtab[newlen++] = (unsigned char)run_count;
                newtab[newlen++] = 1;
                newtab[newlen++] = (unsigned char)(run_obj & 0xff);
                newtab[newlen++] = (unsigned char)((run_obj >> 8) & 0xff);
                for (j = 0; j < (size_t)run_count; j++) {
                    newtab[newlen++] = (unsigned char)(run_flags[j] | 0x01);
                    newtab[newlen++] = (unsigned char)(run_off[j] & 0xff);
                    newtab[newlen++] = (unsigned char)((run_off[j] >> 8) & 0xff);
                }
                run_count = 0;
            }
            if (run_count == 0) {
                run_obj = ent->obj;
            }
            if (run_count >= MAX_ENTRIES) {
                free(newtab);
                free(data);
                printf("%s: bundle too large\n", path);
                return 1;
            }
            run_flags[run_count] = ent->flags;
            run_off[run_count] = (unsigned short)ent->offset;
            run_count++;
        }
        if (run_count > 0) {
            newtab[newlen++] = (unsigned char)run_count;
            newtab[newlen++] = 1;
            newtab[newlen++] = (unsigned char)(run_obj & 0xff);
            newtab[newlen++] = (unsigned char)((run_obj >> 8) & 0xff);
            for (j = 0; j < (size_t)run_count; j++) {
                newtab[newlen++] = (unsigned char)(run_flags[j] | 0x01);
                newtab[newlen++] = (unsigned char)(run_off[j] & 0xff);
                newtab[newlen++] = (unsigned char)((run_off[j] >> 8) & 0xff);
            }
        }
        newtab[newlen++] = 0;

        if (end < ent_start || end - ent_start < newlen) {
            free(newtab);
            free(data);
            printf("%s: new entry table is larger than the original\n", path);
            return 1;
        }
        delta = (end - ent_start) - newlen;

        objtab = rd32(data + e + 0x40);
        for (i = 0; i < nentries; i++) {
            unsigned long flag_off;
            unsigned long flags;
            int seen = 0;
            for (j = 0; j < i; j++) {
                if (entries[j].obj == entries[i].obj) {
                    seen = 1;
                    break;
                }
            }
            if (seen) {
                continue;
            }
            flag_off = e + objtab + ((unsigned long)entries[i].obj - 1) * 24 + 8;
            if (flag_off + 4 > (unsigned long)size) {
                free(newtab);
                free(data);
                printf("%s: bad object table\n", path);
                return 1;
            }
            flags = rd32(data + flag_off);
            wr32(data + flag_off, (flags & ~0x1000UL) | 0x2000UL);
        }

        memmove(data + ent_start + newlen, data + end, (size_t)size - end);
        memcpy(data + ent_start, newtab, newlen);
        written = (size_t)size - delta;
        free(newtab);

        for (i = 0; i < sizeof(rel_fields) / sizeof(rel_fields[0]); i++) {
            unsigned long field = rd32(data + e + rel_fields[i]);
            if (field > ent_rel) {
                wr32(data + e + rel_fields[i], field - delta);
            }
        }
        for (i = 0; i < sizeof(abs_fields) / sizeof(abs_fields[0]); i++) {
            unsigned long field = rd32(data + e + abs_fields[i]);
            if (field >= end) {
                wr32(data + e + abs_fields[i], field - delta);
            }
        }
        ldrsize = rd32(data + e + 0x38);
        if (ldrsize) {
            wr32(data + e + 0x38, ldrsize - delta);
        }
        printf("%s: entry16=%lu delta=%lu ", path, (unsigned long)nentries, delta);
    } else {
        written = (size_t)size;
    }

    /* Keep E32NOINTFIX: OW's internal fixups are already applied.  Restore
       the original PM-compatible bit for DLLs. */
    new_flags = dll ? 0x8210UL : 0x0300UL;
    old_flags = rd32(data + e + 0x10);
    wr32(data + e + 0x10, new_flags);
    printf("flags 0x%04lx->0x%04lx\n", old_flags, new_flags);

    f = fopen(path, "wb");
    if (f == NULL) {
        free(data);
        printf("%s: cannot open for write\n", path);
        return 1;
    }
    if (fwrite(data, 1, written, f) != written) {
        fclose(f);
        free(data);
        printf("%s: short write\n", path);
        return 1;
    }
    fclose(f);
    free(data);
    return 0;
}

static const char *base_name(const char *path)
{
    const char *p = path;
    const char *slash;
    while ((slash = strchr(p, '\\')) != NULL || (slash = strchr(p, '/')) != NULL) {
        p = slash + 1;
    }
    return p;
}

static int same_name(const char *a, const char *b)
{
    while (*a && *b) {
        int ca = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a;
        int cb = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (ca != cb) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

static int patch_lib(const char *path)
{
    static const unsigned char sig[] = {
        0x88, 0x08, 0x00, 0xc0, 0xfe, 0x54
    };
    static const struct {
        const char *name;
        unsigned long stamp;
    } stamps[] = {
        { "gi.lib",      0x6B08FF88UL },
        { "tapedev.lib", 0x6B090036UL }
    };
    FILE *f;
    long size;
    unsigned char *data;
    unsigned long stamp = 0;
    size_t p, found = 0, i;
    const char *base = base_name(path);

    for (i = 0; i < sizeof(stamps) / sizeof(stamps[0]); i++) {
        if (same_name(base, stamps[i].name)) {
            stamp = stamps[i].stamp;
            break;
        }
    }
    if (stamp == 0) {
        printf("%s: unknown library timestamp\n", path);
        return 1;
    }

    f = fopen(path, "rb");
    if (f == NULL) {
        printf("%s: cannot open library for read\n", path);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);
    if (size < (long)sizeof(sig)) {
        fclose(f);
        printf("%s: library too small\n", path);
        return 1;
    }
    data = (unsigned char *)malloc((size_t)size);
    if (data == NULL) {
        fclose(f);
        printf("%s: out of memory\n", path);
        return 1;
    }
    if (fread(data, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        free(data);
        printf("%s: short library read\n", path);
        return 1;
    }
    fclose(f);

    for (p = 0; p + sizeof(sig) + 5 <= (size_t)size; p++) {
        if (memcmp(data + p, sig, sizeof(sig)) == 0) {
            unsigned sum = 0;
            size_t k;
            wr32(data + p + sizeof(sig), stamp);
            for (k = 0; k < sizeof(sig) + 4; k++) {
                sum += data[p + k];
            }
            data[p + sizeof(sig) + 4] = (unsigned char)(-sum);
            found++;
        }
    }
    if (found == 0) {
        free(data);
        printf("%s: no timestamp records found\n", path);
        return 1;
    }

    f = fopen(path, "wb");
    if (f == NULL) {
        free(data);
        printf("%s: cannot open library for write\n", path);
        return 1;
    }
    if (fwrite(data, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        free(data);
        printf("%s: short library write\n", path);
        return 1;
    }
    fclose(f);
    free(data);
    printf("%s: normalized %lu timestamp records to 0x%08lx\n",
           path, (unsigned long)found, stamp);
    return 0;
}

static int is_lib_name(const char *path)
{
    size_t n = strlen(path);
    if (n < 4) {
        return 0;
    }
    return path[n - 4] == '.' &&
           (path[n - 3] == 'L' || path[n - 3] == 'l') &&
           (path[n - 2] == 'I' || path[n - 2] == 'i') &&
           (path[n - 1] == 'B' || path[n - 1] == 'b');
}

int main(int argc, char **argv)
{
    int i, rc = 0;
    if (argc < 2) {
        printf("usage: ENTRY16.EXE file.dll [file.exe ...]\n");
        return 2;
    }
    for (i = 1; i < argc; i++) {
        if (is_lib_name(argv[i])) {
            if (patch_lib(argv[i])) {
                rc = 1;
            }
        } else if (patch_file(argv[i])) {
            rc = 1;
        }
    }
    return rc;
}
