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
#include <wx/xrc/xh_all.h>

#include <wx/xrc/xh_aui.h>
#include <wx/xrc/xh_stc.h>
#include <wx/xrc/xh_propgrid.h>

#include "wx/xrc/gd_dialog.h"
#include "wx/xrc/gd_frame.h"
#include "wx/xrc/gd_propdlg.h"
#include "wx/xrc/gd_wizard.h"

#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/tree.h"

#include "wxguidesigner/xrc/serializer.h"

//#include "wxguidesigner/settings.h"
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
#include "wxguidesigner/gui/mainframe.h"

#include "wxguidesigner/events.h"
#include "wxguidesigner/utils.h"

#include "wxguidesigner/gui/handler.h"

#include <wx/log.h>

wxGDHandler::wxGDHandler()
:
wxEvtHandler(),
m_menuBar       ( NULL ),
m_toolBar       ( NULL ),
m_frame         ( NULL ),
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
    InitAllXmlHandlers();

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

    // Load imagelists shared by wxGDTreeView and wxGDToolPalette
    wxGDArtProvider::Load("controls");

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
}

wxGDHandler::~wxGDHandler()
{
    delete wxLog::SetActiveTarget( m_logOld );
    m_handlers.clear();
//  wxXmlResource::Get()->ClearHandlers(); done in wxXmlResource dtor
    wxGDArtProvider::Unload();
//  m_tree = shared_ptr< ObjectTree >();
}

wxFrame *wxGDHandler::GetMainFrame( wxWindow *parent )
{
    if(!m_frame)
        m_frame = new wxGDMainFrame(this);

    return m_frame;
}

#ifdef __WXDEBUG__
wxGDDebugWindow *wxGDHandler::GetDebugWindow( wxWindow *parent )
{
    if(!parent)
    {
        if( m_frame )
            parent = m_frame;
        else
            return NULL;
    }

    if(!m_debug)
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
    if(!m_editBook)
    {
        // Force groups to use small imagelist
        wxGDArtProvider::Load( "languages", true );

        m_editBook = new wxGDEditorBook( this, parent );
        m_handlers.push_back( m_editBook );
    }

    return m_editBook;
}

