-----------------------------------------------------------------------------
-- Name:        lua.lua
-- Purpose:     Lua library project script.
-- Author:      Andrea Zanellato
-- Modified by: 
-- Created:     2012/05/18
-- Revision:    $Hash$
-- Copyright:   (c) Andrea Zanellato
-- Licence:     GNU General Public License Version 2
-----------------------------------------------------------------------------
project "LuaLib"
    kind                "StaticLib"
    targetname          "lua"
    targetdir           "../../src/lua/lib"
    includedirs         {"../../src/lua/include"}
    files               {"../../src/lua/src/*.c", "../../src/lua/include/*.h*"}
    excludes            {"../../src/lua/src/lua.c", "../../src/lua/src/luac.c"}
    flags               {"ExtraWarnings"}

    configuration "Debug"
        targetsuffix    "d"

    configuration "Release"
