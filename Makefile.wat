# GI 1.01 - Open Watcom 2 build for OS/2 Warp 4.52 / ArcaOS
# Run compile_wat.cmd from the project root (or wmake after setting the env).

WATCOM = C:\WATCOM
OS2TK = C:\OS2TK45
CC = wpp386
RC = wrc
WL = wlink
WLIB = wlib
CFLAGS = -bt=os2 -mf -5 -W3 -I=h -I=src -I=$(OS2TK)\h -I=$(WATCOM)\h -I=$(WATCOM)\h\os2

.ERASE

all: resources dll\GI.DLL dll\JOY.DLL dll\KBD.DLL dll\STRAIGHT.DLL dll\TAPEDEV.DLL lib\gi.lib lib\tapedev.lib sample\GICONFIG.EXE sample\TAPE.EXE finalize .SYMBOLIC
	@echo GI 1.01 build complete

# ---- common helper objects ----

.ERASE
dll\PMTHREAD.OBJ: src\pmthread.cpp h\pmthread.hpp h\thread.hpp h\std.h
	$(CC) $(CFLAGS) -fo=dll\PMTHREAD.OBJ src\pmthread.cpp

.ERASE
dll\ERROR.OBJ: src\error.cpp h\error.hpp
	$(CC) $(CFLAGS) -fo=dll\ERROR.OBJ src\error.cpp

.ERASE
dll\THREAD.OBJ: src\thread.cpp h\thread.hpp h\errdef.h h\error.hpp
	$(CC) $(CFLAGS) -fo=dll\THREAD.OBJ src\thread.cpp

# ---- GI core and plugins ----

.ERASE
dll\GI.OBJ: src\gi.c h\gi.h h\errdef.h
	$(CC) $(CFLAGS) -fo=dll\GI.OBJ src\gi.c

.ERASE
dll\JOY.OBJ: src\joy.c h\gi.h h\checkbox.hpp h\slider.hpp
	$(CC) $(CFLAGS) -fo=dll\JOY.OBJ src\joy.c

.ERASE
dll\KBD.OBJ: src\kbd.c h\gi.h h\checkbox.hpp h\listbox.hpp h\slider.hpp h\kbdconv.h
	$(CC) $(CFLAGS) -fo=dll\KBD.OBJ src\kbd.c

.ERASE
dll\STRAIGHT.OBJ: src\straight.c h\gi.h h\checkbox.hpp h\slider.hpp
	$(CC) $(CFLAGS) -fo=dll\STRAIGHT.OBJ src\straight.c

.ERASE
dll\TAPEDEV.OBJ: src\tapedev.c h\gi.h h\spin.hpp h\checkbox.hpp
	$(CC) $(CFLAGS) -fo=dll\TAPEDEV.OBJ src\tapedev.c

# ---- sample/configuration applications ----

.ERASE
dll\GICONFIG.OBJ: sample\giconfig.cpp h\gi.h h\dir.h h\listbox.hpp h\pmthread.hpp
	$(CC) $(CFLAGS) -fo=dll\GICONFIG.OBJ sample\giconfig.cpp

.ERASE
dll\TAPE.OBJ: sample\tape.cpp h\dir.h h\pmthread.hpp h\checkbox.hpp
	$(CC) $(CFLAGS) -fo=dll\TAPE.OBJ sample\tape.cpp

# ---- resources (wrc pass one, from the directories that own the .rc files) ----

resources: .SYMBOLIC
	call src\compile_res.cmd
	call sample\compile_res.cmd

.ERASE
dll\JOY.RES: src\joy.rc h\checkbox.hpp h\slider.hpp
	call src\compile_res.cmd

.ERASE
dll\KBD.RES: src\kbd.rc h\checkbox.hpp h\listbox.hpp h\slider.hpp
	call src\compile_res.cmd

.ERASE
dll\STRAIGHT.RES: src\straight.rc h\checkbox.hpp h\slider.hpp
	call src\compile_res.cmd

.ERASE
dll\TAPEDEV.RES: src\tapedev.rc h\spin.hpp h\checkbox.hpp
	call src\compile_res.cmd

