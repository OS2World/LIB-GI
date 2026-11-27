# LIB-GI

GI is a generic interface between games and input devices.

This repository contains the original 1993 sources in `gisrc/` and a verified
Open Watcom 2 rebuild as version **1.01** in `ow2/`.

## Version 1.01 rebuild

The Open Watcom 2 build reproduces all seven original modules and keeps the
original GI API, export names, export ordinals, BLDLEVEL author credit, and
freeware notice.

| Module | Description |
|---|---|
| `GI.DLL` | Game Interface core |
| `JOY.DLL` | joystick input plugin |
| `KBD.DLL` | keyboard input plugin |
| `STRAIGHT.DLL` | straight/serial-style input plugin |
| `TAPEDEV.DLL` | tape-device input plugin |
| `GICONFIG.EXE` | GI configuration PM application |
| `TAPE.EXE` | tape-device configuration PM application |

The finished binaries and import libraries are in `ow2/bin/` and `ow2/lib/`.
`GAMEPORT.SYS` has no source in the original release and is shipped unchanged
as a runtime file in `ow2/bin/GAMEPORT.SYS`.

See [RELEASE-NOTES.md](RELEASE-NOTES.md) for build, ABI, and verification
details.

## Building

Requirements:

* Open Watcom 2.0.1 (`wpp386`, `wlink`, `wrc`, `wlib`)
* OS/2 Toolkit 4.5 headers and libraries
* Python 3 for the post-link `ENTRY16` ABI pass

From the `ow2` directory:

```bat
build.cmd
```

Then, on a host with Python 3, run from the same directory:

```bat
python finalize.py
```

`build.cmd` compiles the C/C++ sources with `wpp386`, compiles resources with
`wrc`, links all seven targets, creates `gi.lib` and `tapedev.lib`, and attaches
resources. `finalize.py` rewrites OW2 `ENTRY32` export bundles back to the
original `ENTRY16` ABI, applies module flags, and installs the verified files
into `bin/` and `lib/`.

## Installation

Read `gi.INF`, especially the **Installation** and **Configuration** sections,
before installing or configuring GI.

## Compatibility notes

The exported GI entry points keep the original names and ordinals:

1. `_error`
2. `_numplayer`
3. `_fip`
4. `_GIInitialize`
5. `_GIGetDirection`
6. `_InitFip`
7. `_DelFip`
8. `_Save`
9. `_GIGetThreshold`

The DLLs use original-style `ENTRY16` exports and 32-bit `__pascal` calling
conventions. Open Watcom's `E32NOINTFIX` module bit is intentionally kept;
clearing it makes the OS/2 loader reject these OW-linked images with `SYS0188`.
The original PM-compatible DLL bit is restored. Exported API behavior and
ordinal resolution were verified on ArcaOS/Warp 4.52.

## License / freeware notice

GI is free! Please use, extend, spread, ... GI as you like.

## Author

Roman Fischer

The original documentation also credits Markus Hof, Andre "Infid" Miodonski,
and Frank Ivan / ICA Systems for joystick-driver contributions.
