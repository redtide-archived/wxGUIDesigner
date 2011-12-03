-----------------------------------------------------------------------------
--  Name:        wxcode.lua
--  Purpose:     wxCode controls plugin project build script.
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     2011/11/19
--  Copyright:   (c) Andrea Zanellato
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "PluginWxCode"
    kind                "SharedLib"
    targetname          "wxcode"
    files               {"../../../plugins/wxcode/wxcode.cpp"}
    includedirs
    {
        
    }
    defines             {"BUILD_DLL"}
    flags               {"ExtraWarnings"}
    links               {}

    configuration {"codelite", "not windows"}
        linkoptions     {"-Wl,-rpath,$$``ORIGIN"}

    configuration {"codeblocks", "not windows"}
        linkoptions     {"-Wl,-rpath,$``ORIGIN"}

    configuration "not windows"
        targetdir       "../../../output/lib/wxguidesigner"

    configuration "windows"
        targetprefix    "lib"
        targetdir       "../../../output/plugins/wxcode"

    configuration "Debug"
--      targetsuffix    ( DebugSuffix )
        wx_config       { Debug="yes" }

    configuration "Release"
        buildoptions    {"-fno-strict-aliasing"}
        wx_config       {}
