///////////////////////////////////////////////////////////////////////////////
// Name:        core/manager.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/01/13
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/manager.h"
#include "core/gui/manager.h"
#include "core/object/database.h"

wxGUIDesigner::wxGUIDesigner()
{
    
    ClassInfoDataBase::Get();
}

wxGUIDesigner::~wxGUIDesigner()
{
    GUIManager::Get()->Free();
    ClassInfoDataBase::Get()->Free();
}

wxGUIDesigner *wxGUIDesigner::ms_instance = NULL;

wxGUIDesigner *wxGUIDesigner::Get()
{
    if ( !ms_instance )
        ms_instance = new wxGUIDesigner;

    return ms_instance;
}

void wxGUIDesigner::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void wxGUIDesigner::NewProject()
{
    
}

wxFrame *wxGUIDesigner::GetMainFrame( wxWindow *parent )
{
    return GUIManager::Get()->GetMainFrame( parent );
}