wxNotebook *wxGDHandler::GetPaletteBook( wxWindow *parent )
{
    if(!m_palette)
        m_palette = new wxGDToolPalette( this, parent );

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
    if(!m_treeView)
    {
        m_treeView = new wxGDTreeView( this, parent );
        m_handlers.push_back( m_treeView );
    }

    return m_treeView;
}
/*
wxGDSettings wxGDHandler::GetSettings() const
{
    return m_settings;
}
*/
void wxGDHandler::CreateObject( const wxString &className, int senderId )
{
    Object object = m_tree->CreateObject( className );

    wxGDObjectEvent event( wxGD_EVT_OBJECT_CREATED, senderId, object );
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

Object wxGDHandler::GetTopLevelObject( Object object )
{
    return m_tree->GetTopLevelObject( object );
}

Object wxGDHandler::GetSelectedObject() const
{
    return m_tree->GetSelectedObject();
}
//-----------------------------------------------------------------------------
// Serialize
//-----------------------------------------------------------------------------
bool wxGDHandler::Load( const wxString &filePath )
{
    return wxXRCSerializer::Load( m_tree, filePath );
}

bool wxGDHandler::Save( const wxString &filePath )
{
    return wxXRCSerializer::Save( m_tree, filePath );
}

wxXmlDocument wxGDHandler::Serialize()
{
    return wxXRCSerializer::Serialize( m_tree );
}

void wxGDHandler::SendEvent( wxEvent &event, bool delayed )
{
    vector< wxEvtHandler * >::iterator it;
    // Process the event immediatly or delay it using
    // QueueEvent to be thread safe to all wxEvtHandlers
    for( it = m_handlers.begin(); it != m_handlers.end(); ++it )
    {
        wxEvtHandler *handler = (*it);
        if( handler == event.GetEventObject() )
            continue; // Skip the sender

        if( delayed )
            handler->QueueEvent( event.Clone() );
        else
            handler->ProcessEvent( event );
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

void wxGDHandler::InitAllXmlHandlers()
{
    wxXmlResource::Get()->AddHandler(new wxUnknownWidgetXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxIconXmlHandler);
    wxXmlResource::Get()->AddHandler(new DialogXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxPanelXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxSizerXmlHandler);
    wxXmlResource::Get()->AddHandler(new FrameXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxScrolledWindowXmlHandler);

#if wxUSE_AUI
    wxXmlResource::Get()->AddHandler(new wxAuiXmlHandler);
#endif
#if wxUSE_ANIMATIONCTRL
    wxXmlResource::Get()->AddHandler(new wxAnimationCtrlXmlHandler);
#endif
#if wxUSE_BANNERWINDOW
    wxXmlResource::Get()->AddHandler(new wxBannerWindowXmlHandler);
#endif
#if wxUSE_BITMAPCOMBOBOX
    wxXmlResource::Get()->AddHandler(new wxBitmapComboBoxXmlHandler);
#endif
#if wxUSE_BMPBUTTON
    wxXmlResource::Get()->AddHandler(new wxBitmapButtonXmlHandler);
#endif
#if wxUSE_BOOKCTRL
    wxXmlResource::Get()->AddHandler(new PropertySheetDialogXmlHandler);
#endif
#if wxUSE_BUTTON
    wxXmlResource::Get()->AddHandler(new wxStdDialogButtonSizerXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxButtonXmlHandler);
#endif
#if wxUSE_CALENDARCTRL
    wxXmlResource::Get()->AddHandler(new wxCalendarCtrlXmlHandler);
#endif
#if wxUSE_CHECKBOX
    wxXmlResource::Get()->AddHandler(new wxCheckBoxXmlHandler);
#endif
#if wxUSE_CHECKLISTBOX
    wxXmlResource::Get()->AddHandler(new wxCheckListBoxXmlHandler);
#endif
#if wxUSE_CHOICE
    wxXmlResource::Get()->AddHandler(new wxChoiceXmlHandler);
#endif
#if wxUSE_CHOICEBOOK
    wxXmlResource::Get()->AddHandler(new wxChoicebookXmlHandler);
#endif
#if wxUSE_COLLPANE
    wxXmlResource::Get()->AddHandler(new wxCollapsiblePaneXmlHandler);
#endif
#if wxUSE_COLOURPICKERCTRL
    wxXmlResource::Get()->AddHandler(new wxColourPickerCtrlXmlHandler);
#endif
#if wxUSE_COMBOBOX
    wxXmlResource::Get()->AddHandler(new wxComboBoxXmlHandler);
#endif
#if wxUSE_COMBOCTRL
    wxXmlResource::Get()->AddHandler(new wxComboCtrlXmlHandler);
#endif
#if wxUSE_COMMANDLINKBUTTON
    wxXmlResource::Get()->AddHandler(new wxCommandLinkButtonXmlHandler);
#endif
#if wxUSE_DATEPICKCTRL
    wxXmlResource::Get()->AddHandler(new wxDateCtrlXmlHandler);
#endif
#if wxUSE_DIRDLG
    wxXmlResource::Get()->AddHandler(new wxGenericDirCtrlXmlHandler);
#endif
#if wxUSE_DIRPICKERCTRL
    wxXmlResource::Get()->AddHandler(new wxDirPickerCtrlXmlHandler);
#endif
#if wxUSE_EDITABLELISTBOX
    wxXmlResource::Get()->AddHandler(new wxEditableListBoxXmlHandler);
#endif
#if wxUSE_FILECTRL
    wxXmlResource::Get()->AddHandler(new wxFileCtrlXmlHandler);
#endif
#if wxUSE_FILEPICKERCTRL
    wxXmlResource::Get()->AddHandler(new wxFilePickerCtrlXmlHandler);
#endif
#if wxUSE_FONTPICKERCTRL
    wxXmlResource::Get()->AddHandler(new wxFontPickerCtrlXmlHandler);
#endif
#if wxUSE_GAUGE
    wxXmlResource::Get()->AddHandler(new wxGaugeXmlHandler);
#endif
#if wxUSE_GRID
    wxXmlResource::Get()->AddHandler( new wxGridXmlHandler);
#endif
#if wxUSE_HTML
    wxXmlResource::Get()->AddHandler(new wxHtmlWindowXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxSimpleHtmlListBoxXmlHandler);
#endif
#if wxUSE_HYPERLINKCTRL
    wxXmlResource::Get()->AddHandler( new wxHyperlinkCtrlXmlHandler);
#endif
#if wxUSE_LISTBOOK
    wxXmlResource::Get()->AddHandler(new wxListbookXmlHandler);
#endif
#if wxUSE_LISTBOX
    wxXmlResource::Get()->AddHandler(new wxListBoxXmlHandler);
#endif
#if wxUSE_LISTCTRL
    wxXmlResource::Get()->AddHandler(new wxListCtrlXmlHandler);
#endif
#if wxUSE_MDI
    wxXmlResource::Get()->AddHandler(new wxMdiXmlHandler);
#endif
#if wxUSE_MENUS
    wxXmlResource::Get()->AddHandler(new wxMenuXmlHandler);
    wxXmlResource::Get()->AddHandler(new wxMenuBarXmlHandler);
#endif
#if wxUSE_NOTEBOOK
    wxXmlResource::Get()->AddHandler(new wxNotebookXmlHandler);
#endif
#if wxUSE_ODCOMBOBOX
    wxXmlResource::Get()->AddHandler(new wxOwnerDrawnComboBoxXmlHandler);
#endif
#if wxUSE_RADIOBOX
    wxXmlResource::Get()->AddHandler(new wxRadioBoxXmlHandler);
#endif
#if wxUSE_RADIOBTN
    wxXmlResource::Get()->AddHandler(new wxRadioButtonXmlHandler);
#endif
#if 0 && wxUSE_RICHTEXT
    wxXmlResource::Get()->AddHandler(new wxRichTextCtrlXmlHandler);
#endif
#if wxUSE_SCROLLBAR
    wxXmlResource::Get()->AddHandler(new wxScrollBarXmlHandler);
#endif
#if wxUSE_SEARCHCTRL
    wxXmlResource::Get()->AddHandler(new wxSearchCtrlXmlHandler);
#endif
#if wxUSE_SLIDER
    wxXmlResource::Get()->AddHandler(new wxSliderXmlHandler);
#endif
#if wxUSE_SPINBTN
    wxXmlResource::Get()->AddHandler(new wxSpinButtonXmlHandler);
#endif
#if wxUSE_SPINCTRL
    wxXmlResource::Get()->AddHandler(new wxSpinCtrlXmlHandler);
#endif
#if wxUSE_SPLITTER
    wxXmlResource::Get()->AddHandler(new wxSplitterWindowXmlHandler);
#endif
#if wxUSE_STATBMP
    wxXmlResource::Get()->AddHandler(new wxStaticBitmapXmlHandler);
#endif
#if wxUSE_STATBOX
    wxXmlResource::Get()->AddHandler(new wxStaticBoxXmlHandler);
#endif
#if wxUSE_STATLINE
    wxXmlResource::Get()->AddHandler(new wxStaticLineXmlHandler);
#endif
#if wxUSE_STATTEXT
    wxXmlResource::Get()->AddHandler(new wxStaticTextXmlHandler);
#endif
#if wxUSE_STATUSBAR
    wxXmlResource::Get()->AddHandler(new wxStatusBarXmlHandler);
#endif
    wxXmlResource::Get()->AddHandler(new wxStyledTextCtrlXmlHandler);
#if wxUSE_TEXTCTRL
    wxXmlResource::Get()->AddHandler(new wxTextCtrlXmlHandler);
#endif
#if wxUSE_TOGGLEBTN
    wxXmlResource::Get()->AddHandler(new wxToggleButtonXmlHandler);
#endif
#if wxUSE_TIMEPICKCTRL
    wxXmlResource::Get()->AddHandler(new wxTimeCtrlXmlHandler);
#endif
#if wxUSE_TOOLBAR
    wxXmlResource::Get()->AddHandler(new wxToolBarXmlHandler);
#endif
#if wxUSE_TOOLBOOK
    wxXmlResource::Get()->AddHandler(new wxToolbookXmlHandler);
#endif
#if wxUSE_TREEBOOK
    wxXmlResource::Get()->AddHandler(new wxTreebookXmlHandler);
#endif
#if wxUSE_TREECTRL
    wxXmlResource::Get()->AddHandler(new wxTreeCtrlXmlHandler);
#endif
#if wxUSE_WIZARDDLG
    wxXmlResource::Get()->AddHandler(new WizardXmlHandler);
#endif
}