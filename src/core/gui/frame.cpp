///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/frame.cpp
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
#include <wx/filedlg.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>

#include "core/manager.h"
#include "core/gui/frame.h"
#include "core/gui/manager.h"
#include "core/xrc/object.h"

MainFrame::MainFrame( wxWindow *parent )
{
    wxXmlResource::Get()->LoadFrame( this, parent, "MainFrame" );

    wxMenu *menuFile = GetMenuBar()->GetMenu(0);
    if( menuFile )
        m_history.UseMenu( menuFile );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAbout,
                                this, XRCID("wxID_ABOUT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnExit,
                                this, XRCID("wxID_EXIT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnNewProject,
                                this, XRCID("wxID_NEW") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnOpenProject,
                                this, XRCID("wxID_OPEN") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnSaveProject,
                                this, XRCID("wxID_SAVE") );

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
    if ( !SaveWarning() )
        return;

    SaveLayout();

    event.Skip();
}

void MainFrame::OnOpenProject( wxCommandEvent & )
{
    if ( !SaveWarning() )
        return;

    wxString wildCard = _("XRC Files");
    wildCard.append(" (*.xrc)|*.xrc|");
    wildCard.append(_("All files") );
    wildCard.append(" (*.*)|*.*");

    wxFileDialog dlg( this, _("Open Project"), m_lastDir, wxEmptyString,
                        wildCard, wxFD_OPEN );

    if( dlg.ShowModal() == wxID_OK )
    {
        m_lastDir = dlg.GetDirectory();
        wxString filePath = dlg.GetPath();

        if( wxGUIDesigner::Get()->LoadProject( filePath ) )
            m_history.AddFileToHistory( filePath );
    };
}

void MainFrame::OnSaveProject( wxCommandEvent & )
{
    wxFileDialog dlg( this, _("Save Project"), m_lastDir, wxEmptyString,
                      _("XRC Files (*.xrc)|*.xrc"), wxFD_SAVE );

    if ( dlg.ShowModal() == wxID_OK )
    {
        m_lastDir = dlg.GetDirectory();

        wxString    filePath    = dlg.GetPath();
        wxFileName  fileName    = filePath;

        if ( !fileName.HasExt() )
        {
            fileName.SetExt("xrc");
            filePath = fileName.GetFullPath();
        }

        if ( fileName.FileExists() == true )
        {
            wxMessageDialog md( this,
            _("A file with same name already exists. Do you want to overwrite it?"),
            _("Overwrite"), wxYES_NO | wxICON_INFORMATION | wxNO_DEFAULT );

            if( md.ShowModal() == wxID_NO )
                return;
        }

        wxGUIDesigner::Get()->SaveProject( filePath );
    }
}

void MainFrame::OnNewProject( wxCommandEvent & )
{
    wxGUIDesigner::Get()->NewProject();
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

    config->Read( "last_dir", &m_lastDir );

    m_history.Load( *config );
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

    config->Write( "last_dir", m_lastDir );

    m_history.Save( *config );
}

bool MainFrame::SaveWarning()
{
    int result = wxYES;

    // TODO: Check for modified project

    return ( result != wxCANCEL );
}
