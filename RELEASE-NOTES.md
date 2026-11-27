# GI 1.01 release notes

## What is included

This is a complete, reproducible Open Watcom 2 rebuild of the seven modules
from the original GI release:

* `GI.DLL`
* `JOY.DLL`
* `KBD.DLL`
* `STRAIGHT.DLL`
* `TAPEDEV.DLL`
* `GICONFIG.EXE`
* `TAPE.EXE`

`GAMEPORT.SYS` is shipped unchanged in `dll\`; it has no source in the original
release and was not rebuilt. The finished import libraries are `lib\gi.lib` and
`lib\tapedev.lib`.

The binaries carry BLDLEVEL version `1.01`, keep Roman Fischer's author credit,
and are built for OS/2 Warp 4.52 / ArcaOS.

## Build

The build is driven from the project root:

```bat
compile_wat.cmd
```

Requirements:

* Open Watcom 2.0.1 (`wpp386`, `wlink`, `wrc`, `wlib`, `wmake`)
* OS/2 Toolkit 4.5.2 headers (tested with `C:\OS2TK45`)

`compile_wat.cmd` sets `WATCOM`, `OS2TK`, `PATH`, `INCLUDE`, `LIB`, and `DPATH`
when they are not already set, runs `wmake -f Makefile.wat clean`, then
`wmake -f Makefile.wat all` into `compile.log`, checks `ERRORLEVEL`, and types
the log. A successful build prints `GI 1.01 BUILD OK`.

`Makefile.wat` compiles with `wpp386 -bt=os2 -mf -5 -W3`, links each target
with its original `.lnk` directives, builds `gi.lib` and `tapedev.lib` with
`wlib -q -ii`, and binds resources with a second `wrc` pass because `wlink`'s
`OPTION RESOURCE` only applies to Win32, not 32-bit OS/2 LX resources.

The post-link ABI pass is `src\entry16.c`, built as `dll\ENTRY16.EXE`. No
Python is required. It:

* converts OW2 `ENTRY32` export bundles back to the original `ENTRY16` layout
  in ordinal order;
* marks the thunk objects Big instead of 16:16 alias and adjusts LX offsets;
* keeps `E32NOINTFIX` and restores the PM-compatible DLL bit, giving final DLL
  flags of `0x8210`;
* normalizes the wall-clock timestamp that `wlib` embeds in the two import
  libraries to the 1.01 release values, making those libraries reproducible.

Source changes needed for Open Watcom live in `src\`, `h\`, `sample\`, and
`dll\`. The historical Borland-era tree and the first rebuild workspace were
removed from this layout; both remain in Git history. Notable changes are:

* OW-compatible headers and `#include <os2.h>` in `.rc` files.
* Explicit far16 conversion using `(void *)` before typed casts.
* `tape.rc` inline Borland `POINTER ... BEGIN '<hex>' ... END` blocks converted
  to raw `ptr1.ptr`, `ptr2.ptr`, and `ptr3.ptr` files in `sample\`.
* `ES_ANY` replaced with `0x00000000L` for OW `wrc`.
* Export names and ordinals moved from C decoration to `.lnk` `EXPORT`
  directives.
* GICONFIG/TAPE use explicit ordinal `IMPORT` directives so they bind to the
  original GI/TAPEDEV ordinals.

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

Plugin exports and TAPEDEV global-data ordinals also retain the original names
and ordinal layout. Public calls use the original 32-bit `__pascal` ABI with
callee cleanup, and plugin far16 parameters are converted through the OW
runtime helpers.

One build-level difference remains: the DLLs keep Open Watcom's
`E32NOINTFIX` bit (`0x10`) and therefore do not carry the original Borland
`E32LIBINIT` bit (`0x4`) alone. Clearing `E32NOINTFIX` to mirror the original
flags exactly makes the OS/2 loader reject the images with `SYS0188`. The
original PM-compatible bit is restored, giving final DLL flags of `0x8210`.
This is a module-load flag difference only; the exported API names, ordinals,
and calling convention are preserved.

The `V1.00` strings in historical source comments and in the configuration
header `"Game-Interface Config-File V1.00"` are intentionally preserved. They
belong to the original source text and config format; changing them would alter
behavior without changing the package version.

## Verification

Verification was performed with the ArcaOS/Warp 4.52 test VM:

* A clean `compile_wat.cmd` build completed with no errors.
* `C:\OS2\BLDLEVEL.EXE` reported revision `1.01` and the expected descriptions
  for `GI.DLL`, `GICONFIG.EXE`, and `TAPE.EXE`.
* `lx_export.py` confirmed export names, ordinals, resident names, imports,
  and module flags for the rebuilt modules. Bound LX resource objects are
  present in the modules that had them in the original release.
* A clean rebuild reproduced all seven binaries and both import libraries
  byte-for-byte from the tracked sources.
* Both PM samples were started as `SSF_TYPE_PM` sessions on the ArcaOS VM with
  `DosStartSession`; each returned a session and process ID and produced no new
  `C:\POPUPLOG.OS2` entry.

## Original notice

GI is free! Please use, extend, spread, ... GI as you like.

Original author: Roman Fischer. The INF documentation also credits Markus Hof,
Andre "Infid" Miodonski, and Frank Ivan / ICA Systems.
