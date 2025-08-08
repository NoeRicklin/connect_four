@echo off

pushd %~dp0

md ..\build 2> nul
md bots_params 2> nul

pushd ..\build
del /q *

cl ..\code\*.c /Fe: game.exe /W4 /Zi

popd
popd
