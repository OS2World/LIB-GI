#=============================================================
#
#	GI.MAK - Makefile for project C:\GI\gi.prj
#		Created on 11/06/93 at 08:47
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +GI.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = \BC\LIB
INCLUDEPATH = \BC\INCLUDE;\c\tools;\c\pmtools;\gi;\c\inc


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


EXE_DEPENDENCIES =  \
 gi.obj \
 gi.def

#=============================================================
#		Explicit Rules
#=============================================================
gi.dll: gi.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /v /c /wimt /wmsk /wdpl /Tod /ap /L$(LIBPATH) @&&|
\BC\LIB\C02D.OBJ+
gi.obj
gi.dll,gi
\BC\LIB\C2MT.LIB+
\BC\LIB\OS2.LIB
gi.def
|


#=============================================================
#		Individual File Dependencies
#=============================================================
gi.obj: gi.cfg gi.c 

#=============================================================
#		Compiler Configuration File
#=============================================================
gi.cfg: gi.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-sd
-H=C:\GI\gi.CSM
-vi-
-sm
-v
-wnod
-wuse
-wdef
-wasm
-wamp
-wamb
| gi.cfg


