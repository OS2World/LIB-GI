#=============================================================
#
#	GICONFIG.MAK - Makefile for project C:\GI\giconfig.PRJ
#		Created on 11/06/93 at 08:49
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +GICONFIG.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = \BC\LIB
INCLUDEPATH = \BC\INCLUDE;\c\tools;\c\pmtools;\c\inc;.


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
 giconfig.res

LINK_INCLUDE =  \
 pmthread.obj \
 error.obj \
 thread.obj \
 giconfig.obj \
 {$(LIBPATH)}gi.lib

#=============================================================
#		Explicit Rules
#=============================================================
giconfig.exe: giconfig.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)
  $(TLINK) /v /c /wimt /wmsk /wdpl /Toe /aa /L$(LIBPATH) @&&|
\BC\LIB\C02.OBJ+
pmthread.obj+
error.obj+
thread.obj+
giconfig.obj
giconfig,giconfig
gi.lib+
\BC\LIB\C2.LIB+
\BC\LIB\OS2.LIB

|
  rc.exe giconfig.res giconfig.exe

#=============================================================
#		Individual File Dependencies
#=============================================================
pmthread.obj: giconfig.cfg ..\c\pmtools\pmthread.cpp 
	$(CC) -c ..\c\pmtools\pmthread.cpp

error.obj: giconfig.cfg ..\c\tools\error.cpp 
	$(CC) -c ..\c\tools\error.cpp

thread.obj: giconfig.cfg ..\c\tools\thread.cpp 
	$(CC) -c ..\c\tools\thread.cpp

giconfig.res: giconfig.cfg GICONFIG.RC 
	brcc.exe -R -I$(INCLUDEPATH) -FO giconfig.res GICONFIG.RC

giconfig.obj: giconfig.cfg giconfig.CPP 

#=============================================================
#		Compiler Configuration File
#=============================================================
giconfig.cfg: giconfig.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-P
-H=C:\GI\giconfig.CSM
-vi-
-v
-wnod
-wuse
-wdef
-wasm
-wamp
-wamb
| giconfig.cfg


