@echo off
@REM Glad - OpenGL thing
set obj[0].Name=Glad  
set obj[0].ARGS=-c %~dp0include\glad\glad.c -o %~dp0bintmp\glad.o  
@REM AtulPwd - Gets the current directory of executable
set obj[1].Name=AtulPwd  
set obj[1].ARGS=-c %~dp0include\other\AtulPwd.cpp -o %~dp0bintmp\AtulPwd.o  
@REM fileio - Simple file io stuff
set obj[2].Name=fileio  
set obj[2].ARGS=-c %~dp0include\other\fileio.cpp -o %~dp0bintmp\fileio.o


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