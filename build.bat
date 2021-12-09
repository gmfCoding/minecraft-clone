@echo off
set multiLine=-g^
 %~dp0src\*.cpp^
 -o%~dp0bin\main.exe^
 %~dp0bintmp\glad.o^
 %~dp0bintmp\fileio.o^
 -I%~dp0include^
 -I%~dp0include\nlohmann^
 -I%~dp0include\glad^
 -I%~dp0include\other^
 -L%~dp0lib^
 -lglfw3dll^
 -std=c++17^
 -DGLM_FORCE_XYZW_ONLY
 
C:\msys64\mingw64\bin\g++.exe %multiLine%
if %ERRORLEVEL% EQU 0 (
   echo Success
) else (
   echo Failure Reason Given is %errorlevel%
)