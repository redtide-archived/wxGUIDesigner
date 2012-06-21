///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/manager.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/app.h>
#include <wx/dir.h>
#include <wx/config.h>
#include <wx/event.h>
#include <wx/filefn.h>
#include <wx/frame.h>
#include <wx/fs_arc.h>
#include <wx/fs_mem.h>
#include <wx/intl.h>
#include <wx/notebook.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sstream.h>
#include <wx/stc/stc.h>
#include <wx/stdpaths.h>
#include <wx/treectrl.h>
#include <wx/xml/xml.h>
#include <wx/xrc/xmlres.h>

#include <wx/aui/auibar.h>
#include <wx/aui/dockart.h>
#include <wx/aui/framemanager.h>

#include <wx/xrc/xh_aui.h>
#include <wx/xrc/xh_stc.h>
#include <wx/xrc/xh_propgrid.h>
#include "wx/xrc/gd_frame.h"
#include "wx/xrc/gd_wizard.h"

#include "wxguidesigner/rtti/tree.h"

#include "wxguidesigner/settings.h"
#include "wxguidesigner/gui/aui/dockart.h"
#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/gui/editor.h"
#include "wxguidesigner/gui/palette.h"
#include "wxguidesigner/gui/propgrid/propbook.h"
#include "wxguidesigner/gui/treeview.h"

#ifdef __WXDEBUG__
    #include <wx/textctrl.h>
    #include "wxguidesigner/gui/debugwindow.h"
#endif

#include "wxguidesigner/gui/dialog/prefs.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/mainframe.h"

#include "wxguidesigner/events.h"
#include "wxguidesigner/utils.h"

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
#ifdef __WXDEBUG__
m_debug         ( NULL ),
m_logOld        ( NULL ),
#endif
m_tree          ( new ObjectTree() )//,
//m_settings      ( new Settings() )
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

    bool enabled = false; int selected = 0; int language = 0;

    // wxConfigBase must be initialized in the main application,
    // so we can use its configuration to load/store values
    wxConfigBase::Get()->Read( "locale/enabled",  &enabled,  false );
    wxConfigBase::Get()->Read( "locale/selected", &selected, 0 );

    wxGDArtProvider::Load();

    if( enabled )
    {
        switch( selected )
        {
            case 0: language = wxLANGUAGE_DEFAULT; break;
            case 1: language = wxLANGUAGE_ENGLISH; break;
            case 2: language = wxLANGUAGE_ITALIAN; break;
        }

        SelectLanguage( language );
    }

    int xrcVerSel;
    wxString xrcVer = "2.5.3.0";
    wxConfigBase::Get()->Read( "locale/selected", &xrcVerSel, 1 );

    if( xrcVerSel == 0 )
        xrcVer = "2.3.0.1";

    wxXmlNode *root = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "resource" );
    root->AddAttribute( "xmlns", "http://www.wxwidgets.org/wxxrc" );
    root->AddAttribute( "version", xrcVer );
    m_xrcDoc.SetRoot( root );
}

wxGDHandler::~wxGDHandler()
{
    delete wxLog::SetActiveTarget( m_logOld );
    m_handlers.clear();
    wxXmlResource::Get()->ClearHandlers();
    wxGDArtProvider::Unload();
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
                    LeftDockable(false).RightDockable(false).DockFixed(true).
                    PaneBorder(false) );

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
                    TopDockable(false).BottomDockable(false) );
#ifdef __WXDEBUG__
        mgr->AddPane( GetDebugWindow(pnl), wxAuiPaneInfo().Bottom().
                    Name("DebugWindowPane").Caption(_("Debugger") ).
                    CloseButton(false).MinSize(-1,120).FloatingSize(300,120).
                    LeftDockable(false).RightDockable(false) );
#endif
        m_frame->LoadLayout();

        mgr->Update();
    }

    return m_frame;
}

#ifdef __WXDEBUG__
wxTextCtrl *wxGDHandler::GetDebugWindow( wxWindow *parent )
{
    if( !m_debug )
    {
        m_debug = new wxGDDebugWindow( this, parent );
        m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_debug));
        wxLogMessage(_("Started") );
    }

    return m_debug;
}
#endif
wxDialog *wxGDHandler::GetAboutDialog( wxWindow *parent )
{
    return wxXmlResource::Get()->LoadDialog( parent, "About" );
}

