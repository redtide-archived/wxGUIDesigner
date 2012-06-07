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

#include <wx/msgdlg.h>

using namespace std;

CodeGenerator::CodeGenerator()
{
    Lua luaEngine("./php.lua");
    LuaFunctionArguments arguments;

    arguments.Add(std::string("MyFile"));

    wxString str = "Value of LANGUAGE_NAME: " + luaEngine.GetVar("LANGUAGE_NAME") + "\n\n";
    str += "Call to SourceHeader(file): " + luaEngine.CallFunction("SourceHeader", arguments) + "\n";

    LuaFunctionArguments classStart;
    classStart.Add(std::string("MyFrame"));
    classStart.Add(std::string("wxFrame"));
    
    str += "Call to ClassStart(class, parent): \n" + luaEngine.CallFunction("ClassStart", classStart) + "\n";

    str += "Call to ClassEnd(class, parent): \n" + luaEngine.CallFunction("ClassEnd", classStart) + "\n";

    wxMessageBox( str, "test" );
}

CodeGenerator::~CodeGenerator()
{
    
}

CodeGenerator *CodeGenerator::ms_instance = NULL;

CodeGenerator *CodeGenerator::Get()
{
    if( !ms_instance )
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
