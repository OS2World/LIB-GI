#=============================================================
#
#	TAPE.MAK - Makefile for project C:\GI\tape.PRJ
#		Created on 11/18/93 at 23:07
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +TAPE.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = C:\BC\LIB
INCLUDEPATH = C:\BC\INCLUDE;\c\inc;\c\tools;\c\pmtools;.


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================
LINK_EXCLUDE =  \
 tape.res

LINK_INCLUDE =  \
 tape.obj \
 {$(LIBPATH)}tapedev.lib \
 thread.obj \
 pmthread.obj

#=============================================================
#		Explicit Rules
#=============================================================
tape.exe: tape.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)
  $(TLINK) /v /Toe /aa /L$(LIBPATH) @&&|
C:\BC\LIB\C02.OBJ+
tape.obj+
thread.obj+
pmthread.obj
tape,tape
tapedev.lib+
C:\BC\LIB\C2.LIB+
C:\BC\LIB\OS2.LIB

|
  rc.exe tape.res tape.exe

#=============================================================
#		Individual File Dependencies
#=============================================================
tape.res: tape.cfg TAPE.RC 
	brcc.exe -R -I$(INCLUDEPATH) -FO tape.res TAPE.RC

tape.obj: tape.cfg tape.cpp 

thread.obj: tape.cfg ..\c\tools\thread.CPP 
	$(CC) -c ..\c\tools\thread.CPP

pmthread.obj: tape.cfg ..\c\pmtools\pmthread.CPP 
	$(CC) -c ..\c\pmtools\pmthread.CPP

#=============================================================
#		Compiler Configuration File
#=============================================================
tape.cfg: tape.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-P
-H=C:\GI\tape.CSM
-vi-
-v
| tape.cfg


