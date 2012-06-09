-----------------------------------------------------------------------------
-- Name:        core.lua
-- Purpose:     Core library project script.
-- Author:      Andrea Zanellato
-- Modified by: 
-- Created:     2011/11/19
-- Revision:    $Hash$
-- Copyright:   (c) Andrea Zanellato
-- Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "LibCore"
    kind        "SharedLib"
    files
    {
        "../../include/wxguidesigner/**.h", "../../src/wxguidesigner/**.cpp",
        "../../include/wx/**.h", "../../src/wx/**.cpp",
        "../../output/db/controls.xml", "../../output/db/controls/**.xml"
    }
    includedirs
    {
        "../../include"
    }
    excludes
    {
        "../../src/wxguidesigner/codegenerator/**.*",
        "../../include/wxguidesigner/codegenerator/**.*"
    }
    defines             {"MAKINGDLL_WXGUIDESIGNER"}
    flags               {"ExtraWarnings"}
-- links               {"LibCodeGen"}
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
        wx_config       { Libs="xrc,stc,propgrid,aui,html,adv,core,net,xml", Debug="yes" }

    configuration "Release"
    if wxCompiler == "gcc" then
        buildoptions    {"-fno-strict-aliasing"}
    end
        wx_config       { Libs="xrc,stc,propgrid,aui,html,adv,core,net,xml" }
