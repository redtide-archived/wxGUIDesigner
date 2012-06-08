-----------------------------------------------------------------------------
--  Name:        solution.lua
--  Purpose:     Generic Premake 4 solution defining common configurations
--               for all projects it contains.
--  Author:      Andrea Zanellato
--  Modified by: 
--  Created:     19/10/2011
--  Revision:    $Hash$
--  Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
solution "wxGUIDesigner"
    language "C++"
    configurations      {"Debug", "Release"}

    scriptDir           = os.getcwd()

    dofile( scriptDir .. "/wxwidgets.lua" )

    local wxver         = string.gsub( wxVersion, '%.', '' )
    location            ( "../../build/" .. _ACTION )
    BuildDir            = solution().location
    CustomPrefix        = wxToolkit .. wxUnicodeSign

    os.chdir( BuildDir )

--if wxCompiler == "gcc" and os.is("windows") then
--  flags               {"NoImportLib"}
--end

if wxUseUnicode then
    flags               {"Unicode"}
    defines             {"UNICODE", "_UNICODE"}
end

    configuration "windows"
        defines         {"WIN32", "_WINDOWS"}

    configuration "Debug"
        defines         {"DEBUG", "_DEBUG"}
        flags           {"Symbols"}

    if wxCompiler == "gcc" then
        buildoptions    {"-O0"}
    end
    if wxVersion < "2.9" or os.is("windows") then
        targetsuffix    "d"
    end

    configuration "Release"
        defines         {"NDEBUG"}
        flags           {"OptimizeSpeed"}

    dofile( scriptDir .. "/projects.lua" )
