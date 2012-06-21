///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/mainframe.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/config.h>
#include <wx/dialog.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/interfaces/iobject.h"
//#include "wxguidesigner/settings.h"
#include "wxguidesigner/manager.h"
#include "wxguidesigner/gui/aui/dockart.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/mainframe.h"
//=============================================================================
// wxGDMainFrame
//=============================================================================
wxGDMainFrame::wxGDMainFrame( wxGDHandler *handler, wxWindow *parent )
:
wxFrame(parent, wxID_ANY, "wxGUIDesigner", wxDefaultPosition,
        wxSize(630,480), wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL),
m_handler   (handler),
m_panel     (NULL),
m_mgr       (NULL)
{
//=============================================================================
// Icon bundle
//=============================================================================
    wxBitmap bmpLogo = wxXmlResource::Get()->LoadBitmap("logo");
    if( bmpLogo.IsOk() )
    {
        wxImage imgLogo = bmpLogo.ConvertToImage();

        wxIcon       ico16, ico32;
        wxIconBundle bundle;

        ico16.CopyFromBitmap( imgLogo.Scale(16,16) );
        bundle.AddIcon(ico16);

        ico32.CopyFromBitmap( imgLogo.Scale(32,32) );
        bundle.AddIcon(ico32);

        SetIcons(bundle);
    }
//=============================================================================
// Bars
//=============================================================================
    wxMenuBar *menuBar = wxXmlResource::Get()->LoadMenuBar("MainMenu");
    SetMenuBar(menuBar);
    if(menuBar)
    {
        wxMenu *menuFile = menuBar->GetMenu(0);
        if(menuFile)
            m_history.UseMenu(menuFile);
    }

    SetToolBar( wxXmlResource::Get()->LoadToolBar(this, "ToolBar") );
    CreateStatusBar(1);

    SetMinSize( wxSize(630,480) );
//=============================================================================
// AUI
//=============================================================================
    m_panel = new wxPanel(this, wxID_ANY);
    m_mgr   = new wxAuiManager(m_panel, wxAUI_MGR_ALLOW_FLOATING    |
                                        wxAUI_MGR_LIVE_RESIZE       |
                                        wxAUI_MGR_HINT_FADE         |
                                        wxAUI_MGR_VENETIAN_BLINDS_HINT );

    m_mgr->SetArtProvider( new wxGDAUIDockArt() );

    wxWindow *palette  = (wxWindow *)m_handler->GetPaletteBook(m_panel);
    wxWindow *treeView = (wxWindow *)m_handler->GetTreeView(m_panel);
    wxWindow *editor   = (wxWindow *)m_handler->GetEditorBook(m_panel);
    wxWindow *propBook = (wxWindow *)m_handler->GetPropertyBook(m_panel);

    m_mgr->AddPane( palette, wxAuiPaneInfo().Top().
                    Name("ToolPalettePane").Caption(_("Controls") ).
                    CloseButton(false).MinSize(-1,69).FloatingSize(300,69).
                    LeftDockable(false).RightDockable(false).DockFixed(true).
                    PaneBorder(false) );

    m_mgr->AddPane( treeView, wxAuiPaneInfo().Left().
                    Name("TreeViewPane").Caption(_("Project") ).
                    CloseButton(false).MinSize(180,-1).FloatingSize(150,300).
                    TopDockable(false).BottomDockable(false) );

    m_mgr->AddPane( editor, wxAuiPaneInfo().Center().
                    Name("EditorBookPane").Caption(_("Editor") ).
                    CloseButton(false).DockFixed(true) );

    m_mgr->AddPane( propBook, wxAuiPaneInfo().Right().
                    Name("PropertyBookPane").Caption(_("Properties") ).
                    CloseButton(false).MinSize(180,-1).FloatingSize(150,300).
                    TopDockable(false).BottomDockable(false) );
#ifdef __WXDEBUG__
    wxWindow *dbgWnd = (wxWindow *)m_handler->GetDebugWindow(m_panel);

    m_mgr->AddPane( dbgWnd, wxAuiPaneInfo().Bottom().
                    Name("DebugWindowPane").Caption(_("Debugger") ).
                    CloseButton(false).MinSize(-1,120).FloatingSize(300,120).
                    LeftDockable(false).RightDockable(false) );
#endif
    LoadLayout();

    m_mgr->Update();
//=============================================================================
// Events
//=============================================================================
    Bind( wxEVT_CLOSE_WINDOW,         &wxGDMainFrame::OnClose, this );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnAbout,
                                this, XRCID("wxID_ABOUT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnPreferences,
                                this, XRCID("wxID_PREFERENCES") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnExit,
                                this, XRCID("wxID_EXIT") );
//=============================================================================
// Project
//=============================================================================
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnNewProject,
                                this, XRCID("wxID_NEW") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnOpenProject,
                                this, XRCID("wxID_OPEN") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnOpenRecent,
                                this, XRCID("wxID_FILE1"), XRCID("wxID_FILE9") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnSaveAsProject,
                                this, XRCID("wxID_SAVEAS") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnSaveProject,
                                this, XRCID("wxID_SAVE") );