wxDialog *wxGDHandler::GetSettingsDialog( wxWindow *parent )
{
    return new wxGDDialogPrefs( parent );
}

wxNotebook *wxGDHandler::GetEditorBook( wxWindow *parent )
{
    if( !m_editBook )
        m_editBook = new wxGDEditorBook( this, parent );

    return m_editBook;
}

wxNotebook *wxGDHandler::GetPaletteBook( wxWindow *parent )
{
    if( !m_palette )
    {
        m_palette = new wxGDToolPalette( this, parent );

        wxString c = "controls";

        for( size_t g = 0; g < wxGDArtProvider::GetGroupCount( c ); g++ )
        {
            wxString     lbl = wxGDArtProvider::GetGroupLabel( c, g );
            int          idx = wxGDArtProvider::GetGroupImageListIndex( c, g );
            wxBitmap     bmp = wxGDArtProvider::GroupsImageList->GetBitmap( idx );
            wxAuiToolBar *tb = m_palette->AddGroup( lbl, bmp );

            for( size_t i = 0; i < wxGDArtProvider::GetItemCount( c, g ); i++ )
            {
                wxString item = wxGDArtProvider::GetItemLabel( c, g, i );

                if( item == "-" )
                {
                    tb->AddSeparator();
                }
                else
                {
                    idx = wxGDArtProvider::GetItemImageListIndex( c, g, i );
                    bmp = wxGDArtProvider::ItemsImageList->GetBitmap( idx );
                    tb->AddTool( wxID_ANY, item, bmp, item );
                }
            }

            tb->Realize();
        }
    }

    return m_palette;
}

wxNotebook *wxGDHandler::GetPropertyBook( wxWindow *parent )
{
    if( !m_propBook )
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
        m_handlers.push_back( m_treeView );
    }

    return m_treeView;
}

wxGDSettings wxGDHandler::GetSettings() const
{
    return m_settings;
}

void wxGDHandler::CreateObject( const wxString &className, int senderId )
{
    // Create the object informations
    Object object = m_tree->CreateObject( className );
    Object parent = m_tree->GetSelectObject();

    if( !object || !parent )
        return;

    // Create the real object
    wxObject *wxobject = wxCreateDynamicObject( className );
    if( !wxobject )
        return;

    wxLogMessage( className + " " + _("created") );

    // Serialize the object into the xrc project
    wxXmlNode *objNode = object->Serialize( m_xrcDoc.GetRoot() );
    m_xrcDoc.GetRoot()->AddChild( objNode );

    // Save the xrc file as a string to print it in the xrc editor
    wxStringOutputStream sout;
    m_xrcDoc.Save( sout, 4 );
/*
    // Print the XML result
    wxStyledTextCtrl *xrcEdit = GetEditor( NULL, "xrc" );
    wxString xrcText = sout.GetString();
    if( xrcEdit )
        xrcEdit->SetText( xrcText );
*/
    wxGDObjectEvent event( wxGD_EVT_OBJECT_CREATED, senderId, object );
//  event.SetString( xrcText );
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

void wxGDHandler::SelectLanguage( int language )
{
    if ( !m_locale.Init( language ) )
    {
        wxLogDebug("This language is not supported by the system.");
        return;
    } 

#ifdef __WXGTK__

    wxLocale::AddCatalogLookupPathPrefix("/usr/share/locale");
    wxLocale::AddCatalogLookupPathPrefix("/usr/local/share/locale");

#elif defined(__WXMSW__)

    wxLocale::AddCatalogLookupPathPrefix( wxStandardPaths::Get().GetDataDir() +
                                        "\\locale" );
#endif
    wxLocale::AddCatalogLookupPathPrefix("locale");

    m_locale.AddCatalog("wxguidesigner");

#ifdef __LINUX__
    wxLogNull noLog;
    m_locale.AddCatalog("fileutils");
#endif
}
