REM ############################################################################
REM ##  Name:        premake_gen.bat
REM ##  Purpose:     Premake 4 build script for Windows.
REM ##  Author:      Andrea Zanellato
REM ##  Modified by: 
REM ##  Created:     2011/10/22
REM ##  Copyright:   (c) Andrea Zanellato
REM ##  Licence:     wxWidgets licence
REM ############################################################################
@ECHO OFF

REM Clear output
CLS

REM Set Defaults
SET wxcompiler=gcc
SET wxmedia=yes
SET wxmonolithic=no
SET wxroot=%WXWIN%
SET wxstatic=no
SET wxunicode=yes
SET wxversion=2.9

REM Handle parameters
:Loop
REM Show help and exit
IF [%1]==[-h]               GOTO Help
IF [%1]==[--help]           GOTO Help
IF [%1]==[--wxcompiler]     GOTO Compiler
IF [%1]==[--wxmedia]        GOTO Media
IF [%1]==[--wxmonolithic]   GOTO Monolithic
IF [%1]==[--wxstatic]       GOTO Static
IF [%1]==[--wxroot]         GOTO Root
IF [%1]==[--wxunicode]      GOTO Unicode
IF [%1]==[--wxversion]      GOTO Version
GOTO Premake

:Help
ECHO.
ECHO Available options:
ECHO.
ECHO --wxcompiler   Specify the compiler to use.
ECHO                Example: --wxcompiler=vc if you use MSVC.
ECHO                Current: %wxcompiler%
ECHO.
ECHO --wxmedia      Disable wxMedia library (wxMediaCtrl).
ECHO                Example: --wxmedia=no to disable it.
ECHO                Default: %wxmedia%
ECHO.
ECHO --wxunicode    Whether to use an Unicode or an ANSI build.
ECHO                Ignored in wxWidgets 2.9 and later.
ECHO                Example: --wxunicode=no produces an ANSI build.
ECHO                Default: %wxunicode%
ECHO.
ECHO --wxroot       Specify the wxWidgets build path,
ECHO                useful for wxWidgets builds not installed
ECHO                in your system (alternate/custom builds)
ECHO                Example: --wxroot=D:\Devel\wxWidgets\3.0
ECHO                Current: %WXWIN%
ECHO.
ECHO --wxversion    Specify the wxWidgets version.
ECHO                Example: --wxversion=3.0
ECHO                Default: %wxversion%
ECHO.
GOTO End

:Compiler
SET wxcompiler=%2
SHIFT
SHIFT
GOTO Loop

:Media
SET wxmedia=%2
SHIFT
SHIFT
GOTO Loop

:Monolithic
SET wxmonolithic=%2
SHIFT
SHIFT
GOTO Loop

:Root
SET wxroot=%2
SHIFT
SHIFT
GOTO Loop

:Static
SET wxstatic=%2
SHIFT
SHIFT
GOTO Loop

:Unicode
SET wxunicode=%2
SHIFT
SHIFT
GOTO Loop

:Version
SET wxversion=%2
SHIFT
SHIFT
GOTO Loop

:Premake
build\premake\windows\premake4.exe --file=build/premake/solution.lua --wxcompiler=%wxcompiler% --wxmedia=%wxmedia% --wxmonolithic=%wxmonolithic% --wxroot=%wxroot% --wxstatic=%wxstatic% --wxunicode=%wxunicode% --wxversion=%wxversion% codelite
build\premake\windows\premake4.exe --file=build/premake/solution.lua --wxcompiler=%wxcompiler% --wxmedia=%wxmedia% --wxmonolithic=%wxmonolithic% --wxroot=%wxroot% --wxstatic=%wxstatic% --wxunicode=%wxunicode% --wxversion=%wxversion% codeblocks
ECHO.

REM build\premake\windows\premake4.exe --file=build/premake/solution.lua --wxcompiler=%wxcompiler% --wxmedia=%wxmedia% --wxmonolithic=%wxmonolithic% --wxroot=%wxroot% --wxstatic=%wxstatic% --wxunicode=%wxunicode% --wxversion=%wxversion% vs2008
ECHO.

:End