//=============================================================================
// Undo/Redo
//=============================================================================
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnUndo,
                                this, XRCID("wxID_UNDO") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnRedo,
                                this, XRCID("wxID_REDO") );
//=============================================================================
// Clipboard
//=============================================================================
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnCut,
                                this, XRCID("wxID_CUT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnCopy,
                                this, XRCID("wxID_COPY") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnPaste,
                                this, XRCID("wxID_PASTE") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnDelete,
                                this, XRCID("wxID_DELETE") );
//=============================================================================
// CodeGenerator
//=============================================================================
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnGenerateCode,
                                this, XRCID("wxID_EXECUTE") );
//=============================================================================
// Widget's alignment
//=============================================================================
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnAlignLeft,
                                this, XRCID("ID_ALIGN_LEFT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnAlignCenterH,
                                this, XRCID("ID_ALIGN_CENTER_H") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnAlignRight,
                                this, XRCID("ID_ALIGN_RIGHT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnAlignTop,
                                this, XRCID("ID_ALIGN_TOP") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnAlignCenterV,
                                this, XRCID("ID_ALIGN_CENTER_V") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnAlignBottom,
                                this, XRCID("ID_ALIGN_BOTTOM") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnExpand,
                                this, XRCID("ID_EXPAND") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnStretch,
                                this, XRCID("ID_STRETCH") );
//=============================================================================
// Widget's borders
//=============================================================================
    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnBorderLeft,
                                this, XRCID("ID_BORDER_LEFT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnBorderRight,
                                this, XRCID("ID_BORDER_RIGHT") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnBorderTop,
                                this, XRCID("ID_BORDER_TOP") );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDMainFrame::OnBorderBottom,
                                this, XRCID("ID_BORDER_BOTTOM") );
}

wxGDMainFrame::~wxGDMainFrame()
{
    m_mgr->UnInit();
}
/*
void wxGDMainFrame::LoadLayout()
{
    wxGDSettings settings = m_handler->GetSettings();

    wxString perspective = settings->GetString("/mainframe/perspective");
    bool     maximized   = settings->GetBool( "/mainframe/maximized", true );
    bool     iconized    = settings->GetBool( "/mainframe/iconized", false );

    if( !perspective.empty() )
    {
        m_mgr->LoadPerspective( perspective );
        m_mgr->Update();
    }

    if( maximized )
    {
        Maximize( maximized );
    }
    else if( iconized )
    {
        Iconize( iconized );
    }
    else
    {
        int x = settings->GetInt( "/mainframe/left",   -1 );
        int y = settings->GetInt( "/mainframe/top",    -1 );
        int w = settings->GetInt( "/mainframe/width",  -1 );
        int h = settings->GetInt( "/mainframe/height", -1 );

        if( x > -1 && y > -1 && w >= 630 && h >= 480 )
        {
            SetSize( x, y, w, h );
        }
    }

    m_lastDir = settings->GetString("last_dir");

    m_history.Load( *wxConfigBase::Get() );
}
*/
void wxGDMainFrame::LoadLayout()
{
    wxString perspective;
    bool     maximized, iconized;

    wxConfigBase::Get()->Read( "/mainframe/perspective", &perspective );
    wxConfigBase::Get()->Read( "/mainframe/maximized",   &maximized, true );
    wxConfigBase::Get()->Read( "/mainframe/iconized",    &iconized, false );

    if( !perspective.empty() )
    {
        m_mgr->LoadPerspective( perspective );
        m_mgr->Update();
    }

    if( maximized )
    {
        Maximize( maximized );
    }
    else if( iconized )
    {
        Iconize( iconized );
    }
    else
    {
        int x, y, w, h;
        x = y = w = h = -1;

        wxConfigBase::Get()->Read( "/mainframe/left",   &x );
        wxConfigBase::Get()->Read( "/mainframe/top",    &y );
        wxConfigBase::Get()->Read( "/mainframe/width",  &w );
        wxConfigBase::Get()->Read( "/mainframe/height", &h );

        if( x > -1 && y > -1 && w >= 630 && h >= 480 )
        {
            SetSize( x, y, w, h );
        }
    }

    wxConfigBase::Get()->Read( "last_dir", &m_lastDir );
    m_history.Load( *wxConfigBase::Get() );
}

