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

// ----------------------------------------------------------------------------
// MainFrame
// ----------------------------------------------------------------------------
MainFrame::MainFrame( wxWindow *parent )
{
    wxXmlResource::Get()->LoadFrame( this, parent, "MainFrame" );

    wxMenu *menuFile = GetMenuBar()->GetMenu(0);
    if( menuFile )
        m_history.UseMenu( menuFile );

    Bind( wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAbout,
                                this, XRCID("wxID_ABOUT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnExit,
                                this, XRCID("wxID_EXIT") );
// ----------------------------------------------------------------------------
// Project
// ----------------------------------------------------------------------------
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnNewProject,
                                this, XRCID("wxID_NEW") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnOpenProject,
                                this, XRCID("wxID_OPEN") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnOpenRecent,
                                this, XRCID("wxID_FILE1"), XRCID("wxID_FILE9") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnSaveAsProject,
                                this, XRCID("wxID_SAVEAS") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnSaveProject,
                                this, XRCID("wxID_SAVE") );
// ----------------------------------------------------------------------------
// Undo/Redo
// ----------------------------------------------------------------------------
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnUndo,
                                this, XRCID("wxID_UNDO") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnRedo,
                                this, XRCID("wxID_REDO") );
// ----------------------------------------------------------------------------
// Clipboard
// ----------------------------------------------------------------------------
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnCut,
                                this, XRCID("wxID_CUT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnCopy,
                                this, XRCID("wxID_COPY") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnPaste,
                                this, XRCID("wxID_PASTE") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnDelete,
                                this, XRCID("wxID_DELETE") );
// ----------------------------------------------------------------------------
// CodeGenerator
// ----------------------------------------------------------------------------
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnGenerateCode,
                                this, XRCID("wxID_EXECUTE") );