.ERASE
sample\GICONFIG.RES: sample\giconfig.rc h\gi.h h\dir.h
	call sample\compile_res.cmd

.ERASE
sample\TAPE.RES: sample\tape.rc sample\ptr1.ptr sample\ptr2.ptr sample\ptr3.ptr
	call sample\compile_res.cmd

# ---- DLLs ----

.ERASE
dll\GI.DLL: dll\GI.OBJ dll\gi.lnk
	$(WL) @dll\gi.lnk

.ERASE
dll\JOY.DLL: dll\JOY.OBJ dll\joy.lnk dll\JOY.RES
	$(WL) @dll\joy.lnk
	$(RC) -bt=os2 dll\JOY.RES dll\JOY.DLL

.ERASE
dll\KBD.DLL: dll\KBD.OBJ dll\kbd.lnk dll\KBD.RES
	$(WL) @dll\kbd.lnk
	$(RC) -bt=os2 dll\KBD.RES dll\KBD.DLL

.ERASE
dll\STRAIGHT.DLL: dll\STRAIGHT.OBJ dll\straight.lnk dll\STRAIGHT.RES
	$(WL) @dll\straight.lnk
	$(RC) -bt=os2 dll\STRAIGHT.RES dll\STRAIGHT.DLL

.ERASE
dll\TAPEDEV.DLL: dll\TAPEDEV.OBJ dll\tapedev.lnk dll\TAPEDEV.RES
	$(WL) @dll\tapedev.lnk
	$(RC) -bt=os2 dll\TAPEDEV.RES dll\TAPEDEV.DLL

# ---- import libraries ----

.ERASE
lib\gi.lib: dll\GI.DLL
	$(WLIB) -q -ii -b lib\gi.lib +dll\GI.DLL

.ERASE
lib\tapedev.lib: dll\TAPEDEV.DLL
	$(WLIB) -q -ii -b lib\tapedev.lib +dll\TAPEDEV.DLL

# ---- samples ----

.ERASE
sample\GICONFIG.EXE: dll\GICONFIG.OBJ dll\PMTHREAD.OBJ dll\ERROR.OBJ dll\THREAD.OBJ dll\GI.DLL sample\giconfig.lnk sample\GICONFIG.RES
	$(WL) @sample\giconfig.lnk
	$(RC) -bt=os2 sample\GICONFIG.RES sample\GICONFIG.EXE

.ERASE
sample\TAPE.EXE: dll\TAPE.OBJ dll\PMTHREAD.OBJ dll\ERROR.OBJ dll\THREAD.OBJ dll\TAPEDEV.DLL sample\tape.lnk sample\TAPE.RES
	$(WL) @sample\tape.lnk
	$(RC) -bt=os2 sample\TAPE.RES sample\TAPE.EXE

# ---- host-side ABI pass, implemented as an OS/2 executable ----

.ERASE
dll\ENTRY16.OBJ: src\entry16.c
	$(CC) -bt=os2 -mf -5 -W3 -fo=dll\ENTRY16.OBJ src\entry16.c

.ERASE
dll\ENTRY16.EXE: dll\ENTRY16.OBJ dll\entry16.lnk
	$(WL) @dll\entry16.lnk

finalize: .SYMBOLIC dll\ENTRY16.EXE dll\GI.DLL dll\JOY.DLL dll\KBD.DLL dll\STRAIGHT.DLL dll\TAPEDEV.DLL lib\gi.lib lib\tapedev.lib sample\GICONFIG.EXE sample\TAPE.EXE
	dll\ENTRY16.EXE dll\GI.DLL dll\JOY.DLL dll\KBD.DLL &
		dll\STRAIGHT.DLL dll\TAPEDEV.DLL sample\GICONFIG.EXE sample\TAPE.EXE &
		lib\gi.lib lib\tapedev.lib

# ---- cleanup ----

clean: .SYMBOLIC
	-del dll\*.obj dll\*.res dll\*.map dll\*.dll dll\*.exe
	-del lib\*.lib
	-del sample\*.obj sample\*.res sample\*.map sample\*.exe
