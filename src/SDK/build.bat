@ECHO OFF

SET CYGWIN_DIR=D:\Embedded\cygwin\bin\

SET GCC_GNU_DIR=D:\Embedded\gcc\bin\

SET PATH=%GCC_GNU_DIR%;%CYGWIN_DIR%;%PATH%

cmd /K cd %~dp0

