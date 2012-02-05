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

#include "core/gui/iconprovider.h"
#include "core/gui/manager.h"
#include "core/gui/frame.h"
#include "core/gui/editor.h"
#include "core/gui/palette.h"
#include "core/gui/propgrid.h"
#include "core/gui/treeview.h"

#include "core/object/tree.h"
#include "core/utils.h"

#include <wx/xrc/xh_aui.h>
#include <wx/xrc/xh_stc.h>
#include <wx/xrc/xh_propgrid.h>

#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/frame.h>
#include <wx/fs_arc.h>
#include <wx/notebook.h>
#include <wx/propgrid/propgrid.h>
#include <wx/stc/stc.h>
#include <wx/stdpaths.h>
#include <wx/treectrl.h>
#include <wx/xml/xml.h>

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

    IconProvider::Get();

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
    wxString wxGDPropertyBook  = wxGDXRCArchive + "#zip:propbook.xrc";
    wxString wxGDTreeView      = wxGDXRCArchive + "#zip:treeview.xrc";
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
        ObjectTree::Get()->RemoveHandler( m_propBookHndlr );
        delete m_propBookHndlr;
        m_propBookHndlr = NULL;
    }

    if ( m_treeViewHndlr )
    {
        ObjectTree::Get()->RemoveHandler( m_treeViewHndlr );
        delete m_treeViewHndlr;
        m_treeViewHndlr = NULL;
    }

    m_editors.clear();
}

void GUIManager::Free()
{
    IconProvider::Free();
    ObjectTree::Free();

    wxXmlResource::Get()->ClearHandlers();

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

wxMenuBar *GUIManager::GetMainMenu( wxWindow *parent )
{
    if ( !m_menuBar )
        m_menuBar  = wxXmlResource::Get()->LoadMenuBar( parent, "MainMenu" );

    return m_menuBar;
}

wxDialog *GUIManager::GetAboutDialog( wxWindow *parent )
{
    wxDialog *dlg = wxXmlResource::Get()->LoadDialog( parent, "About" );

    return dlg;
}

wxNotebook *GUIManager::GetEditorBook( wxWindow *parent )
{
    if ( !m_editBook )
    {
        m_editBook = XRCCTRL( *parent, "EditorBook", wxNotebook );

        if ( m_editBook && IconProvider::Get()->SelectCategory("languages") )
        {
            m_editBookHndlr = new EditorHandler( m_editBook );

            for ( size_t i = 0; i < IconProvider::Get()->GetGroupCount(); i++ )
            {
                wxString    label   = IconProvider::Get()->GetGroupLabel( i );
                wxBitmap    bmp     = IconProvider::Get()->GetGroupBitmap( i );
                wxImageList *imgLst = m_editBook->GetImageList();

                if ( IconProvider::Get()->GetItemCount( i ) )
                {
                    wxNotebook  *nb     = new wxNotebook( m_editBook, wxID_ANY );
                    wxImageList *itmLst = new wxImageList( 16, 16 );

                    nb->SetImageList( itmLst );
                    m_editBook->AddPage( nb, label, false, imgLst->Add( bmp ) );

                    for ( size_t n = 0;
                          n < IconProvider::Get()->GetItemCount( i ); n++ )
                    {
                        wxString item = IconProvider::Get()->GetItemLabel( i, n );
                        wxStyledTextCtrl *stc = GetEditor( nb, item );

                        if ( stc )
                        {
                            bmp = IconProvider::Get()->GetItemBitmap( i, n );
                            nb->AddPage( stc, item, false, itmLst->Add( bmp ) );
                        }
                    }
                }
                else
                {
                    wxString name = IconProvider::Get()->GetGroupName( i );
                    wxStyledTextCtrl *stc = GetEditor( m_editBook, name );

                    if ( stc )
                    {
                        m_editBook->AddPage( stc, label, false, imgLst->Add( bmp ) );
                    }
                }
            }

            ObjectTree::Get()->AddHandler( m_editBookHndlr );
        }
    }

    return m_editBook;
}

wxPanel *GUIManager::GetDesignPanel()
{
    return XRCCTRL( *m_editBook, "DesignerPanel", wxPanel );
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
        m_palette = XRCCTRL( *parent, "ToolPalette", wxNotebook );

        if ( m_palette && IconProvider::Get()->SelectCategory("controls") )
        {
            m_paletteHndlr = new PaletteHandler( m_palette );

            for ( size_t i = 0; i < IconProvider::Get()->GetGroupCount(); i++ )
            {
                wxString    label = IconProvider::Get()->GetGroupLabel( i );
                wxBitmap    bmp   = IconProvider::Get()->GetGroupBitmap( i );
                wxToolGroup *tg   = m_paletteHndlr->AddGroup( label, bmp );

                for ( size_t n = 0;
                      n < IconProvider::Get()->GetItemCount( i ); n++ )
                {
                    wxString item = IconProvider::Get()->GetItemLabel( i, n );

                    if ( item == "-" )
                    {
                        tg->AddSeparator();
                    }
                    else
                    {
                        bmp = IconProvider::Get()->GetItemBitmap( i, n );
                        tg->AddTool( wxID_ANY, item, bmp, item );
                    }
                }

                tg->Realize();
            }

            m_palette->Bind( wxEVT_COMMAND_TOOL_CLICKED,
                            &PaletteHandler::OnToolClicked, m_paletteHndlr );
        }
    }

    return m_palette;
}

