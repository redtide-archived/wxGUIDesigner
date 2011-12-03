-----------------------------------------------------------------------------
--  Name:        plugin.lua
--  Purpose:     Plugin library project script.
--  Author:      Andrea Zanellato
--  Modified by:
--  Created:     2011/11/19
--  Copyright:   (c) Andrea Zanellato
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "LibPlugin"
    kind                "StaticLib"
    files
    {
        "../../sdk/**.h", "../../sdk/**.cpp",
        "../../output/plugins/**.xml"
    }
    includedirs         {"../../sdk/plugin"}
    libdirs             {"../../sdk/lib"}
    targetdir           "../../sdk/lib/"
    flags               {"ExtraWarnings"}
    links               {}
    targetname          ( CustomPrefix .. "plugin" )

configuration "not windows"
    buildoptions {"-fPIC"}

 -- Visual C++ 2005/2008
configuration "vs*"
    defines             {"_CRT_SECURE_NO_DEPRECATE"}

configuration "Debug"
    wx_config           { Debug="yes" }

configuration "Release"
    buildoptions        {"-fno-strict-aliasing"}
    wx_config           {}

