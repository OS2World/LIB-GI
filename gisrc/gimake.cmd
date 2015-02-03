@echo off
@echo Deleting files...
del *.obj
del *.map
del *.dll
del *.exe

@echo Re-building ALL GI-Modules...

make -B -fgi.mak
make -B -fgiconfig.mak
make -B -fjoy.mak
make -B -fkbd.mak
make -B -fstraight.mak
make -B -ftapedev.mak
make -B -ftape.mak

@echo All GI-Modules were rebuilt

