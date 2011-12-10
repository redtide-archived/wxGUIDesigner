///////////////////////////////////////////////////////////////////////////////
// Name:        core/manager.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "manager.h"

#include <wx/aui/auibar.h>
#include <wx/aui/framemanager.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/propgrid/manager.h>
#include <wx/sizer.h>
#include <wx/dcclient.h>
#include <wx/log.h>
#include <wx/filesys.h>
#include <wx/fs_arc.h>

wxGUIDesigner::wxGUIDesigner()
{
    m_frameHandler   = NULL;
    m_editorHandler  = NULL;
    m_paletteHandler = NULL;
    m_frame          = NULL;
    m_menuBar        = NULL;
    m_toolBar        = NULL;
    m_treeView       = NULL;
    m_editBook       = NULL;
    m_propBook       = NULL;
    m_palette        = NULL;
    m_ilsPropBook    = NULL;
    m_pgProps        = NULL;
    m_pgEvents       = NULL;

    wxInitAllImageHandlers();

    m_plugMgr = PluginManager::Get();

    m_xmlResource = wxXmlResource::Get();
    m_xmlResource->InitAllHandlers();

    m_xmlResource->AddHandler( new wxPropertyGridXmlHandler );
    m_xmlResource->AddHandler( new wxStyledTextCtrlXmlHandler );

    wxFileSystem::AddHandler( new wxArchiveFSHandler );

    m_xmlResource->Load( wxGD_MAINMENU );
    m_xmlResource->Load( wxGD_TOOLBAR );

    if ( !m_xmlResource->Load( wxGD_IMAGES ) )           return;
    if ( !m_xmlResource->Load( wxGD_ABOUT ) )            return;
//  if ( !m_xmlResource->Load( wxGD_DESIGNER ) )         return;
    if ( !m_xmlResource->Load( wxGD_EDITOR ) )           return;
    if ( !m_xmlResource->Load( wxGD_OBJECT_INSPECTOR ) ) return;
    if ( !m_xmlResource->Load( wxGD_OBJECT_PALETTE ) )   return;
    if ( !m_xmlResource->Load( wxGD_OBJECT_TREE ) )      return;
}

wxGUIDesigner::~wxGUIDesigner()
{
}

wxDialog *wxGUIDesigner::GetAboutDialog( wxWindow *parent )
{
    wxDialog *dlg = m_xmlResource->LoadDialog( parent, "About" );

    return dlg;
}

wxMenuBar *wxGUIDesigner::GetMainMenu( wxWindow *parent )
{
    if ( !m_menuBar )
        m_menuBar  = m_xmlResource->LoadMenuBar( parent, "MainMenu" );

    return m_menuBar;
}

wxToolBar *wxGUIDesigner::GetToolBar( wxWindow *parent )
{
    if ( !m_toolBar )
        m_toolBar = m_xmlResource->LoadToolBar( parent, "ToolBar" );

    return m_toolBar;
}

wxFrame *wxGUIDesigner::GetFrame( wxWindow *parent )
{
    if ( !m_frame )
    {
        m_auiXmlHandler = new wxAuiXmlHandler();
        m_xmlResource->AddHandler( m_auiXmlHandler );

        if ( !m_xmlResource->Load( wxGD_AUIFRAME ) ) return NULL;

        m_frame = m_xmlResource->LoadFrame( NULL, "MainFrame" );

        if ( !m_frame ) return NULL;

        GetPropertyBook( m_frame );
        GetPaletteBook( m_frame );
        GetTreeView( m_frame );
        GetEditorBook( m_frame );

        wxWindow *panel = wxWindow::FindWindowById( XRCID("PanelMain") );

        wxAuiManager *mgr = m_auiXmlHandler->GetAuiManager( panel );

        m_frameHandler = new FrameHandler( m_frame, mgr );

        m_frame->Bind( wxEVT_COMMAND_TOOL_CLICKED, &FrameHandler::OnAbout,
                        m_frameHandler, XRCID("wxID_ABOUT") );

        m_frame->Bind( wxEVT_COMMAND_TOOL_CLICKED, &FrameHandler::OnNewProject,
                        m_frameHandler, XRCID("wxID_NEW") );

        m_frame->Bind( wxEVT_COMMAND_TOOL_CLICKED, &FrameHandler::OnExit,
                        m_frameHandler, XRCID("wxID_EXIT") );

        m_frame->Bind( wxEVT_CLOSE_WINDOW, &FrameHandler::OnClose,
                        m_frameHandler );
    }

    wxBitmap bmpLogo = m_xmlResource->LoadBitmap("Logo");

    if ( bmpLogo.IsOk() )
    {
        wxImage imgLogo = bmpLogo.ConvertToImage();

        wxIcon       ico16, ico32;
        wxIconBundle bundle;

        ico16.CopyFromBitmap( imgLogo.Scale( 16, 16 ) );
        bundle.AddIcon( ico16 );

        ico32.CopyFromBitmap( imgLogo.Scale( 32, 32 ) );
        bundle.AddIcon( ico32 );

        m_frame->SetIcons( bundle );
    }

    return m_frame;
}

