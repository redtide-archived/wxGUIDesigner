-----------------------------------------------------------------------------
--  Name:        common.lua
--  Purpose:     Common controls plugin project build script.
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     2011/11/19
--  Copyright:   (c) Andrea Zanellato
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "PluginCommon"
    kind                "SharedLib"
    targetname          "common"
    files               {"../../../plugins/common/common.cpp"}
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
        targetdir       "../../../output/plugins/common"

    configuration "Debug"
--      targetsuffix    ( DebugSuffix )
        wx_config       { Debug="yes" }

    configuration "Release"
        buildoptions    {"-fno-strict-aliasing"}
        wx_config       {}
