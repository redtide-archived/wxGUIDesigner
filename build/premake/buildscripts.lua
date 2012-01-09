-------------------------------------------------------------------------------
--  Name:        buildscripts.lua
--  Purpose:     Build scripts project
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     18/12/2011
--  Revision:    $Hash$
--  Licence:     GNU General Public License Version 2
-------------------------------------------------------------------------------
project "BuildScripts"
    kind            "WindowedApp"
    files           {"../../build/premake/*.lua"}

configuration "not windows"
    files           {"../../*.sh"}

configuration "windows"
    files           {"../../*.bat"}
