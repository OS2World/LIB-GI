#=============================================================
#
#	STRAIGHT.MAK - Makefile for project C:\GI\straight.PRJ
#		Created on 11/06/93 at 08:50
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +STRAIGHT.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = \bc\lib;\c
INCLUDEPATH = \bc\include;.;\C\tools;\c\pmtools;\c\inc;\c


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
 straight.res

LINK_INCLUDE =  \
 straight.obj

#=============================================================
#		Explicit Rules
#=============================================================
straight.dll: straight.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)
  $(TLINK) /v /x /Tod /ap /L$(LIBPATH) @&&|
\bc\lib\C02D.OBJ+
straight.obj
straight.dll
		# no map file
\bc\lib\C2MT.LIB+
\bc\lib\OS2.LIB

|
  rc.exe straight.res straight.dll

#=============================================================
#		Individual File Dependencies
#=============================================================
straight.obj: straight.cfg straight.c 

straight.res: straight.cfg STRAIGHT.RC 
	brcc.exe -R -I$(INCLUDEPATH) -FO straight.res STRAIGHT.RC

#=============================================================
#		Compiler Configuration File
#=============================================================
straight.cfg: straight.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-P
-sd
-H=C:\GI\straight.CSM
-vi-
-sm
-v
-C
-K
-wnod
-wuse
-wsig
-wdef
-w-par
-wamb
| straight.cfg


