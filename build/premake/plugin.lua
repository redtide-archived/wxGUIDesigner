-----------------------------------------------------------------------------
--  Name:        plugin.lua
--  Purpose:     Plugin library project script.
--  Author:      Andrea Zanellato
--  Modified by:
--  Created:     2011/11/19
--  Revision:    $Hash$
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "LibPlugin"
    kind                "SharedLib"
    files
    {
        "../../include/plugin/**.h", "../../src/plugin/**.cpp",
        "../../output/db/**.xml"
    }
    includedirs         {"../../include/plugin"}
    defines             {"MAKINGDLL_PLUGIN"}
    flags               {"ExtraWarnings"}
    targetname          ( CustomPrefix .. "plugin" )

configuration "not windows"
--  buildoptions {"-fPIC"}
    libdirs             {"../../output/lib/wxguidesigner"}
    targetdir           "../../output/lib/wxguidesigner"

configuration "windows"
    libdirs             {"../../output"}
    targetdir           "../../output"
    targetprefix        "lib"

 -- Visual C++ 2005/2008
configuration "vs*"
    defines             {"_CRT_SECURE_NO_DEPRECATE"}

configuration "Debug"
    wx_config           { Debug="yes" }

configuration "Release"
    buildoptions        {"-fno-strict-aliasing"}
    wx_config           {}