wxNotebook *wxGUIDesigner::GetEditorBook( wxWindow *parent )
{
    if ( !m_editBook )
    {
        m_editBook = XRCCTRL( *parent, "Editor", wxNotebook );

        if ( m_editBook )
        {
            m_editorHandler = new EditorHandler( m_editBook );

            m_editBook->Bind( wxEVT_PLUGIN_LOADED,
                            &EditorHandler::OnPluginLoaded, m_editorHandler );

            m_plugMgr->AddHandler( m_editBook );
            m_plugMgr->LoadPlugins("codegens");
        }
    }

    return m_editBook;
}

wxPanel *wxGUIDesigner::GetDesignPanel()
{
    return XRCCTRL( *m_editBook, "DesignerWindow", wxPanel );
}

void wxGUIDesigner::OnWindowPaint( wxPaintEvent &event )
{
    wxWindow *win = wxDynamicCast( event.GetEventObject(), wxWindow );

    if ( win )
    {
        wxPaintDC dc( win );

    //  dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxRED_PEN);
        dc.DrawRectangle(wxPoint(0,0), win->GetSize());
    }
    event.Skip();
}

wxNotebook *wxGUIDesigner::GetPaletteBook( wxWindow *parent )
{
    if ( !m_palette )
    {
        m_palette = XRCCTRL( *parent, "ObjectPalette", wxNotebook );
        if ( m_palette )
        {
            m_paletteHandler = new PaletteHandler( m_palette );

            m_palette->Bind( wxEVT_PLUGIN_LOADED,
                            &PaletteHandler::OnPluginLoaded, m_paletteHandler );

            m_plugMgr->AddHandler( m_palette );
            m_plugMgr->LoadPlugins("controls");

            m_palette->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                            &PaletteHandler::OnToolClicked, m_paletteHandler );
        }
    }

    return m_palette;
}

wxTreeCtrl *wxGUIDesigner::GetTreeView( wxWindow *parent )
{
    if ( !m_treeView )
        m_treeView = XRCCTRL( *parent, "ObjectTree", wxTreeCtrl );

    return m_treeView;
}

wxNotebook *wxGUIDesigner::GetPropertyBook( wxWindow *parent )
{
    if ( !m_propBook )
    {
        m_propBook = XRCCTRL( *parent, "ObjectInspector", wxNotebook );
        if ( m_propBook )
        {
            m_pgProps     = XRCCTRL( *m_propBook, "PropGrid", wxPropertyGrid );
            m_pgEvents    = XRCCTRL( *m_propBook, "EventGrid", wxPropertyGrid );
            m_ilsPropBook = m_propBook->GetImageList();
        }
    }

    return m_propBook;
}

void wxGUIDesigner::NewProject()
{

}

void wxGUIDesigner::CreateObject( const wxString &classname, wxWindow *parent )
{
    if ( !parent )
        parent = GetDesignPanel();

    if ( !parent ) return;
/*
    wxObject *obj = m_xmlResource->LoadObject( parent, name, classname );
    wxWindow *win = wxDynamicCast( obj, wxWindow );
    if ( win )
    {
        
    }*/
}

void wxGUIDesigner::Free()
{
    m_xmlResource->ClearHandlers();

    if ( m_editorHandler )  delete m_editorHandler;
    if ( m_frameHandler )   delete m_frameHandler;
    if ( m_paletteHandler ) delete m_paletteHandler;

    m_plugMgr->Free();
}

wxGUIDesigner *wxGUIDesigner::ms_instance = NULL;

wxGUIDesigner *wxGUIDesigner::Get()
{
    if ( !ms_instance )
        ms_instance = new wxGUIDesigner;

    return ms_instance;
}
