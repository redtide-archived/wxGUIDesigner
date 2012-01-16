///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/manager.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/20
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/gui/manager.h"
#include "core/gui/frame.h"
#include "core/gui/editor.h"
#include "core/gui/palette.h"
#include "core/gui/propgrid.h"
#include "core/gui/treeview.h"

#include "core/object/tree.h"

#include <wx/xrc/xh_aui.h>
#include <wx/xrc/xh_stc.h>
#include <wx/xrc/xh_propgrid.h>

#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/propgrid/propgrid.h>
#include <wx/treectrl.h>
#include <wx/fs_arc.h>
#include <wx/xml/xml.h>
#include <wx/filefn.h>
#include <wx/stdpaths.h>

#include <wx/log.h>
#include <wx/xrc/xmlres.h>

GUIManager::GUIManager() :  m_frame( NULL ),
                            m_menuBar( NULL ),
                            m_toolBar( NULL ),
                            m_editBook( NULL ),
                            m_propBook( NULL ),
                            m_palette( NULL ),
                            m_treeView( NULL ),
                            m_ilsPropBook( NULL ),
                            m_pgProps( NULL ),
                            m_pgEvents( NULL ),
                            m_editBookHndlr( NULL ),
                            m_paletteHndlr( NULL ),
                            m_propBookHndlr( NULL ),
                            m_treeViewHndlr( NULL )
{
    wxInitAllImageHandlers();

    wxXmlResource::Get()->InitAllHandlers();
    wxXmlResource::Get()->AddHandler( new wxPropertyGridXmlHandler );
    wxXmlResource::Get()->AddHandler( new wxStyledTextCtrlXmlHandler );

    wxFileSystem::AddHandler( new wxArchiveFSHandler );

    wxString wxGDXRCDir = wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') +
                            wxFILE_SEP_PATH + "wxguidesigner" + wxFILE_SEP_PATH;

    wxString wxGDXRCArchive    = wxGDXRCDir     + "wxguidesigner.xrs";
    wxString wxGDAboutDialog   = wxGDXRCArchive + "#zip:about.xrc";
    wxString wxGDDesignerPanel = wxGDXRCArchive + "#zip:designer.xrc";
    wxString wxGDEditorBook    = wxGDXRCArchive + "#zip:editor.xrc";
    wxString wxGDMainMenu      = wxGDXRCArchive + "#zip:mainmenu.xrc";
    wxString wxGDPropertyBook  = wxGDXRCArchive + "#zip:objinsp.xrc";
    wxString wxGDTreeView      = wxGDXRCArchive + "#zip:objtree.xrc";
    wxString wxGDPaletteBook   = wxGDXRCArchive + "#zip:palette.xrc";
    wxString wxGDToolbar       = wxGDXRCArchive + "#zip:toolbar.xrc";
    wxString wxGDImages        = wxGDXRCArchive + "#zip:images.xrc";

    wxXmlResource::Get()->Load( wxGDMainMenu );
    wxXmlResource::Get()->Load( wxGDToolbar );

    if ( !wxXmlResource::Get()->Load( wxGDImages ) )       return;
    if ( !wxXmlResource::Get()->Load( wxGDAboutDialog ) )  return;
//  if ( !wxXmlResource::Get()->Load( wxGDDesignerPanel ) ) return;
    if ( !wxXmlResource::Get()->Load( wxGDEditorBook ) )   return;
    if ( !wxXmlResource::Get()->Load( wxGDPropertyBook ) ) return;
    if ( !wxXmlResource::Get()->Load( wxGDPaletteBook ) )  return;
    if ( !wxXmlResource::Get()->Load( wxGDTreeView ) )     return;
}

GUIManager::~GUIManager()
{
}

