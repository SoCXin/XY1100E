@ECHO OFF

SET CYGWIN_DIR=J:\NB\Embedded\cygwin\bin\

SET GCC_GNU_DIR=J:\NB\Embedded\gcc\bin\

SET PATH=%GCC_GNU_DIR%;%CYGWIN_DIR%;%PATH%

cmd /K cd %~dp0

