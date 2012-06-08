-----------------------------------------------------------------------------
--  Name:        wxguidesigner.lua
--  Purpose:     Main application project
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     19/10/2011
--  Revision:    $Hash$
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "wxGUIDesigner"
    kind                    "WindowedApp"
    files                   {"../../src/main.*"}
    includedirs             { "../../include"}
    defines                 {"NO_GCC_PRAGMA"}
    flags                   {"ExtraWarnings"}
    libdirs                 {"../../output/lib/wxguidesigner"}
--  links                   {"LibCodeGen", "LibCore"}
    links                   {"LibCore"}

    configuration {"codelite", "not windows"}
        linkoptions         {"-Wl,-rpath,$$``ORIGIN/../lib/wxguidesigner"}

    configuration {"codeblocks", "not windows"}
        linkoptions         {"-Wl,-rpath,$``ORIGIN/../lib/wxguidesigner"}

    configuration "macosx"
        linkoptions         {"-L../../output/lib/wxguidesigner"}

    configuration {"macosx", "Debug"}
        postbuildcommands   {"../macosx/postbuildd.sh"}

    configuration {"macosx", "Release"}
        postbuildcommands   {"../macosx/postbuild.sh"}

    configuration "not windows"
        excludes            {"../../src/*.rc"}
        libdirs             {"../../output/lib/wxguidesigner"}
        targetdir           "../../output/bin"
        targetname          "wxguidesigner"

    configuration "windows"
        files               {"../../src/*.rc"}
        libdirs             {"../../output"}
        targetdir           "../../output"

    if wxCompiler == "gcc" then
        buildoptions    {"-gstabs"}
        links           {"bfd", "intl", "iberty", "psapi", "imagehlp"}
    end

    configuration "Debug"
        wx_config           { Libs="xrc,stc,propgrid,aui,html,adv,core,xml", Debug="yes" }

    configuration "Release"

    if wxCompiler == "gcc" then
        buildoptions    {"-fno-strict-aliasing"}
    end
        wx_config           { Libs="xrc,stc,propgrid,aui,html,adv,core,xml" }

    if os.is("windows") then
        flags               {"Symbols"}
    end