// ----------------------------------------------------------------------------
// Widget's alignment
// ----------------------------------------------------------------------------
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAlignLeft,
                                this, XRCID("ID_ALIGN_LEFT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAlignCenterH,
                                this, XRCID("ID_ALIGN_CENTER_H") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAlignRight,
                                this, XRCID("ID_ALIGN_RIGHT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAlignTop,
                                this, XRCID("ID_ALIGN_TOP") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAlignCenterV,
                                this, XRCID("ID_ALIGN_CENTER_V") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnAlignBottom,
                                this, XRCID("ID_ALIGN_BOTTOM") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnExpand,
                                this, XRCID("ID_EXPAND") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnStretch,
                                this, XRCID("ID_STRETCH") );
// ----------------------------------------------------------------------------
// Widget's borders
// ----------------------------------------------------------------------------
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnBorderLeft,
                                this, XRCID("ID_BORDER_LEFT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnBorderRight,
                                this, XRCID("ID_BORDER_RIGHT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnBorderTop,
                                this, XRCID("ID_BORDER_TOP") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnBorderBottom,
                                this, XRCID("ID_BORDER_BOTTOM") );
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

    if( wxGUIDesigner::Get()->IsProjectModified() )
    {
        result = wxMessageBox(_("Current project file has been modified.\n" ) +
                                _("Do you want to save the changes?"),
                                _("Save project"), wxYES | wxNO | wxCANCEL, this );
        if ( result == wxYES )
        {
            wxCommandEvent event;
            OnSaveProject( event );
        }
    }

    return ( result != wxCANCEL );
}

void MainFrame::OnAbout( wxCommandEvent & )
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
// ----------------------------------------------------------------------------
// Project
// ----------------------------------------------------------------------------
void MainFrame::OnNewProject( wxCommandEvent & )
{
    if ( !SaveWarning() )
        return;

    wxGUIDesigner::Get()->NewProject();
}

void MainFrame::OnOpenProject( wxCommandEvent & )
{
    if( !SaveWarning() )
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

void MainFrame::OnOpenRecent( wxCommandEvent &event )
{
    if( !SaveWarning() )
        return;

    wxMenuItem *menuItm = NULL;
    wxMenuBar  *menuBar = GetMenuBar();

    if( menuBar )
        menuItm = menuBar->FindItem( event.GetId() );

    if( !menuItm )
        return;
    
    wxString    filePath( menuItm->GetItemLabelText().AfterLast(' ') );
    wxFileName  fileName( filePath );

    if( fileName.FileExists() )
    {
        if ( wxGUIDesigner::Get()->LoadProject( fileName.GetFullPath() ) )
            m_lastDir = fileName.GetPath();
    }
    else
    {
        wxString message =
            _("The project file") + " " + fileName.GetName() + " " +
            _("doesn't exist.") +
            _("Would you like to remove it from the recent files list?");

        int answer =
            wxMessageBox( message, _("Open recent project"),
                                wxICON_WARNING | wxYES_NO );
        if( answer == wxYES )
        {
            for( size_t i = 0; i < m_history.GetCount(); i++ )
            {
                if( filePath == m_history.GetHistoryFile(i) )
                {
                    m_history.RemoveFileFromHistory(i);
                    break;
                }
            }
        }
    }
}

void MainFrame::OnSaveAsProject( wxCommandEvent & )
{
    wxFileDialog dlg( this, _("Save Project"), m_lastDir, wxEmptyString,
                      _("XRC Files (*.xrc)|*.xrc"), wxFD_SAVE );

    if ( dlg.ShowModal() == wxID_OK )
    {
        m_lastDir = dlg.GetDirectory();

        wxString    filePath    = dlg.GetPath();
        wxFileName  fileName    = filePath;

        if( !fileName.HasExt() )
        {
            fileName.SetExt("xrc");
            filePath = fileName.GetFullPath();
        }

        if( fileName.FileExists() == true )
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

void MainFrame::OnSaveProject( wxCommandEvent &event )
{
    wxString currPrj = wxGUIDesigner::Get()->GetProjectFileName();

    if( currPrj.empty() )
    {
        OnSaveAsProject( event );
    }
    else
    {
        wxGUIDesigner::Get()->SaveProject( currPrj );
        m_history.AddFileToHistory( currPrj );
    }
}
// ----------------------------------------------------------------------------
// Undo/Redo
// ----------------------------------------------------------------------------
void MainFrame::OnUndo( wxCommandEvent &event )
{
    
}

void MainFrame::OnRedo( wxCommandEvent &event )
{
    
}
// ----------------------------------------------------------------------------
// Clipboard
// ----------------------------------------------------------------------------
void MainFrame::OnCut( wxCommandEvent &event )
{
    
}

void MainFrame::OnCopy( wxCommandEvent &event )
{
    
}

void MainFrame::OnPaste( wxCommandEvent &event )
{
    
}

void MainFrame::OnDelete( wxCommandEvent &event )
{
    
}
// ----------------------------------------------------------------------------
// CodeGenerator
// ----------------------------------------------------------------------------
void MainFrame::OnGenerateCode( wxCommandEvent & )
{
    wxGUIDesigner::Get()->GenerateCode();
}
// ----------------------------------------------------------------------------
// Widget's alignment
// ----------------------------------------------------------------------------
void MainFrame::OnAlignLeft( wxCommandEvent &event )
{
    
}

void MainFrame::OnAlignCenterH( wxCommandEvent &event )
{
    
}

void MainFrame::OnAlignRight( wxCommandEvent &event )
{
    
}

void MainFrame::OnAlignTop( wxCommandEvent &event )
{
    
}

void MainFrame::OnAlignCenterV( wxCommandEvent &event )
{
    
}

void MainFrame::OnAlignBottom( wxCommandEvent &event )
{
    
}

void MainFrame::OnExpand( wxCommandEvent &event )
{
    
}

void MainFrame::OnStretch( wxCommandEvent &event )
{
    
}
// ----------------------------------------------------------------------------
// Widget's borders
// ----------------------------------------------------------------------------
void MainFrame::OnBorderLeft( wxCommandEvent &event )
{
    
}

void MainFrame::OnBorderRight( wxCommandEvent &event )
{
    
}

void MainFrame::OnBorderTop( wxCommandEvent &event )
{
    
}

void MainFrame::OnBorderBottom( wxCommandEvent &event )
{
    
}