void GUIManager::Free()
{
    wxXmlResource::Get()->ClearHandlers();

    if ( m_editBookHndlr )
    {
        ObjectTree::Get()->RemoveHandler( m_editBookHndlr );
        delete m_editBookHndlr;
        m_editBookHndlr = NULL;
    }

    if ( m_paletteHndlr )
    {
        delete m_paletteHndlr;
        m_paletteHndlr = NULL;
    }

    if ( m_propBookHndlr )
    {
        delete m_propBookHndlr;
        m_propBookHndlr = NULL;
    }

    if ( m_treeViewHndlr )
    {
        delete m_treeViewHndlr;
        m_treeViewHndlr = NULL;
    }

    PluginManager::Get()->Free();

    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

GUIManager *GUIManager::ms_instance = NULL;

GUIManager *GUIManager::Get()
{
    if ( !ms_instance )
        ms_instance = new GUIManager;

    return ms_instance;
}

wxDialog *GUIManager::GetAboutDialog( wxWindow *parent )
{
    wxDialog *dlg = wxXmlResource::Get()->LoadDialog( parent, "About" );

    return dlg;
}

wxMenuBar *GUIManager::GetMainMenu( wxWindow *parent )
{
    if ( !m_menuBar )
        m_menuBar  = wxXmlResource::Get()->LoadMenuBar( parent, "MainMenu" );

    return m_menuBar;
}

wxToolBar *GUIManager::GetToolBar( wxWindow *parent )
{
    if ( !m_toolBar )
        m_toolBar = wxXmlResource::Get()->LoadToolBar( parent, "ToolBar" );

    return m_toolBar;
}

wxFrame *GUIManager::GetMainFrame( wxWindow *parent )
{
    if ( !m_frame )
    {
        wxAuiXmlHandler *auiXmlHandler = new wxAuiXmlHandler();
        wxXmlResource::Get()->AddHandler( auiXmlHandler );

        wxString wxGDXRCDir = wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') +
                                wxFILE_SEP_PATH + "wxguidesigner" + wxFILE_SEP_PATH;
        wxString wxGDXRCArchive = wxGDXRCDir    + "wxguidesigner.xrs";
        wxString wxGDAUIFrame   = wxGDXRCArchive + "#zip:mainframe.xrc";

        if ( !wxXmlResource::Get()->Load( wxGDAUIFrame ) )
            return NULL;

        m_frame = new MainFrame( NULL );

        m_frame->m_mgr = auiXmlHandler->GetAuiManager
                        ( wxFindWindowById( XRCID("PanelMain"), m_frame ) );

        m_frame->LoadLayout();

        GetPropertyBook( m_frame );
        GetPaletteBook( m_frame );
        GetTreeView( m_frame );
        GetEditorBook( m_frame );
    }

    wxBitmap bmpLogo = wxXmlResource::Get()->LoadBitmap("Logo");

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

wxNotebook *GUIManager::GetEditorBook( wxWindow *parent )
{
    if ( !m_editBook )
    {
        m_editBook = XRCCTRL( *parent, "Editor", wxNotebook );

        if ( m_editBook )
        {
            m_editBookHndlr = new EditorHandler( m_editBook );

            PluginManager::Get()->AddHandler( m_editBookHndlr );
            PluginManager::Get()->LoadPlugins("languages");
            ObjectTree::Get()->AddHandler( m_editBookHndlr );
        }
    }

    return m_editBook;
}

wxPanel *GUIManager::GetDesignPanel()
{
    return XRCCTRL( *m_editBook, "DesignerWindow", wxPanel );
}

void GUIManager::OnWindowPaint( wxPaintEvent &event )
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

wxNotebook *GUIManager::GetPaletteBook( wxWindow *parent )
{
    if ( !m_palette )
    {
        m_palette = XRCCTRL( *parent, "ObjectPalette", wxNotebook );
        if ( m_palette )
        {
            m_paletteHndlr = new PaletteHandler( m_palette );

            PluginManager::Get()->AddHandler( m_paletteHndlr );
            PluginManager::Get()->LoadPlugins("controls");

            m_palette->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                            &PaletteHandler::OnToolClicked, m_paletteHndlr );
        }
    }

    return m_palette;
}

wxTreeCtrl *GUIManager::GetTreeView( wxWindow *parent )
{
    if ( !m_treeView )
        m_treeView = XRCCTRL( *parent, "ObjectTree", wxTreeCtrl );
/*
        if ( m_treeView )
        {
            m_treeViewHndlr = new TreeViewHandler( m_treeView );

            ObjectTree::Get()->AddHandler( m_treeViewHndlr );

            m_treeView->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                            &TreeViewHandler::OnToolClicked, m_treeViewHndlr );
        }
*/
    return m_treeView;
}

wxNotebook *GUIManager::GetPropertyBook( wxWindow *parent )
{
    if ( !m_propBook )
    {
        m_propBook = XRCCTRL( *parent, "ObjectInspector", wxNotebook );
        if ( m_propBook )
        {
            m_propBookHndlr = new PropBookHandler( m_propBook );

            m_propBook->Bind( wxEVT_SIZE, &PropBookHandler::OnSize,
                                m_propBookHndlr );

            m_pgProps     = XRCCTRL( *m_propBook, "PropGrid", wxPropertyGrid );
            m_pgEvents    = XRCCTRL( *m_propBook, "EventGrid", wxPropertyGrid );
            m_ilsPropBook = m_propBook->GetImageList();
        }
    }

    return m_propBook;
}
/*
void GUIManager::NewProject()
{
    wxPanel *designer = GetDesignPanel();
    if ( !designer || !m_treeView || !m_pgProps ) return;

    wxString projFile = wxGD_PLUGIN_DIR + wxFILE_SEP_PATH + "classes" +
                        wxFILE_SEP_PATH + "project.xml";

    if ( !wxFileExists( projFile ) ) return;

    wxXmlDocument doc;
    if ( !doc.Load( projFile ) ) return;

    wxXmlNode *rootNode = doc.GetRoot();

    if ( rootNode->GetName() != "class" ) return;

    wxXmlNode *childNode = rootNode->GetChildren();
    while ( childNode )
    {
        if ( childNode && childNode->GetName() == "properties" )
        {
            wxXmlNode *propNode = childNode->GetChildren();
            while ( propNode )
            {
                if ( propNode->GetName() == "name" )
                {
                    //m_project = new Object( propNode->GetNodeContent() );
                }

                propNode = propNode->GetNext();
            }
        }

        childNode = childNode->GetNext();
    }

    if ( m_project )
    {
        
    }
}

void GUIManager::CreateObject( const wxString &classname )
{

    using namespace wxGDConv;

//  wxLogDebug(HexToString(wxBORDER_RAISED));
//  wxLogDebug("%i", StringToHex("0x04000000"));
//  wxColour clr = wxGDConv::GetSystemColour("wxSYS_COLOUR_BTNSHADOW");
//  GetEditorBook()->SetBackgroundColour(clr);
    int i = wxUP | wxDOWN | wxRIGHT | wxLEFT;
    wxLogDebug("%i", i);
    wxLogDebug("%i", StringToHex("0x00F0"));

    #define TEST1(arg) #arg
    #define TEST2(arg) TEST1(arg)

    wxString s = TEST1(wxBORDER_RAISED);

    const char *c = TEST1(s);

    int i = (int)&c;

    wxLogDebug(s);
    wxLogDebug("%i", i);


    #undef TEST1
    #undef TEST2

    if ( !m_project ) return;

    

//  Object *object = new Object();
    

    wxObject *obj = m_xmlResource->LoadObject( parent, name, classname );
    wxWindow *win = wxDynamicCast( obj, wxWindow );
    if ( win )
    {
        
    }
}
*/
