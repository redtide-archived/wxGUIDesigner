///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/frame.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/gui/frame.h"
#include "core/gui/manager.h"

#include <wx/aui/framemanager.h>
#include <wx/config.h>
#include <wx/dialog.h>
#include <wx/xrc/xmlres.h>

MainFrame::MainFrame( wxWindow *parent )
{
    wxXmlResource::Get()->LoadFrame( this, parent, "MainFrame" );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAbout,
                                this, XRCID("wxID_ABOUT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnNewProject,
                                this, XRCID("wxID_NEW") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnExit,
                                this, XRCID("wxID_EXIT") );

    Bind( wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this );
}

void MainFrame::OnAbout( wxCommandEvent &event )
{
    wxDialog *dlg = GUIManager::Get()->GetAboutDialog( this );
    dlg->ShowModal();
    dlg->Destroy();
}

void MainFrame::OnExit( wxCommandEvent & )
{
    Close();
}

void MainFrame::OnClose( wxCloseEvent &event )
{
    if ( !SaveWarning() ) return;

    SaveLayout();

    event.Skip();
}

void MainFrame::OnNewProject( wxCommandEvent & )
{
//  GUIManager::Get()->NewProject(); //TODO
}

void MainFrame::LoadLayout()
{
    wxConfigBase *config = wxConfigBase::Get();

    wxString perspective;
    if ( m_mgr && config->Read( "/mainframe/perspective", &perspective ) )
    {
        m_mgr->LoadPerspective( perspective );
        m_mgr->Update();
    }

    bool maximized, iconized;

    config->Read( "/mainframe/maximized", &maximized, true );
    config->Read( "/mainframe/iconized",  &iconized, false );

    if ( maximized )
    {
        Maximize( maximized );
    }
    else if ( iconized )
    {
        Iconize( iconized );
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
            SetSize( x, y, w, h );
        }
    }
}

void MainFrame::SaveLayout()
{
    bool iconized  = IsIconized();
    bool maximized = IsMaximized();

    wxConfigBase *config = wxConfigBase::Get();

    if ( !maximized )
    {
        config->Write( "/mainframe/left",   iconized ? -1 : GetPosition().x );
        config->Write( "/mainframe/top",    iconized ? -1 : GetPosition().y );
        config->Write( "/mainframe/width",  iconized ? -1 : GetSize().GetWidth() );
        config->Write( "/mainframe/height", iconized ? -1 : GetSize().GetHeight() );
    }

    config->Write( "/mainframe/maximized", maximized );
    config->Write( "/mainframe/iconized",  iconized );

    if ( m_mgr )
    {
        wxString perspective = m_mgr->SavePerspective();
        config->Write( "/mainframe/perspective", perspective );
    }
}

bool MainFrame::SaveWarning()
{
    int result = wxYES;

    // TODO: Check for modified project

    return ( result != wxCANCEL );
}