void wxGDMainFrame::SaveLayout()
{
//  wxGDSettings settings = m_handler->GetSettings();
    wxString perspective  = m_mgr->SavePerspective();
    bool        iconized  = IsIconized();
    bool        maximized = IsMaximized();
    int         left      = iconized ? -1 : GetPosition().x;
    int         top       = iconized ? -1 : GetPosition().y;
    int         width     = iconized ? -1 : GetSize().GetWidth();
    int         height    = iconized ? -1 : GetSize().GetHeight();

    wxConfigBase *config = wxConfigBase::Get();

    if( !maximized )
    {
        config->Write( "/mainframe/left",   left   );
        config->Write( "/mainframe/top",    top    );
        config->Write( "/mainframe/width",  width  );
        config->Write( "/mainframe/height", height );
    }

    config->Write( "/mainframe/perspective", perspective );
    config->Write( "/mainframe/maximized",   maximized   );
    config->Write( "/mainframe/iconized",    iconized    );
    config->Write( "last_dir",               m_lastDir   );

    m_history.Save( *config );
//  settings->Update();
}

bool wxGDMainFrame::SaveWarning()
{
    int result = wxYES;

    if( wxGUIDesigner::Get()->IsProjectModified() )
    {
        result = wxMessageBox(_("Current project file has been modified.\n" ) +
                                _("Do you want to save the changes?"),
                                _("Save project"), wxYES | wxNO | wxCANCEL, this );
        if( result == wxYES )
        {
            wxCommandEvent event;
            OnSaveProject( event );
        }
    }

    return( result != wxCANCEL );
}

void wxGDMainFrame::OnAbout( wxCommandEvent & )
{
    if(!m_handler)
        return;

    wxDialog *dlg = m_handler->GetAboutDialog( this );
    if( dlg )
    {
        dlg->ShowModal();
        dlg->Destroy();
    }
}

void wxGDMainFrame::OnPreferences( wxCommandEvent & )
{
    if(!m_handler)
        return;

    wxDialog *dlg = m_handler->GetSettingsDialog( this );
    if( dlg )
    {
        dlg->ShowModal();
        dlg->Destroy();
    }
}

void wxGDMainFrame::OnExit( wxCommandEvent & )
{
    Close();
}

void wxGDMainFrame::OnClose( wxCloseEvent &event )
{
    if( !SaveWarning() )
        return;

    SaveLayout();

    event.Skip();
}
//=============================================================================
// Project
//=============================================================================
void wxGDMainFrame::OnNewProject( wxCommandEvent & )
{
    if( !SaveWarning() )
        return;

    wxGUIDesigner::Get()->NewProject();
}

void wxGDMainFrame::OnOpenProject( wxCommandEvent & )
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

void wxGDMainFrame::OnOpenRecent( wxCommandEvent &event )
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
        if( wxGUIDesigner::Get()->LoadProject( fileName.GetFullPath() ) )
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

void wxGDMainFrame::OnSaveAsProject( wxCommandEvent & )
{
    wxFileDialog dlg( this, _("Save Project"), m_lastDir, wxEmptyString,
                      _("XRC Files (*.xrc)|*.xrc"), wxFD_SAVE );

    if( dlg.ShowModal() == wxID_OK )
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

void wxGDMainFrame::OnSaveProject( wxCommandEvent &event )
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
//=============================================================================
// Undo/Redo
//=============================================================================
void wxGDMainFrame::OnUndo( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnRedo( wxCommandEvent &event )
{
    
}
//=============================================================================
// Clipboard
//=============================================================================
void wxGDMainFrame::OnCut( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnCopy( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnPaste( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnDelete( wxCommandEvent &event )
{
    
}
//=============================================================================
// CodeGenerator
//=============================================================================
void wxGDMainFrame::OnGenerateCode( wxCommandEvent & )
{
    wxGUIDesigner::Get()->GenerateCode();
}
//=============================================================================
// Widget's alignment
//=============================================================================
void wxGDMainFrame::OnAlignLeft( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnAlignCenterH( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnAlignRight( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnAlignTop( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnAlignCenterV( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnAlignBottom( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnExpand( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnStretch( wxCommandEvent &event )
{
    
}
//=============================================================================
// Widget's borders
//=============================================================================
void wxGDMainFrame::OnBorderLeft( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnBorderRight( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnBorderTop( wxCommandEvent &event )
{
    
}

void wxGDMainFrame::OnBorderBottom( wxCommandEvent &event )
{
    
}
