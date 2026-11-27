# LIB-GI

GI is a generic interface between games and input devices. This repository
contains the original 1993 sources and a reproducible Open Watcom 2 rebuild
for OS/2 Warp 4.52 / ArcaOS as version **1.01**.

The rebuild preserves the original GI API, export names and ordinals, BLDLEVEL
author credit, and freeware notice. The finished DLLs and import libraries are
tracked here, so a clean checkout can be inspected without running a build.

## Requirements

* Open Watcom 2.0.1: `wpp386`, `wlink`, `wrc`, `wlib`, `wmake`
* OS/2 Toolkit 4.5.2 headers (tested with `C:\OS2TK45`)
* An OS/2 Warp 4.52 or ArcaOS host to build and run the PM samples

Python is **not** required. The post-link ABI pass is an OS/2 C program built
by the makefile.

## Quick start

From the project root on the OS/2 build machine:

```bat
compile_wat.cmd
```

This sets the Watcom/Toolkit environment if needed, runs `wmake clean` followed
by `wmake -f Makefile.wat all` into `compile.log`, checks `ERRORLEVEL`, and
types the log. A successful build ends with `GI 1.01 BUILD OK`.

To rebuild only the two PM configuration applications after the DLLs are in
place:

```bat
sample\compile_sample.cmd
```

## Repository layout

| Path | Contents |
|---|---|
| `Makefile.wat` | Open Watcom 2 build definition |
| `compile_wat.cmd` | Clean full-build wrapper |
| `dll\` | Linker directives, rebuilt DLLs, and unchanged `GAMEPORT.SYS` |
| `lib\` | `gi.lib` and `tapedev.lib` import libraries |
| `h\` | Public and internal headers |
| `src\` | Library sources, `.def` files, and resource scripts |
| `sample\` | GI configuration and tape-device PM samples |
| `doc\` | `gi.INF`, `readme.txt`, and release documentation |
| `RELEASE-NOTES.md` | Build, ABI, and verification details |

The historical Borland-era tree (`gisrc/`) and the first rebuild workspace
(`ow2/`) were retired after the layout reorganization; their contents remain
available in Git history where needed.

## Build details

`Makefile.wat` uses `.ERASE`, `&` continuations, and `.SYMBOLIC` targets. It
compiles all C/C++ sources with `wpp386 -bt=os2 -mf -5 -W3`, links with the
original `.lnk` directives, creates import libraries with `wlib -q -ii`, and
binds resources with a second `wrc` pass.

A second `wrc` pass is required because `wlink`'s `OPTION RESOURCE` is for
Win32; it does not bind 32-bit OS/2 LX resources. `wrc` resolves `ICON` and
`BITMAP` paths relative to its working directory, which is why the resource
wrappers run from `src\` and `sample\`.

After linking, `dll\ENTRY16.EXE` rewrites the OW2 `ENTRY32` export bundles back
to the original `ENTRY16` ABI in ordinal order, restores the PM-compatible DLL
module bit, and normalizes the `wlib` wall-clock timestamp in the two import
libraries to the 1.01 release values. That normalization makes the import
libraries byte-for-byte reproducible; it does not change any imported symbol.

The DLLs intentionally keep Open Watcom's `E32NOINTFIX` bit (`0x10`). Clearing
it makes the OS/2 loader reject these OW-linked images with `SYS0188`. The
final DLL module flags are `0x8210`.

## Verify the build

On the ArcaOS/Warp 4.52 test machine:

```bat
C:\OS2\BLDLEVEL.EXE dll\GI.DLL
C:\OS2\BLDLEVEL.EXE sample\GICONFIG.EXE
C:\OS2\BLDLEVEL.EXE sample\TAPE.EXE
```

Expected signatures:

* `@#OS2World:1.01#@GI 1.01 - Game Interface by Roman Fischer (Open Watcom 2)`
* `@#OS2World:1.01#@GICONFIG 1.01 - GI configuration by Roman Fischer (Open Watcom 2)`
* `@#OS2World:1.01#@TAPE 1.01 - GI tape control by Roman Fischer (Open Watcom 2)`

On the host, `claude-os2-toolkit/tools/lx_export.py` confirms the export names,
ordinals, entry types, imports, and the third read-only LX resource object.
A clean rebuild reproduces all seven binaries and both import libraries
byte-for-byte.

## Running the samples

`GICONFIG.EXE` and `TAPE.EXE` are Presentation Manager applications. Launch
them from a PM desktop folder or PM command prompt; a plain `detach` starts a
background process (`PT_DETACHED`) and is not a valid test of their `LibMain`
behavior. The automated VM check uses `DosStartSession` with `SSF_TYPE_PM`, so
the samples themselves run in a PM session even though the SSH launcher is
detached. The samples import `GI.DLL`, `TAPEDEV.DLL`, and `GAMEPORT.SYS` from
the `dll\` directory, so keep that directory on `LIBPATH` or copy the runtime
files beside the sample executables when testing.

A normal test run should show the configuration window, close cleanly, and
leave no new entry in `C:\POPUPLOG.OS2`.

## Public API and ordinals

GI exports ordinals 1-9 with the original names:

1. `_error`
2. `_numplayer`
3. `_fip`
4. `_GIInitialize`
5. `_GIGetDirection`
6. `_InitFip`
7. `_DelFip`
8. `_Save`
9. `_GIGetThreshold`

Plugin exports and TAPEDEV global-data ordinals also retain the original names
and ordinal layout. Public calls use the original 32-bit `__pascal` ABI with
callee cleanup; plugin far16 parameters are converted through the Open Watcom
runtime helpers.

## Version information

Package, linker, wrapper, and documentation version references are `1.01`.
The `V1.00` strings in historical source comments and in the configuration-file
header (`"Game-Interface Config-File V1.00"`) are intentionally preserved:
they are part of the original source text and on-disk config format, and
changing them would alter behavior without changing the package version.

## Documentation

Read `doc/gi.INF`, especially its **Installation** and **Configuration**
sections, before installing or configuring GI. `doc/readme.txt` is the original
short pointer to that manual. `RELEASE-NOTES.md` records the exact rebuild
changes and verification results.

## Known constraints

* PM samples must be launched from a PM context; VIO/detached launches are not
  representative.
* `GAMEPORT.SYS` has no source in the original release and is shipped unchanged.
* The original `.INF` is distributed as documentation; this rebuild does not
  claim to have regenerated it from a lost IPF source.

## License / freeware notice

GI is free! Please use, extend, spread, ... GI as you like.

## Author

Roman Fischer

The original documentation also credits Markus Hof, Andre "Infid" Miodonski,
and Frank Ivan / ICA Systems for joystick-driver contributions.
