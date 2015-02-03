#=============================================================
#
#	TAPEDEV.MAK - Makefile for project C:\GI\tapedev.prj
#		Created on 11/18/93 at 23:05
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +TAPEDEV.CFG
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
 tapedev.res

LINK_INCLUDE =  \
 tapedev.obj \
 tapedev.def \
 {$(LIBPATH)}gi.lib

#=============================================================
#		Explicit Rules
#=============================================================
tapedev.dll: tapedev.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)
  $(TLINK) /v /c /Tod /ap /L$(LIBPATH) @&&|
C:\BC\LIB\C02D.OBJ+
tapedev.obj
tapedev.dll,tapedev
gi.lib+
C:\BC\LIB\C2MT.LIB+
C:\BC\LIB\OS2.LIB
tapedev.def
|
  rc.exe tapedev.res tapedev.dll

#=============================================================
#		Individual File Dependencies
#=============================================================
tapedev.obj: tapedev.cfg tapedev.c 

tapedev.res: tapedev.cfg TAPEDEV.RC 
	brcc.exe -R -I$(INCLUDEPATH) -FO tapedev.res TAPEDEV.RC

#=============================================================
#		Compiler Configuration File
#=============================================================
tapedev.cfg: tapedev.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-P
-sd
-H=C:\GI\tapedev.CSM
-vi-
-sm
-v
| tapedev.cfg


