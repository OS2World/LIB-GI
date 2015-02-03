#=============================================================
#
#	KBD.MAK - Makefile for project C:\GI\kbd.PRJ
#		Created on 11/06/93 at 08:50
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +KBD.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = \BC\LIB
INCLUDEPATH = \BC\INCLUDE;\c\tools;\c\pmtools;\c\inc;\c;.


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
 kbd.res

LINK_INCLUDE =  \
 kbd.obj \
 kbd.def

#=============================================================
#		Explicit Rules
#=============================================================
kbd.dll: kbd.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)
  $(TLINK) /v /c /w-dup /wimt /wmsk /wdpl /Tod /ap /L$(LIBPATH) @&&|
\BC\LIB\C02D.OBJ+
kbd.obj
kbd.dll,kbd
\BC\LIB\C2MT.LIB+
\BC\LIB\OS2.LIB
kbd.def
|
  rc.exe kbd.res kbd.dll

#=============================================================
#		Individual File Dependencies
#=============================================================
kbd.obj: kbd.cfg kbd.c 

kbd.res: kbd.cfg KBD.RC 
	brcc.exe -R -I$(INCLUDEPATH) -FO kbd.res KBD.RC

#=============================================================
#		Compiler Configuration File
#=============================================================
kbd.cfg: kbd.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-P
-sd
-H=C:\GI\kbd.CSM
-vi-
-sm
-v
-wnod
-wuse
-wdef
-wasm
-wamp
-wamb
| kbd.cfg


