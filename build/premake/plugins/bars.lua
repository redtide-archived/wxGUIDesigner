-----------------------------------------------------------------------------
--  Name:        bars.lua
--  Purpose:     Bar controls plugin project build script.
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     2011/11/19
--  Copyright:   (c) Andrea Zanellato
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "PluginBars"
    kind                "SharedLib"
    targetname          "bars"
    files               {"../../../plugins/bars/bars.cpp"}
    includedirs
    {
        
    }
    defines             {"BUILD_DLL"}
    flags               {"ExtraWarnings"}
    links               {}

    configuration "not windows"
        targetdir       "../../../output/lib/wxguidesigner"

    configuration "windows"
        targetprefix    "lib"
        targetdir       "../../../output/plugins/bars"

    configuration "Debug"
--      targetsuffix    ( DebugSuffix )
        wx_config       { Debug="yes" }

    configuration "Release"
        buildoptions    {"-fno-strict-aliasing"}
        wx_config       {}
