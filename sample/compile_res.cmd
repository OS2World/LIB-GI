@echo off
rem Compile sample resources from sample so POINTER file paths resolve correctly.
cd sample
wrc -r -bt=os2 -i=..\h -i=. -i=%OS2TK%\h -fo=GICONFIG.RES giconfig.rc
if errorlevel 1 goto failed
wrc -r -bt=os2 -i=..\h -i=. -i=%OS2TK%\h -fo=TAPE.RES tape.rc
if errorlevel 1 goto failed
cd ..
goto done
:failed
cd ..
echo Sample resource compilation failed
exit 1
:done
