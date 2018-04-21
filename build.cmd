@echo

set qm=D:\qt\470\qt\bin\qmake.exe
set path=D:\qt\470\mingw\bin
set mm=%path%\mingw32-make.exe

%qm% ReadEpub.pro -r -spec win32-g++ CONFIG+=release
%mm% -w
