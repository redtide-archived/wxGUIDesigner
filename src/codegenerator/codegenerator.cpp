///////////////////////////////////////////////////////////////////////////////
// Name:        codegenerator/codegenerator.h
// Purpose:     Code generator library
// Author:      Jefferson González
// Modified by: 
// Created:     2012/05/17
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "codegenerator.h"
#include "luawrapper.h"

using namespace std;

CodeGenerator::CodeGenerator()
{
    Lua luaEngine("./php.lua");
    LuaFunctionArguments arguments;

    arguments.Add(std::string("MyFile"));

    cout << "Value of LANGUAGE_NAME: " << luaEngine.GetVar("LANGUAGE_NAME") << "\n\n";
    cout << "Call to SourceHeader(file): " << luaEngine.CallFunction("SourceHeader", arguments) << "\n";

    LuaFunctionArguments classStart;
    classStart.Add(std::string("MyFrame"));
    classStart.Add(std::string("wxFrame"));
    
    cout << "Call to ClassStart(class, parent): \n" << luaEngine.CallFunction("ClassStart", classStart) << "\n";

    cout << "Call to ClassEnd(class, parent): \n" << luaEngine.CallFunction("ClassEnd", classStart) << "\n";
}

CodeGenerator::~CodeGenerator()
{
    
}

CodeGenerator *CodeGenerator::ms_instance = NULL;

CodeGenerator *CodeGenerator::Get()
{
    if ( !ms_instance )
        ms_instance = new CodeGenerator;

    return ms_instance;
}

void CodeGenerator::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void CodeGenerator::AddLanguage()
{
    
}

void CodeGenerator::ScanLanguages()
{
    
}
