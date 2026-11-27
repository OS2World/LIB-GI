"""Convert an OW2 OS/2 LX DLL's export bundles to 16-bit entries.

Open Watcom 2's 32-bit OS/2 linker emits ENT_32BIT entries even for code built
with __far16.  The original Borland-built GI modules use ENT_16BIT entries for
the same calls.  This rewrites the entry table, sets the Big bit on the target
objects so the 16-bit thunks live in 32-bit objects, shrinks the table, and
adjusts all later LX header offsets/file offsets so the image stays loadable.
"""

import struct
import sys
from pathlib import Path

# Relative-to-LX-header offsets in e32_exe
REL_FIELDS = (0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x6C, 0x70, 0x78, 0x7C, 0x98)
# Absolute file offsets in e32_exe
ABS_FIELDS = (0x4C, 0x80, 0x88)


def parse_entry_table(data, off):
    entries = []
    ordinal = 1
    while off + 2 <= len(data):
        count = data[off]
        if count == 0:
            return entries, off + 1
        btype = data[off + 1]
        off += 2
        if btype == 0:
            ordinal += count
            continue
        if off + 2 > len(data):
            break
        obj = struct.unpack_from("<H", data, off)[0]
        off += 2
        for _ in range(count):
            if btype == 1:
                if off + 3 > len(data):
                    return entries, off
                flags = data[off]
                offset = struct.unpack_from("<H", data, off + 1)[0]
                off += 3
            elif btype in (2, 3):
                if off + 5 > len(data):
                    return entries, off
                flags = data[off]
                offset = struct.unpack_from("<H" if btype == 2 else "<I", data, off + 1)[0]
                off += 5
            elif btype == 4:
                if off + 7 > len(data):
                    return entries, off
                flags = data[off]
                offset = struct.unpack_from("<I", data, off + 3)[0]
                off += 7
            else:
                raise ValueError("unknown bundle type %d" % btype)
            entries.append((ordinal, btype, obj, flags, offset))
            ordinal += 1
    return entries, off


def build_entry_table(entries):
    """Emit ENT_16BIT bundles in ordinal order.

    LX bundles cannot reorder entries: the nth entry encountered is ordinal n.
    Grouping all entries by object (rather than by consecutive ordinal run)
    silently reassigns ordinals, so keep the original order and only close a
    bundle when the target object changes.
    """
    out = bytearray()
    run = []  # (obj, flags, offset)

    def flush():
        if not run:
            return
        obj = run[0][0]
        if len(run) > 255:
            raise ValueError("too many entries in object %d" % obj)
        out.append(len(run))
        out.append(1)  # ENT_16BIT
        out.extend(struct.pack("<H", obj))
        for _, flags, offset in run:
            out.append(flags)
            out.extend(struct.pack("<H", offset))
        run.clear()

    for ordinal, btype, obj, flags, offset in entries:
        if btype != 3:
            raise ValueError("ordinal %d is not an entry32" % ordinal)
        if offset > 0xFFFF:
            raise ValueError("ordinal %d offset exceeds 16 bits" % ordinal)
        if run and run[0][0] != obj:
            flush()
        # OW2 leaves E32EXPORT clear because export status is carried by the
        # resident-name table.  Borland set it for every exported ordinal.
        run.append((obj, flags | 0x01, offset))
    flush()
    out.append(0)
    return bytes(out)


def patch(path, backup=True):
    data = bytearray(Path(path).read_bytes())
    if data[:2] != b"MZ":
        raise ValueError("not an MZ file")
    e_lfanew = struct.unpack_from("<I", data, 0x3C)[0]
    if data[e_lfanew:e_lfanew + 2] != b"LX":
        raise ValueError("not an LX file")

    ent_rel = struct.unpack_from("<I", data, e_lfanew + 0x5C)[0]
    off = e_lfanew + ent_rel
    entries, end = parse_entry_table(data, off)
    new = build_entry_table(entries)
    delta = (end - off) - len(new)
    if delta < 0:
        raise ValueError("new entry table is larger than the original")

    # OW2 marks its far16 thunk object as a 16:16 alias object.  The original
    # Borland-built modules instead put far16 thunks in a Big/Default code
    # object.  Replace the alias bit with the Big bit on target objects.
    objtab = e_lfanew + struct.unpack_from("<I", data, e_lfanew + 0x40)[0]
    for obj in sorted({entry[2] for entry in entries}):
        flag_off = objtab + (obj - 1) * 24 + 8
        flags = struct.unpack_from("<I", data, flag_off)[0]
        struct.pack_into("<I", data, flag_off, (flags & ~0x1000) | 0x2000)

    if backup and not Path(str(path) + ".bak").exists():
        Path(str(path) + ".bak").write_bytes(data)

    # Remove the old table and insert the shorter one.
    data[off:end] = new

    # Adjust every header pointer that refers to data after the entry table.
    for field in REL_FIELDS:
        foff = e_lfanew + field
        value = struct.unpack_from("<I", data, foff)[0]
        if value > ent_rel:
            struct.pack_into("<I", data, foff, value - delta)
    for field in ABS_FIELDS:
        foff = e_lfanew + field
        value = struct.unpack_from("<I", data, foff)[0]
        if value >= end:
            struct.pack_into("<I", data, foff, value - delta)

    ldr_off = e_lfanew + 0x38
    ldrsize = struct.unpack_from("<I", data, ldr_off)[0]
    if ldrsize:
        struct.pack_into("<I", data, ldr_off, ldrsize - delta)

    Path(path).write_bytes(data)
    return len(entries), delta


if __name__ == "__main__":
    for name in sys.argv[1:]:
        n, delta = patch(name)
        print(f"{name}: converted {n} entries, shrank table by {delta} bytes")
