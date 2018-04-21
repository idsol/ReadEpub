xcopy build\release\ReadEpub.exe deploy /q /y
xcopy build\release\quazip.dll deploy /q /y
xcopy translations\*.qm build\release\translations /q /y
xcopy translations\*.qm deploy\translations /q /y
pause
