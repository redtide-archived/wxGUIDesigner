///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/mainframe.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <wx/aui/framemanager.h>
#include <wx/config.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/stdpaths.h>
#include <wx/xrc/xmlres.h>

#include "handlers/mainframe.h"
#include "manager.h"

FrameHandler::FrameHandler( wxFrame *owner, wxAuiManager *mgr )
{
    m_frame = owner;
    m_mgr   = mgr;

    LoadLayout();
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

void FrameHandler::LoadLayout()
{
    wxConfigBase *config = wxConfigBase::Get();

    wxString perspective;
    if ( m_mgr && config->Read( "/mainframe/perspective", &perspective ) )
    {
        m_mgr->LoadPerspective( perspective );
        m_mgr->Update();
    }

    bool maximized;
    config->Read( "/mainframe/maximized", &maximized, true );

    bool iconized;
    config->Read( "/mainframe/iconized", &iconized, false );

    if ( maximized )
    {
        m_frame->Maximize( maximized );
    }
    else if ( iconized )
    {
        m_frame->Iconize( iconized );
    }
    else
    {
        int x, y, w, h;
        x = y = w = h = -1;

        config->Read( "/mainframe/left",   &x );
        config->Read( "/mainframe/top",    &y );
        config->Read( "/mainframe/width",  &w );
        config->Read( "/mainframe/height", &h );

        if ( x > -1 && y > -1 && w >= 630 && h >= 480 )
        {
            m_frame->SetSize( x, y, w, h );
        }
    }
}

void FrameHandler::SaveLayout()
{
    bool iconized  = m_frame->IsIconized();
    bool maximized = m_frame->IsMaximized();

    wxConfigBase *config = wxConfigBase::Get();

    if ( !maximized )
    {
        config->Write( "/mainframe/left",   iconized ? -1 : m_frame->GetPosition().x );
        config->Write( "/mainframe/top",    iconized ? -1 : m_frame->GetPosition().y );
        config->Write( "/mainframe/width",  iconized ? -1 : m_frame->GetSize().GetWidth() );
        config->Write( "/mainframe/height", iconized ? -1 : m_frame->GetSize().GetHeight() );
    }

    config->Write( "/mainframe/maximized", maximized );
    config->Write( "/mainframe/iconized",  iconized );

    wxString perspective = m_mgr->SavePerspective();
    config->Write( "/mainframe/perspective", perspective );
}

bool FrameHandler::SaveWarning()
{
    int result = wxYES;

    // TODO: Check for modified project

    return ( result != wxCANCEL );
}
