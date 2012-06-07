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
#include <wx/dir.h>
#include <wx/event.h>
#include <wx/filefn.h>
#include <wx/frame.h>
#include <wx/fs_arc.h>
#include <wx/fs_mem.h>
#include <wx/notebook.h>
#include <wx/propgrid/propgrid.h>
#include <wx/stc/stc.h>
#include <wx/stdpaths.h>
#include <wx/treectrl.h>
#include <wx/xml/xml.h>
#include <wx/xrc/xmlres.h>

#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>

#include <wx/xrc/xh_aui.h>
#include <wx/xrc/xh_stc.h>
#include <wx/xrc/xh_propgrid.h>
#include "wx/xrc/gd_frame.h"
#include "wx/xrc/gd_wizard.h"

#include "core/object/tree.h"

#include "core/settings.h"
#include "core/gui/auidockart.h"
#include "core/gui/iconprovider.h"
#include "core/gui/editor.h"
#include "core/gui/palette.h"
#include "core/gui/propgrid/propbook.h"
#include "core/gui/treeview.h"
#include "core/gui/dialog/prefs.h"
#include "core/gui/handler.h"
#include "core/gui/frame.h"

#include "core/events.h"
#include "core/utils.h"

#include <wx/log.h>

wxGDHandler::wxGDHandler()
:
wxEvtHandler(),
m_frame         ( NULL ),
m_menuBar       ( NULL ),
m_toolBar       ( NULL ),
m_editBook      ( NULL ),
m_palette       ( NULL ),
m_propBook      ( NULL ),
m_treeView      ( NULL ),
m_ilsPropBook   ( NULL ),
m_icons         ( new wxGDArtProvider() ),
m_tree          ( new ObjectTree() ),
m_settings      ( new Settings() )
{
    wxXmlResource::Get()->InitAllHandlers();
    wxXmlResource::Get()->AddHandler( new wxStyledTextCtrlXmlHandler );
    wxXmlResource::Get()->AddHandler( new FrameXmlHandler );
    wxXmlResource::Get()->AddHandler( new WizardXmlHandler );

    wxFileSystem::AddHandler( new wxArchiveFSHandler );
    wxFileSystem::AddHandler( new wxMemoryFSHandler );

    wxString wxGDXRCDir        = GetResourcePath();
    wxString wxGDXRCArchive    = wxGDXRCDir     + "wxguidesigner.xrs";
    wxString wxGDImages        = wxGDXRCArchive + "#zip:images.xrc";
    wxString wxGDMainMenu      = wxGDXRCArchive + "#zip:mainmenu.xrc";
    wxString wxGDToolbar       = wxGDXRCArchive + "#zip:toolbar.xrc";
    wxString wxGDAboutDialog   = wxGDXRCArchive + "#zip:about.xrc";

    wxXmlResource::Get()->Load( wxGDImages );
    wxXmlResource::Get()->Load( wxGDMainMenu );
    wxXmlResource::Get()->Load( wxGDToolbar );
    wxXmlResource::Get()->Load( wxGDAboutDialog );

    // TODO: Retrieve the XRC version from config
    wxXmlNode *root = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "resource" );
    root->AddAttribute( "xmlns", "http://www.wxwidgets.org/wxxrc" );
    root->AddAttribute( "version", "2.5.3.0" );
    m_xrcDoc.SetRoot( root );
}

wxGDHandler::~wxGDHandler()
{
    m_editors.clear();
    m_handlers.clear();
//  m_tree = shared_ptr< ObjectTree >();
}

wxFrame *wxGDHandler::GetMainFrame( wxWindow *parent )
{
    if(!m_frame)
    {
        m_frame = new wxGDFrame(this);

        wxAuiManager *mgr = m_frame->GetAUIManager();
        wxPanel      *pnl = m_frame->GetAUIPanel();

        mgr->SetArtProvider( new wxGDAUIDockArt() );

        mgr->AddPane( GetPaletteBook(pnl), wxAuiPaneInfo().Top().
                    Name("ToolPalettePane").Caption(_("Controls") ).
                    CloseButton(false).MinSize(-1,69).FloatingSize(300,69).
                    LeftDockable(false).RightDockable(false).DockFixed(true) );

        mgr->AddPane( GetTreeView(pnl), wxAuiPaneInfo().Left().
                    Name("TreeViewPane").Caption(_("Project") ).
                    CloseButton(false).MinSize(180,-1).FloatingSize(150,300).
                    TopDockable(false).BottomDockable(false) );

        mgr->AddPane( GetEditorBook(pnl), wxAuiPaneInfo().Center().
                    Name("EditorBookPane").Caption(_("Editor") ).
                    CloseButton(false).DockFixed(true) );

        mgr->AddPane( GetPropertyBook(pnl), wxAuiPaneInfo().Right().
                    Name("PropertyBookPane").Caption(_("Properties") ).
                    CloseButton(false).MinSize(180,-1).FloatingSize(150,300).
                    TopDockable(false).BottomDockable( false ) );

        m_frame->LoadLayout();

        mgr->Update();
    }

    return m_frame;
}