wxNotebook *GUIManager::GetPropertyBook( wxWindow *parent )
{
    if ( !m_propBook )
    {
        m_propBook = XRCCTRL( *parent, "PropertyBook", wxNotebook );
        if ( m_propBook )
        {
            m_propBookHndlr = new PropBookHandler( m_propBook );

            ObjectTree::Get()->AddHandler( m_propBookHndlr );

            m_propBook->Bind( wxEVT_SIZE, &PropBookHandler::OnSize,
                                m_propBookHndlr );

            m_pgProps     = XRCCTRL( *m_propBook, "PropGrid", wxPropertyGrid );
            m_pgEvents    = XRCCTRL( *m_propBook, "EventGrid", wxPropertyGrid );
            m_ilsPropBook = m_propBook->GetImageList();

            m_pgProps->Bind( wxEVT_PG_CHANGED,
                            &PropBookHandler::OnPGChanged, m_propBookHndlr );
            m_pgProps->Bind( wxEVT_PG_SELECTED,
                            &PropBookHandler::OnPGSelected, m_propBookHndlr );
            m_pgEvents->Bind( wxEVT_PG_CHANGED,
                            &PropBookHandler::OnEGChanged, m_propBookHndlr );
            m_pgEvents->Bind( wxEVT_PG_SELECTED,
                            &PropBookHandler::OnEGSelected, m_propBookHndlr );
            m_pgEvents->Bind( wxEVT_PG_DOUBLE_CLICK,
                            &PropBookHandler::OnEGDblClick, m_propBookHndlr );
        }
    }

    return m_propBook;
}

wxToolBar *GUIManager::GetToolBar( wxWindow *parent )
{
    if ( !m_toolBar )
        m_toolBar = wxXmlResource::Get()->LoadToolBar( parent, "ToolBar" );

    return m_toolBar;
}

wxTreeCtrl *GUIManager::GetTreeView( wxWindow *parent )
{
    if ( !m_treeView )
    {
        m_treeView = XRCCTRL( *parent, "TreeView", wxTreeCtrl );

        if ( m_treeView && IconProvider::Get()->SelectCategory("controls") )
        {
            wxImageList *ils = m_treeView->GetImageList();
            if ( !ils )
            {
                ils = new wxImageList( 22, 22 );
                m_treeView->AssignImageList( ils );
            }

            for ( size_t i = 0; i < IconProvider::Get()->GetGroupCount(); i++ )
            {
                for ( size_t n = 0;
                      n < IconProvider::Get()->GetItemCount( i ); n++ )
                {
                    wxString lbl = IconProvider::Get()->GetItemLabel( i, n );
                    if ( lbl != "-" )
                    {
                        wxBitmap bmp = IconProvider::Get()->GetItemBitmap( i, n );
                        int      idx = ils->Add( bmp );
                        m_imgIds.insert( ImageIds::value_type( lbl, idx ) );
                    }
                }
            }

            m_treeView->AddRoot( "Project", 0 );

            m_treeViewHndlr = new TreeViewHandler( m_treeView );

            ObjectTree::Get()->AddHandler( m_treeViewHndlr );

            m_treeView->Bind( wxEVT_COMMAND_TREE_SEL_CHANGED,
                            &TreeViewHandler::OnSelChanged, m_treeViewHndlr );

            m_treeView->Bind( wxEVT_COMMAND_TREE_BEGIN_DRAG,
                            &TreeViewHandler::OnBeginDrag, m_treeViewHndlr );

            m_treeView->Bind( wxEVT_COMMAND_TREE_END_DRAG,
                            &TreeViewHandler::OnEndDrag, m_treeViewHndlr );
        }
    }

    return m_treeView;
}


wxStyledTextCtrl *GUIManager::GetEditor( wxWindow *parent, const wxString &name )
{
    // If already loaded, return this editor
    CodeEditors::const_iterator it = m_editors.find( name );
    if ( it != m_editors.end() )
        return it->second;

    wxString langDirPath = GetDataBasePath() + wxFILE_SEP_PATH + "languages" +
                                    wxFILE_SEP_PATH + name + wxFILE_SEP_PATH;
    wxDir langDir( langDirPath );

    if ( langDir.IsOpened() )
    {
        wxString xrcFile;
        bool haveXrc =
            langDir.GetFirst( &xrcFile, "*.xrc", wxDIR_FILES | wxDIR_HIDDEN );

        wxString xrcPath = langDirPath + wxFILE_SEP_PATH + xrcFile;

        if ( !haveXrc || !wxXmlResource::Get()->Load( xrcPath ) )
            return NULL;
    }

    wxObject *obj = wxXmlResource::Get()->LoadObject
                                        ( parent, name, "wxStyledTextCtrl" );

    wxStyledTextCtrl *stc = wxDynamicCast( obj, wxStyledTextCtrl );

    // Save editor for later use
    if ( stc )
        m_editors.insert( CodeEditors::value_type( name, stc ) );

    return stc;
}

int GUIManager::GetImageIndex( const wxString &className )
{
    ImageIds::iterator it = m_imgIds.find( className );
    if ( it != m_imgIds.end() )
        return it->second;

    return wxNOT_FOUND;
}
