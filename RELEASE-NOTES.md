# GI 1.01 release notes

## What is included

This is a complete Open Watcom 2 rebuild of the seven modules from the original
GI release:

* `GI.DLL`
* `JOY.DLL`
* `KBD.DLL`
* `STRAIGHT.DLL`
* `TAPEDEV.DLL`
* `GICONFIG.EXE`
* `TAPE.EXE`

The original `GAMEPORT.SYS` is shipped unchanged. It has no source in the
original release and was not rebuilt.

The binaries carry BLDLEVEL version `1.01`, keep Roman Fischer's author credit,
and are built for OS/2 Warp 4.52 / ArcaOS.

## Build

Built with:

* Open Watcom 2.0.1 (`wpp386`, `wlink`, `wrc`, `wlib`)
* OS/2 Toolkit 4.5.2 headers/libraries
* Python 3 for the post-link LX entry-table pass

Build from `ow2/build.cmd`, then run `python finalize.py` from the same
directory on a host with Python 3. The build script compiles all C/C++ sources
with `wpp386`, compiles resources with `wrc`, links the five DLLs and two PM
executables, generates `gi.lib` and `tapedev.lib` with `wlib -q -ii`, and
attaches resources in a second resource pass. The host-side finalizer performs
the `ENTRY16` post-link pass and installs the finished files into `bin/` and
`lib/`.

Source changes needed for Open Watcom were kept in `ow2/`; the original
Borland-era sources remain untouched in `gisrc/`. Notable changes are:

* OW-compatible headers and `#include <os2.h>` in `.rc` files.
* Explicit far16 conversion using `(void *)` before typed casts.
* `tape.rc` inline Borland `POINTER ... BEGIN '<hex>' ... END` blocks converted
  to raw `ptr1.ptr`, `ptr2.ptr`, and `ptr3.ptr` files.
* `ES_ANY` replaced with `0x00000000L` for OW `wrc`.
* Export names and ordinals moved from C decoration to `.lnk` `EXPORT`
  directives.
* GICONFIG/TAPE use explicit ordinal `IMPORT` directives so they bind to the
  original GI/TAPEDEV ordinals.
* `finalize.py` uses `lx_entry16_patch.py` to convert OW `ENTRY32` export bundles
  to original-style `ENTRY16` bundles in ordinal order.

## API and ABI preservation

GI exports ordinals 1-9 retain the original names:

| Ordinal | Export |
|---:|---|
| 1 | `_error` |
| 2 | `_numplayer` |
| 3 | `_fip` |
| 4 | `_GIInitialize` |
| 5 | `_GIGetDirection` |
| 6 | `_InitFip` |
| 7 | `_DelFip` |
| 8 | `_Save` |
| 9 | `_GIGetThreshold` |

Plugin exports and TAPEDEV global-data ordinals also retain the original
names and ordinal layout. Public calls use the original 32-bit `__pascal`
ABI with callee cleanup, and plugin far16 parameters are converted through the
OW runtime helpers.

One build-level difference remains: the DLLs keep Open Watcom's
`E32NOINTFIX` bit (`0x10`) and therefore do not carry the original Borland
`E32LIBINIT` bit (`0x4`) alone. OW's internal fixups are already applied;
clearing `E32NOINTFIX` to mirror the original flags exactly makes the OS/2
loader reject the images with `SYS0188`. The original PM-compatible bit is
restored, giving final DLL flags of `0x8210`. This is a module-load flag
difference only; the exported API names, ordinals, and calling convention are
preserved.

## Verification

Verification was performed on the ArcaOS/Warp 4.52 VM:

* All seven targets link successfully and contain `1.01` BLDLEVEL strings.
* A rebuild from the final repository sources reproduced all seven binaries
  byte-for-byte.
* `lx_export.py` confirmed export names, ordinals, resident names, imports,
  resources, and module flags for all rebuilt modules.
* `DosLoadModule` succeeded for the rebuilt GI.
* `DosQueryProcAddr` returned `NO_ERROR` for GI ordinals 1-9.
* `DosQueryProcType` reported all nine GI entries as 16-bit entries.
* Original `GICONFIG.EXE` and `TAPE.EXE` launched successfully against the
  rebuilt DLLs.
* Rebuilt `GICONFIG.EXE` and `TAPE.EXE` launched successfully against the
  rebuilt DLLs.
* No `C:\POPUPLOG.OS2` entry was produced by any of the four client/DLL
  combinations.

## Original notice

GI is free! Please use, extend, spread, ... GI as you like.

Original author: Roman Fischer. The INF documentation also credits Markus Hof,
Andre "Infid" Miodonski, and Frank Ivan / ICA Systems.