wxDialog *wxGDHandler::GetAboutDialog( wxWindow *parent )
{
    return wxXmlResource::Get()->LoadDialog( parent, "About" );
}

wxDialog *wxGDHandler::GetSettingsDialog( wxWindow *parent )
{
    if( !m_prefs )
        m_prefs = new wxGDDialogPrefs( this, parent );

    return m_prefs;
}

wxNotebook *wxGDHandler::GetEditorBook( wxWindow *parent )
{
    if( !m_editBook )
    {
        m_editBook = new wxGDEditorBook( this, parent );

        if( m_icons->SelectCategory("languages") )
        {
            for( size_t i = 0; i < m_icons->GetGroupCount(); i++ )
            {
                wxString    label   = m_icons->GetGroupLabel( i );
                wxBitmap    bmp     = m_icons->GetGroupBitmap( i );
                wxImageList *imgLst = m_editBook->GetImageList();

                if( m_icons->GetItemCount( i ) )
                {
                    wxNotebook  *nb     = new wxNotebook( m_editBook, wxID_ANY );
                    wxImageList *itmLst = new wxImageList( 16, 16 );

                    nb->SetImageList( itmLst );
                    m_editBook->AddPage( nb, label, false, imgLst->Add( bmp ) );

                    for( size_t n = 0;
                          n < m_icons->GetItemCount( i ); n++ )
                    {
                        wxString item = m_icons->GetItemLabel( i, n );
                        wxStyledTextCtrl *stc = GetEditor( nb, item );

                        if( stc )
                        {
                            bmp = m_icons->GetItemBitmap( i, n );
                            nb->AddPage( stc, item, false, itmLst->Add( bmp ) );
                        }
                    }
                }
                else
                {
                    wxString name = m_icons->GetGroupName( i );
                    wxStyledTextCtrl *stc = GetEditor( m_editBook, name );

                    if( stc )
                    {
                        m_editBook->AddPage( stc, label, false, imgLst->Add( bmp ) );
                    }
                }
            }

            m_handlers.push_back( m_editBook );
        }
    }

    return m_editBook;
}

wxPanel *wxGDHandler::GetDesignPanel()
{
    return XRCCTRL( *m_editBook, "DesignerPanel", wxPanel );
}

void wxGDHandler::OnWindowPaint( wxPaintEvent &event )
{
    wxWindow *win = wxDynamicCast( event.GetEventObject(), wxWindow );

    if( win )
    {
        wxPaintDC dc( win );

    //  dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxRED_PEN);
        dc.DrawRectangle(wxPoint(0,0), win->GetSize());
    }
    event.Skip();
}

wxNotebook *wxGDHandler::GetPaletteBook( wxWindow *parent )
{
    if( !m_palette )
    {
        m_palette = new wxGDToolPalette( this, parent );

        if( m_icons->SelectCategory("controls") )
        {
            for( size_t i = 0; i < m_icons->GetGroupCount(); i++ )
            {
                wxString    label = m_icons->GetGroupLabel( i );
                wxBitmap    bmp   = m_icons->GetGroupBitmap( i );
                wxToolGroup *tg   = m_palette->AddGroup( label, bmp );

                for( size_t n = 0;
                      n < m_icons->GetItemCount( i ); n++ )
                {
                    wxString item = m_icons->GetItemLabel( i, n );

                    if( item == "-" )
                    {
                        tg->AddSeparator();
                    }
                    else
                    {
                        bmp = m_icons->GetItemBitmap( i, n );
                        tg->AddTool( wxID_ANY, item, bmp, item );
                    }
                }

                tg->Realize();
            }
        }
    }

    return m_palette;
}

wxNotebook *wxGDHandler::GetPropertyBook( wxWindow *parent )
{
    if(!m_propBook)
    {
        m_propBook = new wxGDPropertyBook( this, parent );
        m_handlers.push_back( m_propBook );
    }

    return m_propBook;
}

wxToolBar *wxGDHandler::GetToolBar( wxWindow *parent )
{
    if(!m_toolBar)
        m_toolBar = wxXmlResource::Get()->LoadToolBar( parent, "ToolBar" );

    return m_toolBar;
}

