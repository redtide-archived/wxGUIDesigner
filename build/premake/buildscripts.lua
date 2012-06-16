-------------------------------------------------------------------------------
-- Name:        buildscripts.lua
-- Purpose:     Build scripts project
-- Author:      Andrea Zanellato
-- Modified by: 
-- Created:     18/12/2011
-- Revision:    $Hash$
-- Copyleft:    (ɔ) Andrea Zanellato
-- Licence:     GNU General Public License Version 3
-------------------------------------------------------------------------------
project "BuildScripts"
    kind            "WindowedApp"
    files           {"../../build/premake/*.lua"}

configuration "not windows"
    files           {"../../*.sh"}

configuration "windows"
    files           {"../../*.bat"}
