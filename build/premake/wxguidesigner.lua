-----------------------------------------------------------------------------
--  Name:        wxguidesigner.lua
--  Purpose:     Main application project
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     19/10/2011
--  Copyright:   (c) Andrea Zanellato
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "wxGUIDesigner"
    kind                    "WindowedApp"
    files
    {
        "../../src/main.h", "../../src/main.cpp"
    }
    includedirs
    {
        "../../include/core", "../../sdk/plugin"
    }
    defines                 {"NO_GCC_PRAGMA"}
    flags                   {"ExtraWarnings"}
    libdirs                 {"../../sdk/lib"}
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
        buildoptions        {"-gstabs"}
        links               {"bfd", "intl", "iberty", "psapi", "imagehlp"}
end
    configuration "Debug"
        wx_config           { Debug="yes" }

    configuration "Release"
        buildoptions        {"-fno-strict-aliasing"}
        wx_config           {}

    if os.is("windows") then
        flags               {"Symbols"}
    end

