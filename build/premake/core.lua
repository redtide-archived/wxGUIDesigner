-----------------------------------------------------------------------------
--  Name:        core.lua
--  Purpose:     Core library project script.
--  Author:      Andrea Zanellato
--  Modified by:
--  Created:     2011/11/19
--  Copyright:   (c) Andrea Zanellato
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "LibCore"
    kind                "SharedLib"
    files
    {
        "../../include/core/**.h", "../../src/core/**.cpp",
        "../../build/premake/*.lua",
        "../../output/xrc/gui/*.xrc", "../../output/xrc/classes/*.xrc"
    }
    includedirs
    {
        "../../include/core", "../../include/core/handlers",
        "../../sdk/plugin"
    }
    libdirs             {"../../sdk/lib"}
    defines             {"MAKINGDLL_CORE"}
    flags               {"ExtraWarnings"}
    links               {"LibPlugin"}
    targetname          ( CustomPrefix .. "core" )

    configuration "not windows"
        targetdir       "../../output/lib/wxguidesigner"

    configuration "windows"
        targetprefix    "lib"
        targetdir       "../../output"

    configuration "vs*"
        defines         {"_CRT_SECURE_NO_DEPRECATE"}

    configuration "Debug"
        wx_config       { Libs="all", Debug="yes" }

    configuration "Release"
        buildoptions    {"-fno-strict-aliasing"}
        wx_config       { Libs="all" }
