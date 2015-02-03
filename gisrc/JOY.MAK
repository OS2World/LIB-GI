#=============================================================
#
#	JOY.MAK - Makefile for project C:\GI\joy.PRJ
#		Created on 11/06/93 at 08:50
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +JOY.CFG
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
 joy.res

LINK_INCLUDE =  \
 joy.obj \
 joy.def

#=============================================================
#		Explicit Rules
#=============================================================
joy.dll: joy.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)
  $(TLINK) /v /c /Tod /ap /L$(LIBPATH) @&&|
\BC\LIB\C02D.OBJ+
joy.obj
joy.dll,joy
\BC\LIB\C2MT.LIB+
\BC\LIB\OS2.LIB
joy.def
|
  rc.exe joy.res joy.dll

#=============================================================
#		Individual File Dependencies
#=============================================================
joy.obj: joy.cfg joy.c 

joy.res: joy.cfg JOY.RC 
	brcc.exe -R -I$(INCLUDEPATH) -FO joy.res JOY.RC

#=============================================================
#		Compiler Configuration File
#=============================================================
joy.cfg: joy.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-P
-sd
-H=C:\GI\joy.CSM
-vi-
-sm
-v
| joy.cfg


