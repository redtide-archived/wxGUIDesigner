///////////////////////////////////////////////////////////////////////////////
// Name:        core/manager.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "manager.h"

#include <wx/aui/auibar.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/propgrid/manager.h>
#include <wx/sizer.h>
#include <wx/dcclient.h>
#include <wx/log.h>

wxGUIDesigner::wxGUIDesigner()
{
    m_frame     = NULL;
    m_frameHandler   = NULL;
    m_paletteHandler = NULL;
    m_menuBar   = NULL;
    m_toolBar   = NULL;
    m_objTree   = NULL;
    m_editor    =
    m_objInsp   =
    m_objPalette = NULL;
    m_objInspImages = NULL;
    m_pgProps = m_pgEvents = NULL;

    wxInitAllImageHandlers();

    m_plugMgr = PluginManager::Get();

    m_xmlResource = wxXmlResource::Get();
    m_xmlResource->InitAllHandlers();

    m_xmlResource->AddHandler( new wxPropertyGridXmlHandler );
    m_xmlResource->AddHandler( new wxStyledTextCtrlXmlHandler );

    m_xmlResource->Load( wxGD_MAINMENU );
    m_xmlResource->Load( wxGD_TOOLBAR );

    if ( !m_xmlResource->Load( wxGD_ABOUT ) ) return;
//  if ( !m_xmlResource->Load( wxGD_DESIGNER ) ) return;
    if ( !m_xmlResource->Load( wxGD_EDITOR ) ) return;
    if ( !m_xmlResource->Load( wxGD_OBJECT_INSPECTOR ) ) return;
    if ( !m_xmlResource->Load( wxGD_OBJECT_PALETTE ) ) return;
    if ( !m_xmlResource->Load( wxGD_OBJECT_TREE ) ) return;
    
    m_xmlResource->Load( wxGD_XRC_DIR + wxFILE_SEP_PATH + "plugins/advanced/animation.xrc" );
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

wxFrame *wxGUIDesigner::GetMainFrame( wxWindow *parent )
{
    if ( !m_frame )
    {
        m_auiXmlHandler = new wxAuiXmlHandler();
        m_xmlResource->AddHandler( m_auiXmlHandler );

        if ( !m_xmlResource->Load( wxGD_AUIFRAME ) ) return NULL;

        m_frame = m_xmlResource->LoadFrame( NULL, "MainFrame" );

        if ( !m_frame ) return NULL;

        GetObjectInspector( m_frame );
        GetObjectPalette( m_frame );
        GetObjectTree( m_frame );
        GetEditor( m_frame );

        m_frameHandler = new FrameHandler( m_frame );

        m_frame->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                        &FrameHandler::OnAbout,      m_frameHandler, XRCID("wxID_ABOUT") );

        m_frame->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                        &FrameHandler::OnNewProject, m_frameHandler, XRCID("wxID_NEW") );

        m_frame->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                        &FrameHandler::OnExit,       m_frameHandler, XRCID("wxID_EXIT") );

        m_frame->Bind( wxEVT_CLOSE_WINDOW, &FrameHandler::OnClose, m_frameHandler );
    }

    if ( wxFileExists( wxGD_LOGO ) );
    {
        wxIcon       ico16, ico32;
        wxIconBundle bundle;
        wxImage      imgLogo = wxImage( wxGD_LOGO );

        ico16.CopyFromBitmap( imgLogo.Scale( 16, 16 ) );
        bundle.AddIcon( ico16 );

        ico32.CopyFromBitmap( imgLogo.Scale( 32, 32 ) );
        bundle.AddIcon( ico32 );

        m_frame->SetIcons( bundle );
    }

    return m_frame;
}

wxNotebook *wxGUIDesigner::GetEditor( wxWindow *parent )
{
    if ( !m_editor )
        m_editor = XRCCTRL( *parent, "Editor", wxNotebook );

    return m_editor;
}

wxPanel *wxGUIDesigner::GetDesignerWindow()
{
    return XRCCTRL( *GetEditor(), "DesignerWindow", wxPanel );
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

wxNotebook *wxGUIDesigner::GetObjectPalette( wxWindow *parent )
{
    if ( !m_objPalette )
    {
        m_objPalette = XRCCTRL( *parent, "ObjectPalette", wxNotebook );
        if ( m_objPalette )
        {
            m_paletteHandler = new PaletteHandler( m_objPalette );

            m_objPalette->Bind( wxEVT_PLUGIN_LOADED,
                                &PaletteHandler::OnPluginLoaded, m_paletteHandler );

            m_plugMgr->AddHandler( m_objPalette );
            m_plugMgr->LoadPlugins();

            m_objPalette->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                                &PaletteHandler::OnToolClicked, m_paletteHandler );
        }
    }

    return m_objPalette;
}

wxTreeCtrl *wxGUIDesigner::GetObjectTree( wxWindow *parent )
{
    if ( !m_objTree )
        m_objTree = XRCCTRL( *parent, "ObjectTree", wxTreeCtrl );

    return m_objTree;
}

wxNotebook *wxGUIDesigner::GetObjectInspector( wxWindow *parent )
{
    if ( !m_objInsp )
    {
        m_objInsp = XRCCTRL( *parent, "ObjectInspector", wxNotebook );
        if ( m_objInsp )
        {
            m_pgProps       = XRCCTRL( *m_objInsp, "PropGrid", wxPropertyGrid );
            m_pgEvents      = XRCCTRL( *m_objInsp, "EventGrid", wxPropertyGrid );
            m_objInspImages = m_objInsp->GetImageList();
        }
    }

    return m_objInsp;
}

void wxGUIDesigner::NewProject()
{

}

void wxGUIDesigner::CreateObject( const wxString &classname, wxWindow *parent )
{
    if ( !parent )
        parent = GetDesignerWindow();

    if ( !parent ) return;

    wxObject *obj = m_xmlResource->LoadObject( parent, name, classname );
    wxWindow *win = wxDynamicCast( obj, wxWindow );
    if ( win )
    {
        
    }
}

void wxGUIDesigner::Free()
{
    m_xmlResource->ClearHandlers();

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
