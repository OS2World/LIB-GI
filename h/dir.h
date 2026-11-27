#ifndef OW_GI_DIR_H
#define OW_GI_DIR_H

/* Minimal Borland-compatible path helpers for the Open Watcom rebuild. */
#include <stddef.h>
#include <string.h>

#define MAXDRIVE 3
#define MAXDIR   260
#define MAXFILE  260
#define MAXEXT   260
#define MAXPATH  260

#define WILDCARDS 0x01
#define EXTENSION 0x02
#define FILENAME  0x04
#define DIRECTORY 0x08
#define DRIVE     0x10

static int gi_fnsplit(const char *path, char *drive, char *dir,
                      char *name, char *ext)
{
    const char *p = path;
    const char *name_start;
    const char *ext_start;
    const char *sep;
    size_t n;
    int flags = 0;

    if (drive) drive[0] = '\0';
    if (dir)   dir[0]   = '\0';
    if (name)  name[0]  = '\0';
    if (ext)   ext[0]   = '\0';
    if (!path) return 0;

    if (((p[0] >= 'A' && p[0] <= 'Z') || (p[0] >= 'a' && p[0] <= 'z')) && p[1] == ':') {
        if (drive) {
            drive[0] = p[0];
            drive[1] = ':';
            drive[2] = '\0';
        }
        flags |= DRIVE;
        p += 2;
    }

    name_start = p;
    for (sep = p; *sep; sep++) {
        if (*sep == '/' || *sep == '\\' || *sep == ':') {
            name_start = sep + 1;
            if (dir) {
                n = (size_t)(sep - p) + 1;
                if (n >= MAXDIR) n = MAXDIR - 1;
                memcpy(dir, p, n);
                dir[n] = '\0';
            }
            flags |= DIRECTORY;
        }
    }

    ext_start = name_start;
    for (sep = name_start; *sep; sep++) {
        if (*sep == '.') ext_start = sep;
    }

    if (name) {
        n = (size_t)(ext_start - name_start);
        if (n >= MAXFILE) n = MAXFILE - 1;
        memcpy(name, name_start, n);
        name[n] = '\0';
    }
    if (name_start != ext_start) flags |= FILENAME;

    if (ext && *ext_start) {
        n = strlen(ext_start);
        if (n >= MAXEXT) n = MAXEXT - 1;
        memcpy(ext, ext_start, n);
        ext[n] = '\0';
        flags |= EXTENSION;
    }

    if (strchr(path, '*') || strchr(path, '?')) flags |= WILDCARDS;
    return flags;
}

static void gi_fnmerge(char *path, const char *drive, const char *dir,
                       const char *name, const char *ext)
{
    size_t pos = 0;

    path[0] = '\0';
    if (drive && *drive) {
        size_t n = strlen(drive);
        memcpy(path + pos, drive, n);
        pos += n;
    }
    if (dir && *dir) {
        size_t n = strlen(dir);
        memcpy(path + pos, dir, n);
        pos += n;
    }
    if (name && *name) {
        size_t n = strlen(name);
        memcpy(path + pos, name, n);
        pos += n;
    }
    if (ext && *ext) {
        size_t n = strlen(ext);
        memcpy(path + pos, ext, n);
        pos += n;
    }
    path[pos] = '\0';
}

#define fnsplit gi_fnsplit
#define fnmerge gi_fnmerge

#endif
