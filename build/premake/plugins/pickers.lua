-----------------------------------------------------------------------------
--  Name:        pickers.lua
--  Purpose:     Picker controls plugin project build script.
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     2011/11/19
--  Copyright:   (c) Andrea Zanellato
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "PluginPickers"
    kind                "SharedLib"
    targetname          "pickers"
    files               {"../../../plugins/pickers/pickers.cpp"}
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
        targetdir       "../../../output/plugins/pickers"

    configuration "Debug"
--      targetsuffix    ( DebugSuffix )
        wx_config       { Debug="yes" }

    configuration "Release"
        buildoptions    {"-fno-strict-aliasing"}
        wx_config       {}
