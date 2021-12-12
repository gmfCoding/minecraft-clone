@echo off
set gladArgs=-c %~dp0include\glad\glad.c^
 -o %~dp0bintmp\glad.o

 set fileIO=-c %~dp0include\other\fileio.cpp^
 -o %~dp0bintmp\fileio.o
 
C:\msys64\mingw64\bin\g++.exe %gladArgs%
if %ERRORLEVEL% EQU 0 (
   echo Successfully Built glad
) else (
   echo glad build failled, reason: %errorlevel%
)

C:\msys64\mingw64\bin\g++.exe %fileIO%
if %ERRORLEVEL% EQU 0 (
   echo Successfully Built fileio
) else (
   echo fileio build failled, reason: %errorlevel%
)

pause