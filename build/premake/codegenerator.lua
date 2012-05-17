-----------------------------------------------------------------------------
--  Name:        codegenerator.lua
--  Purpose:     Code generator library project script.
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     2012/05/18
--  Revision:    $Hash$
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "LibCodeGen"
    kind                "SharedLib"
    files
    {
        "../../include/codegenerator/**.h", "../../src/codegenerator/**.cpp"
    }
    includedirs         {"../../include/codegenerator"}
    defines             {"MAKINGDLL_CODEGEN"}
    flags               {"ExtraWarnings"}
    targetname          ( CustomPrefix .. "codegenerator" )

    configuration "not windows"
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
    if wxCompiler == "gcc" then
        buildoptions    {"-fno-strict-aliasing"}
    end
        wx_config           {}
