@echo off

pushd %~dp0

md ..\build 2> nul
pushd ..\build
del /q *

cl ..\code\*.c /Fe: game.exe

popd
popd
