rem @echo off
call build_sws.cmd

copy /Y alut.dll release\
copy /Y game.exe release\
copy /Y editor.exe release\

copy /Y *.sw release\