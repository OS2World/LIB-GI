@echo off

@echo Building: GISRC.ZIP
zip gisrc.zip \c\inc\errdef.h \c\inc\std.h \c\tools\thread.cpp \c\tools\thread.hpp
zip gisrc.zip \c\tools\error.cpp \c\tools\error.hpp \c\pmtools\checkbox.hpp
zip gisrc.zip \c\pmtools\slider.hpp \c\pmtools\listbox.hpp \c\pmtools\spin.hpp
zip gisrc.zip \c\pmtools\pmthread.cpp \c\pmtools\pmthread.hpp
zip gisrc.zip joy.c joy.prj joy.rc joy.def joy.mak
zip gisrc.zip kbd.c kbd.prj kbd.rc kbd.def kbdconv.h kbd.mak
zip gisrc.zip straight.c straight.prj straight.rc straight.mak
zip gisrc.zip gi.c gi.h gi.prj gi.def gi.mak
zip gisrc.zip giconfig.cpp giconfig.rc giconfig.prj giconfig.mak
zip gisrc.zip tapedev.c tapedev.prj tapedev.def tapedev.rc tapedev.mak
zip gisrc.zip tape.cpp tape.prj tape.rc tape.mak
zip gisrc.zip *.mak gi.tag mkinf.cmd
zip gisrc.zip version.cmd gimake.cmd

@echo Building: GIRUN.ZIP
zip girun.zip gi.dll joy.dll kbd.dll straight.dll giconfig.exe gameport.sys
zip girun.zip tapedev.dll tape.exe

@echo Building: GI.ZIP
zip gi.zip gisrc.zip girun.zip gi.inf readme

if %1.=. then goto end
@echo Creating Directory
md %1
@echo Copying...
copy gi.zip    %1
:end

