@echo off
SETLOCAL
@REM Glad - OpenGL thing
set obj[0].Name=build
set obj[0].ARGS=-c %~dp0include\glad\glad.c -o %~dp0bintmp\glad.o  
@REM AtulPwd - Gets the current directory of executable
set obj[1].Name=run  
set obj[1].ARGS=-c %~dp0include\other\AtulPwd.cpp -o %~dp0bintmp\AtulPwd.o  
@REM fileio - Simple file io stuff
set obj[2].Name=clean  
set obj[2].ARGS=call Clean \bintmp;call Clean \bin;


SETLOCAL
CALL :Display 5 , 10
EXIT /B %ERRORLEVEL%

:Clean
cd /d %~1
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)
cd ../
EXIT /B 0

@REM Build each object
FOR /L %%i IN (0 1 2) DO  (
   call g++ %%obj[%%i].ARGS%%

   if %ERRORLEVEL% EQU 0 (
      call echo Successfully %%obj[%%i].Name%%
   ) else (
      call echo %%obj[%%i].Name%% build failled, reason: %errorlevel%
   )
)

pause