@echo off
rem Compile DLL resources from src so POINTER file paths resolve correctly.
cd src
wrc -r -bt=os2 -i=..\h -i=. -i=%OS2TK%\h -fo=..\dll\JOY.RES joy.rc
if errorlevel 1 goto failed
wrc -r -bt=os2 -i=..\h -i=. -i=%OS2TK%\h -fo=..\dll\KBD.RES kbd.rc
if errorlevel 1 goto failed
wrc -r -bt=os2 -i=..\h -i=. -i=%OS2TK%\h -fo=..\dll\STRAIGHT.RES straight.rc
if errorlevel 1 goto failed
wrc -r -bt=os2 -i=..\h -i=. -i=%OS2TK%\h -fo=..\dll\TAPEDEV.RES tapedev.rc
if errorlevel 1 goto failed
cd ..
goto done
:failed
cd ..
echo DLL resource compilation failed
exit 1
:done
