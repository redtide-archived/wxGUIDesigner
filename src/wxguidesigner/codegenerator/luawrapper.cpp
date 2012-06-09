///////////////////////////////////////////////////////////////////////////////
// Name:        codegenerator/codegenerator.cpp
// Purpose:     Lua wrapper for code generator library
// Author:      Jefferson González
// Modified by: 
// Created:     2012/05/17
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "codegenerator/luawrapper.h"

/* Implementation for LuaFunctionArguments */

LuaFunctionArguments::~LuaFunctionArguments()
{
    for(size_t i=0; i<m_arguments.size(); i++)
    {
        if(m_arguments[i].type == STRING && m_arguments[i].value.SVAL != NULL)
        {
            //Causes a segfault dont know why :S
            //delete[] m_arguments[i].value.SVAL;
            //m_arguments[i].value.SVAL = NULL;

        }
    }
}

void LuaFunctionArguments::Add(bool value)
{
    LuaArgument argument;
    argument.type = BOOL;
    argument.value.BVAL = value;

    m_arguments.push_back(argument);
}

void LuaFunctionArguments::Add(size_t value)
{
    LuaArgument argument;
    argument.type = INTEGER;
    argument.value.IVAL = value;

    m_arguments.push_back(argument);
}

void LuaFunctionArguments::Add(const std::string& value)
{
    LuaArgument argument;
    argument.type = STRING;

    char* value_copy = new char[value.size()];

    for(size_t c=0; c<=value.size(); c++)
    {
        value_copy[c] = value[c];
    }

    argument.value.SVAL = value_copy;

    m_arguments.push_back(argument);
}

void LuaFunctionArguments::Add(double value)
{
    LuaArgument argument;
    argument.type = DOUBLE;
    argument.value.DVAL = value;

    m_arguments.push_back(argument);
}

LuaArgument LuaFunctionArguments::operator[](size_t index)
{
    if(index > m_arguments.size())
        return LuaArgument();

    return m_arguments[index];
}

size_t LuaFunctionArguments::Count()
{
    return m_arguments.size();
}


/* Implementation for Lua Wrapper */

Lua::Lua(const std::string& file)
{
    m_isLoaded = false;

    Load(file);
}

Lua::~Lua()
{
    Unload();
}

void Lua::Load(const std::string& file)
{
    if(m_isLoaded)
        Unload();

    m_lua = luaL_newstate();

    if(m_lua == NULL)
    {
        throw LuaException("Not enough resources to initialize the Lua engine.");
        return;
    }

    m_isLoaded = true;
    luaL_openlibs(m_lua);

    if (luaL_loadfile(m_lua, file.c_str()) || lua_pcall(m_lua, 0, 0, 0)) {
        std::string errorMessage = "The file '";
        errorMessage += file;
        errorMessage += "' was not found or could not be loaded.";

        throw LuaException(errorMessage.c_str());
    }
}

void Lua::Unload()
{
    if(m_isLoaded)
    {
        lua_pop(m_lua, 1);
        lua_close(m_lua);
        m_isLoaded = false;
    }
}

std::string Lua::CallFunction(std::string functionName, LuaFunctionArguments arguments)
{
    if(!m_isLoaded)
        throw LuaException("Lua engine not loaded, use Load() first.");

    lua_getglobal(m_lua, functionName.c_str());

    //If function not found return empty string
    if(!lua_isfunction(m_lua, -1))
    {
        return "";
    }

    //Pass all arguments to function
    for(size_t argc=0; argc<arguments.Count(); argc++)
    {
        switch(arguments[argc].type)
        {
            case BOOL:
                lua_pushboolean(m_lua, arguments[argc].value.BVAL);
                break;

            case INTEGER:
                lua_pushinteger(m_lua, arguments[argc].value.IVAL);
                break;

            case STRING:
                lua_pushstring(m_lua, arguments[argc].value.SVAL);
                break;

            case DOUBLE:
                lua_pushnumber(m_lua, arguments[argc].value.DVAL);
                break;
        }
    }

    /* do the call (arguments count, 1 result) */
    if (lua_pcall(m_lua, arguments.Count(), 1, 0) != 0) {

        std::string errorMessage = "Error on function: ";
        errorMessage += functionName;
        errorMessage += "()";

        throw LuaException(errorMessage);
    }

    return lua_tostring(m_lua, -1);
}

std::string Lua::GetVar(const std::string& name)
{
    lua_getglobal(m_lua, name.c_str());

    if(!lua_isstring(m_lua, -1))
    {
        return "";
    }

    return lua_tostring(m_lua, -1);
}
