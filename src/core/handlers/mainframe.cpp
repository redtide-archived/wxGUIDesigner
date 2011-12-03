///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/mainframe.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// RCS-ID:      $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <wx/aui/framemanager.h>
#include <wx/dialog.h>
#include <wx/stdpaths.h>
#include <wx/xrc/xmlres.h>

#include "handlers/mainframe.h"
#include "manager.h"

FrameHandler::FrameHandler( wxFrame *owner )
{
    m_frame = owner;
}

void FrameHandler::OnAbout( wxCommandEvent &event )
{
    wxDialog *dlg = wxGUIDesigner::Get()->GetAboutDialog( m_frame );
    if ( dlg )
    {
        dlg->ShowModal();
        dlg->Destroy();
    }
}

void FrameHandler::OnExit( wxCommandEvent & )
{
    m_frame->Close();
}

void FrameHandler::OnClose( wxCloseEvent &event )
{
    if ( !SaveWarning() ) return;

    SaveLayout();

    event.Skip();
}

void FrameHandler::OnNewProject( wxCommandEvent & )
{
//  wxGUIDesigner::Get()->NewProject(); //TODO
}

void FrameHandler::SaveLayout()
{
//  bool isIconized  = m_frame->IsIconized();
//  bool isMaximized = m_frame->IsMaximized();
}

bool FrameHandler::SaveWarning()
{
    int result = wxYES;

    // TODO: Check for modified project

    return ( result != wxCANCEL );
}
