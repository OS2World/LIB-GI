@echo off
rem Open Watcom 2 rebuild for GI 1.01 (OS/2 Warp 4.52 / ArcaOS).
rem Run from this directory after setting WATCOM, INCLUDE and LIB as below.

set WATCOM=C:\WATCOM
set PATH=C:\WATCOM\binp;%PATH%
set INCLUDE=.;.\c\inc;.\c\tools;.\c\pmtools;C:\OS2TK45\h;C:\WATCOM\h;C:\WATCOM\h\os2
set LIB=C:\WATCOM\lib386;C:\WATCOM\lib386\os2;C:\OS2TK45\lib

if not exist obj md obj

wpp386 -bt=os2 -mf -5 -W3 -fo=obj\gi.obj gi.c
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\joy.obj joy.c
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\kbd.obj kbd.c
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\straight.obj straight.c
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\tapedev.obj tapedev.c
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\giconfig.obj giconfig.cpp
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\tape.obj tape.cpp
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\pmthread.obj c\pmtools\pmthread.cpp
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\error.obj c\tools\error.cpp
wpp386 -bt=os2 -mf -5 -W3 -fo=obj\thread.obj c\tools\thread.cpp

wrc -r -i=C:\OS2TK45\h -i=C:\WATCOM\h -i=C:\WATCOM\h\os2 -fo=obj\giconfig.res giconfig.rc
wrc -r -i=C:\OS2TK45\h -i=C:\WATCOM\h -i=C:\WATCOM\h\os2 -fo=obj\joy.res joy.rc
wrc -r -i=C:\OS2TK45\h -i=C:\WATCOM\h -i=C:\WATCOM\h\os2 -fo=obj\kbd.res kbd.rc
wrc -r -i=C:\OS2TK45\h -i=C:\WATCOM\h -i=C:\WATCOM\h\os2 -fo=obj\straight.res straight.rc
wrc -r -i=C:\OS2TK45\h -i=C:\WATCOM\h -i=C:\WATCOM\h\os2 -fo=obj\tape.res tape.rc
wrc -r -i=C:\OS2TK45\h -i=C:\WATCOM\h -i=C:\WATCOM\h\os2 -fo=obj\tapedev.res tapedev.rc

wlink @gi.lnk
wlink @joy.lnk
wlink @kbd.lnk
wlink @straight.lnk
wlink @tapedev.lnk
wlib -q -ii gi.lib +GI.DLL
wlib -q -ii tapedev.lib +TAPEDEV.DLL
wlink @giconfig.lnk
wlink @tape.lnk

wrc -bt=os2 obj\giconfig.res GICONFIG.EXE
wrc -bt=os2 obj\joy.res JOY.DLL
wrc -bt=os2 obj\kbd.res KBD.DLL
wrc -bt=os2 obj\straight.res STRAIGHT.DLL
wrc -bt=os2 obj\tape.res TAPE.EXE
wrc -bt=os2 obj\tapedev.res TAPEDEV.DLL

rem Final ABI pass is host-side (the OS/2 test VM has no Python):
rem   python finalize.py
rem finalize.py rewrites ENTRY32 exports as ENTRY16, sets module flags, and
rem installs the finished files into bin/ and lib/.
