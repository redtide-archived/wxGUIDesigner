-----------------------------------------------------------------------------
-- Name:        wxwidgets.lua
-- Purpose:     wxWidgets configuration file for Premake4
-- Author:      laurent.humbertclaude@gmail.com, v.krishnakumar@gmail.com
-- Modified by: Andrea Zanellato zanellato.andrea@gmail.com
-- Revision:    $Hash$
-----------------------------------------------------------------------------
newoption  {
    trigger     = "wxcompiler",
    description = "Compiler to use",
    allowed = {
        { "gcc", "GNU Compiler" },
        { "vc",  "MS Visual C" }
    }
}
newoption  {
    trigger     = "wxmedia",
    description = "Whether to use wxMediaCtrl in wxMedia library",
    allowed = {
        { "yes", "Don't use wxMedia library" },
        { "no",  "Use wxMedia library" }
    }
}
newoption  {
    trigger     = "wxmonolithic",
    description = "Whether to use wxWidgets as monolithic DLL (MSW only)",
}
newoption  {
    trigger     = "wxroot",
    value       = "PATH",
    description = {
                    "Path to wxwidgets root folder, by default, WXWIN envvar",
                    "will be used or wx-config found in path on POSIX"
    }
}
newoption  {
    trigger     = "wxstatic",
    description = "Whether to use static or dynamic library build",
    allowed = {
        { "yes", "Static build" },
        { "no",  "DLL build" }
    }
}
newoption  {
    trigger     = "wxunicode",
    description = "Whether to use Unicode or ANSI build (wxWidgets 2.8 only)",
    allowed = {
        { "yes", "Unicode build" },
        { "no",  "ANSI build" }
    }
}
newoption  {
    trigger     = "wxversion",
    description = "wxWidgets version to use",
    allowed = {
        { "2.4", "wxWidgets 2.4" },
        { "2.5", "wxWidgets 2.5" },
        { "2.6", "wxWidgets 2.6" },
        { "2.7", "wxWidgets 2.7" },
        { "2.8", "wxWidgets 2.8" },
        { "2.9", "wxWidgets 2.9" }
    }
}
-----------------------------------------------------------------------------
-- Common globals initialization
-----------------------------------------------------------------------------
wxDebugSuffix   = "d"
wxPrefix        = "wx_"
wxUnicodeSign   = "u"

wxCompiler      = _OPTIONS.wxcompiler
wxMonolithic    = _OPTIONS.wxmonolithic
wxRoot          = _OPTIONS.wxroot
wxStatic        = _OPTIONS.wxstatic
wxUnicode       = _OPTIONS.wxunicode
wxUseMediaCtrl  = _OPTIONS.wxmedia
wxVersion       = _OPTIONS.wxversion
-----------------------------------------------------------------------------
-- FIXME the 'allowed' parameter seems to work only on wxversion
-- Version : one of '2.4', '2.5', '2.6', '2.7', '2.8', '2.9'.
-- Default to '2.9'
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-- wxCompiler : compiler used to compile windows libraries ( "vc" or "gcc" )
-----------------------------------------------------------------------------
if not os.is("windows") then
    wxCompiler = "gcc"
    wxMonolithic = "no"
elseif wxCompiler ~= "gcc" and wxCompiler ~= "vc" then
    error( wxCompiler .. ": invalid compiler.")
end
-----------------------------------------------------------------------------
-- wxMedia/wxUseMediaCtrl
-----------------------------------------------------------------------------
if wxUseMediaCtrl == "yes" then
    wxUseMediaCtrl = true
elseif wxUseMediaCtrl == "no" then
    wxUseMediaCtrl = false
else
    error( wxUseMediaCtrl .. ": wrong parameter, use only yes or no." )
end
-----------------------------------------------------------------------------
-- wxMonolithic
-----------------------------------------------------------------------------
if wxMonolithic ~= "yes" and wxMonolithic ~= "no" then
    error( wxMonolithic .. ": wrong parameter, use only yes or no." )
elseif wxMonolithic == "no" then
    wxMonolithic = false
else
    wxMonolithic = true
end
-----------------------------------------------------------------------------
-- wxRoot : Path to wxWidgets root folder.
--         Can be left empty if WXWIN is defined or if wx-config is accessible.
--         Useful in case more than a configuration is present in the system
--         e.g. wx 2.8 installed and a self built 2.9 one is used.
-----------------------------------------------------------------------------
if os.getenv('WXWIN') and not wxRoot then
    wxRoot = os.getenv('WXWIN')
elseif not os.isdir(wxRoot) then
    error(wxRoot..": not a valid directory.")
end
-----------------------------------------------------------------------------
-- wxStatic : indicates how wx is to be linked. Values are either
--             "yes" for static linking or "no" for shared linking.
--             Default to "no"
-----------------------------------------------------------------------------
if wxStatic ~= "yes" and wxStatic ~= "no" then
    error( wxStatic .. ": wrong parameter, use only yes or no." )
end
-----------------------------------------------------------------------------
-- wxUnicode : use "yes" for Unicode or "no" for ANSI version.
--             ANSI version only available up to 2.8
--             Default to "yes"
-----------------------------------------------------------------------------
if wxUnicode ~= "yes" and wxUnicode ~= "no" then
    error( wxUnicode .. ": wrong parameter, use only yes or no." )
