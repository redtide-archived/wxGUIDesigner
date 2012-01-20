-----------------------------------------------------------------------------
--  Name:        core.lua
--  Purpose:     Core library project script.
--  Author:      Andrea Zanellato
--  Modified by:
--  Created:     2011/11/19
--  Revision:    $Hash$
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "LibCore"
    kind                "SharedLib"
    files
    {
        "../../include/core/**.h", "../../src/core/**.cpp",
        "../../include/wx/**.h", "../../src/wx/**.cpp",
        "../../output/xrc/*.xrc", "../../output/xrc/classes/*.xrc",
        "../../output/db/**.xml"
    }
    includedirs
    {
        "../../include"
    }
    defines             {"MAKINGDLL_CORE"}
    flags               {"ExtraWarnings"}
--  links               {"LibPlugin"}
    targetname          ( CustomPrefix .. "core" )

    configuration "not windows"
        libdirs         {"../../output/lib/wxguidesigner"}
        targetdir       "../../output/lib/wxguidesigner"

    configuration "windows"
        libdirs         {"../../output"}
        targetdir       "../../output"
        targetprefix    "lib"

    configuration "vs*"
        defines         {"_CRT_SECURE_NO_DEPRECATE"}

    configuration "Debug"
        wx_config       { Libs="all", Debug="yes" }

    configuration "Release"
        buildoptions    {"-fno-strict-aliasing"}
        wx_config       { Libs="all" }