wxTreeCtrl *wxGDHandler::GetTreeView( wxWindow *parent )
{
    if( !m_treeView )
    {
        m_treeView = new wxGDTreeView( this, parent );

        if( m_icons->SelectCategory("controls") )
        {
            wxImageList *imageList = m_treeView->GetImageList();
            if( !imageList )
            {
                imageList = new wxImageList( 22,22 );
                m_treeView->AssignImageList( imageList );
            }

            for( size_t i = 0; i < m_icons->GetGroupCount(); i++ )
            {
                for( size_t n = 0;
                      n < m_icons->GetItemCount( i ); n++ )
                {
                    wxString label = m_icons->GetItemLabel( i, n );
                    if( label != "-" )
                    {
                        wxBitmap bmp = m_icons->GetItemBitmap( i, n );
                        int      idx = imageList->Add( bmp );
                        m_imgIds.insert( ImageIds::value_type( label, idx ) );
                    }
                }
            }

            m_handlers.push_back( m_treeView );
        }
    }

    return m_treeView;
}

wxStyledTextCtrl *wxGDHandler::GetEditor( wxWindow *parent, const wxString &name )
{
    // If already loaded, return this editor
    CodeEditors::const_iterator it = m_editors.find( name );
    if( it != m_editors.end() )
        return it->second;

    wxString langDirPath = GetDataBasePath() + wxFILE_SEP_PATH + "languages" +
                                    wxFILE_SEP_PATH + name + wxFILE_SEP_PATH;
    wxDir langDir( langDirPath );

    if( langDir.IsOpened() )
    {
        wxString xrcFile;
        bool haveXrc =
            langDir.GetFirst( &xrcFile, "*.xrc", wxDIR_FILES | wxDIR_HIDDEN );

        wxString xrcPath = langDirPath + xrcFile;

        if( !haveXrc || !wxXmlResource::Get()->Load( xrcPath ) )
            return NULL;
    }

    wxObject *obj = wxXmlResource::Get()->LoadObject
                                        ( parent, name, "wxStyledTextCtrl" );

    wxStyledTextCtrl *stc = wxDynamicCast( obj, wxStyledTextCtrl );

    // Save editor for later use
    if( stc )
        m_editors.insert( CodeEditors::value_type( name, stc ) );

    return stc;
}

wxGDSettings wxGDHandler::GetSettings() const
{
    return m_settings;
}

int wxGDHandler::GetImageIndex( const wxString &className )
{
    ImageIds::iterator it = m_imgIds.find( className );
    if( it != m_imgIds.end() )
        return it->second;

    return wxNOT_FOUND;
}

void wxGDHandler::CreateObject( const wxString &className, int senderId )
{
    Object object = m_tree->CreateObject( className );
    if( !object )
        return;

    Object parent = m_tree->GetSelectObject();
    if( parent )
    {
        wxXmlNode *objNode = object->Serialize( m_xrcDoc.GetRoot() );
        m_xrcDoc.GetRoot()->AddChild( objNode );
    }

/*
#include <wx/sstream.h>
#include <wx/fs_mem.h>
#include <wx/wizard.h>
    wxStringOutputStream sout;
    wxXmlDocument doc = m_handler->GetXRCDocument();
    doc.Save(sout,4);
    wxStyledTextCtrl *xrcEdit = m_handler->GetEditor(NULL,"xrc");
    wxString xrcText = sout.GetString();
    if( xrcEdit )
        xrcEdit->SetText( xrcText );
    wxMemoryFSHandler::AddFile("xrc.xrc", xrcText );
    wxXmlResource::Get()->Load("memory:xrc.xrc");
    if( wxXmlResource::Get()->LoadObject( object, m_client, name, className ) )
    {
        wxWindow *window = wxDynamicCast( object, wxWindow );
        if( window )
        {
            wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
            sizer->Add( window, 1, wxEXPAND );
            m_client->SetSizer( sizer );
            m_resizer->SetSize( window->GetBestSize() );
        }
    }

    wxMemoryFSHandler::RemoveFile("xrc.xrc");
*/

    wxObject *wxobject = wxCreateDynamicObject( className );
    if( !wxobject )
        return;

    wxGDObjectEvent event( wxGD_EVT_OBJECT_CREATED, senderId, object );
    event.SetEventObject( wxobject );
    SendEvent( event );
}

void wxGDHandler::SelectObject( Object object, int senderId )
{
    if( !object )
        return;
        
    m_tree->SelectObject( object );
    wxGDObjectEvent event( wxGD_EVT_OBJECT_SELECTED, senderId, object );
    SendEvent( event );
}

void wxGDHandler::SendEvent( wxEvent &event, bool delayed )
{
    vector< wxEvtHandler * >::iterator handler;
    // Process the event immediatly or delay it using
    // QueueEvent to be thread safe to all wxEvtHandlers
    for( handler = m_handlers.begin(); handler != m_handlers.end(); ++handler )
    {
        if( (*handler) == event.GetEventObject() )
            continue; // Skip the sender

        if( delayed )
            (*handler)->QueueEvent( event.Clone() );
        else
            (*handler)->ProcessEvent( event );
    }
}
