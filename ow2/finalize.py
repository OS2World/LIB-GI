"""Finalize the Open Watcom 2 GI 1.01 binaries.

Open Watcom emits ENTRY32 export bundles and clears the E32NOINTFIX module
flag.  The original GI ABI uses ENTRY16 exports and PM-compatible DLL flags.
This script performs the post-link ABI pass and installs the finished files in
bin/ and lib/.
"""

import importlib.util
import shutil
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parent
PATCHER = ROOT / "lx_entry16_patch.py"

# Keep Open Watcom's E32NOINTFIX bit (0x10): its internal fixups are already
# applied, so clearing that bit as the Borland originals did makes the loader
# reject the image with SYS0188.  Add the original PM-compatible bit (0x200).
DLL_FLAGS = {
    "GI.DLL": 0x8210,
    "JOY.DLL": 0x8210,
    "KBD.DLL": 0x8210,
    "STRAIGHT.DLL": 0x8210,
    "TAPEDEV.DLL": 0x8210,
}
EXE_FLAGS = {
    "GICONFIG.EXE": 0x0300,
    "TAPE.EXE": 0x0300,
}
LIBS = ("gi.lib", "tapedev.lib")

spec = importlib.util.spec_from_file_location("lx_entry16_patch", PATCHER)
mod = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mod)


def patch_flags(path, flags):
    data = bytearray(Path(path).read_bytes())
    e = struct.unpack_from("<I", data, 0x3C)[0]
    if data[e:e + 2] != b"LX":
        raise ValueError("not LX: %s" % path)
    old = struct.unpack_from("<I", data, e + 0x10)[0]
    struct.pack_into("<I", data, e + 0x10, flags)
    Path(path).write_bytes(data)
    return old


def main():
    bin_dir = ROOT / "bin"
    lib_dir = ROOT / "lib"
    bin_dir.mkdir(exist_ok=True)
    lib_dir.mkdir(exist_ok=True)

    for name, flags in DLL_FLAGS.items():
        path = ROOT / name
        n, delta = mod.patch(str(path), backup=False)
        old = patch_flags(path, flags)
        shutil.copy2(path, bin_dir / name)
        print("%s: entry16=%d, table_delta=%d, flags %#06x->%#06x" %
              (name, n, delta, old, flags))
    for name, flags in EXE_FLAGS.items():
        path = ROOT / name
        old = patch_flags(path, flags)
        shutil.copy2(path, bin_dir / name)
        print("%s: flags %#06x->%#06x" % (name, old, flags))
    for name in LIBS:
        shutil.copy2(ROOT / name, lib_dir / name)
        print("%s: installed" % name)


if __name__ == "__main__":
    main()
