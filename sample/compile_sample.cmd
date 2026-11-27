@echo off
rem Rebuild only the two PM sample/configuration applications from the project root.
if not "%WATCOM%"=="" goto have_watcom
set WATCOM=C:\WATCOM
:have_watcom
if not "%OS2TK%"=="" goto have_os2tk
set OS2TK=C:\OS2TK45
:have_os2tk
set PATH=%WATCOM%\binp;%PATH%
set INCLUDE=%OS2TK%\h;%WATCOM%\h;%WATCOM%\h\os2
set LIB=%WATCOM%\lib386;%WATCOM%\lib386\os2;%OS2TK%\lib
if not "%DPATH%"=="" goto have_dpath
set DPATH=.;%OS2TK%\h;%WATCOM%\h
:have_dpath
wmake -f Makefile.wat sample\GICONFIG.EXE sample\TAPE.EXE