elseif wxUnicode == "no" and wxVersion < "2.9" then
    wxUseUnicode    = false
    wxUnicodeSign   = ""
elseif not os.is("windows") and wxVersion > "2.8" then
    wxDebugSuffix   = ""
    wxUseUnicode    = true
else
    wxUseUnicode    = true
end

if os.is("windows") then
    wxPrefix  = "wx"
    wxToolkit = "msw"
elseif os.is("macosx") then
    wxToolkit = "mac"
else -- linux or bsd
    wxToolkit = "gtk2"
end
-----------------------------------------------------------------------------
-- The wx_config the parameters are.
--         Debug   : "yes" use debug version of wxwidgets. Default to "no"
--         Host    : 
--         Universal : use universal configuration. Default to "no"
--         Libs    : a list of wx libraries that you want to link with.
--                     eg: "aui,media,html"
--                     Default to ""; base is implicit
-----------------------------------------------------------------------------
function wx_config( options )

-- Parameter Checks
    local wrongParam        = false
    local allowedWxOptions  = {"Debug", "Host", "Universal", "Libs"}

    for option in pairs(options) do
        if not table.contains(allowedWxOptions, option) then
            print ("unrecognized option '"..option.. "'")
            wrongParam = true
        end
    end
    if wrongParam then
        print("valid options are : '" .. table.concat(allowedWxOptions, "', '").."'")
    end

    wx_config_Private(
                        options.Debug        or "",
                        options.Host         or "",
                        options.Universal    or "",
                        options.Libs         or ""
                     )
end

function wx_config_Private( wxDebug, wxHost, wxUniversal, wxLibs )

    if wxDebug == "yes" then
        defines {"__WXDEBUG__"}
    else
        flags   {"Optimize"}
    end

    if wxStatic == "yes" then
        flags   { "StaticRuntime" }
    else
        defines { "WXUSINGDLL" }
    end

    -- function to compensate lack of wx-config program on windows
    -- but wait, look at http://sites.google.com/site/wxconfig/ for one !
    function wx_config_for_windows(wxCompiler)
        -- buildtype is one of "", "u", "d" or "ud"
        local wxBuildType = wxUnicodeSign

        if wxDebug == "yes" then
            wxBuildType = wxBuildType .. wxDebugSuffix
        end

        local wxLibPath = path.join(wxRoot, "lib")
        wxLibPath = path.join(wxLibPath, wxCompiler .. "_" .. iif(wxStatic == 'yes', 'lib', 'dll'))
        -- common defines
        defines{ "__WXMSW__" }

        -- common include path
        includedirs {
            path.join(wxRoot, "include"),
            path.join(wxLibPath, "msw" .. wxBuildType)   -- something like "%WXWIN%\lib\vc_lib\mswud" to find "wx/setup.h"
            }

        -- common library path
        libdirs { wxLibPath }

        -- add the libs
        libVersion = string.gsub(wxVersion, '%.', '') -- remove dot from version
        if wxMonolithic then
            links { "wxmsw"..libVersion..wxBuildType }
        else
            links { "wxbase"..libVersion..wxBuildType } -- base lib
            if wxLibs ~= "" then
                for i, lib in ipairs(string.explode(wxLibs, ",")) do
                    local libPrefix = 'wxmsw'
                    if lib == "xml" or lib == "net" or lib == "odbc" then
                        libPrefix = 'wxbase'
                    end
                    links { libPrefix..libVersion..wxBuildType..'_'..lib}
                end
            end
            -- link with support libraries
            for i, lib in ipairs({"wxjpeg", "wxpng", "wxzlib", "wxtiff", "wxexpat"}) do
                links { lib..wxDebugSuffix }
            end
            links { "wxregex" .. wxBuildType }
        end
    end

    -- use wx-config to figure out build parameters
    function wx_config_for_posix()
        local configCmd = "wx-config" -- this is the wx-config command line
        if wxRoot ~= "" then configCmd = path.join(wxRoot, "wx-config") end

        local function checkYesNo(value, option)
            if value == "" then return "" end
            if value == "yes" or value == "no" then return " --"..option.."="..value end
            error("wx"..option..' can only be "yes", "no" or empty' )
        end

        configCmd = configCmd .. checkYesNo(wxDebug,     "debug")
        configCmd = configCmd .. checkYesNo(wxStatic,    "static")
        configCmd = configCmd .. checkYesNo(wxUnicode,   "unicode")
        configCmd = configCmd .. checkYesNo(wxUniversal, "universal")

        if wxHost ~= "" then configCmd = configCmd .. " --host=" .. wxHost end
--     if wxVersion ~= "" then configCmd = configCmd .. " --version=" .. wxVersion end

        -- set the parameters to the current configuration
        buildoptions {"`" .. configCmd .." --cxxflags`"}
        linkoptions  {"`" .. configCmd .." --libs " .. wxLibs .. "`"}
    end

    if not os.is("windows") then
        wx_config_for_posix()
    else
        wx_config_for_windows( wxCompiler )
    end
end
