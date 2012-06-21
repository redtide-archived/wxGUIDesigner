///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/handler.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_HANDLER_H__
#define __WXGUIDESIGNER_GUI_HANDLER_H__

#include <wx/xml/xml.h>
#include "wxguidesigner/dllimpexp.h"
#include "wxguidesigner/defs.h"
#include <map>

class wxEvent;
class wxEvtHandler;
class wxDialog;
class wxFrame;
class wxImageList;
class wxLocale;
class wxMenuBar;
class wxNotebook;
class wxPaintEvent;
class wxPanel;
class wxPropertyGrid;
class wxString;
class wxStyledTextCtrl;
class wxToolBar;
class wxTreeCtrl;
class wxWindow;
class wxXmlResource;

#ifdef __WXDEBUG__
    class wxLog;
    class wxTextCtrl;
    class wxGDDebugWindow;
#endif

class wxGDEditorBook;
class wxGDPropertyBook;
class wxGDToolPalette;
class wxGDTreeView;
class wxGDFrame;
class wxGDDialogPrefs;
class Settings;
class ObjectTree;

using namespace std;
using namespace std::tr1;

typedef shared_ptr< Settings > wxGDSettings;

class DLLIMPEXP_WXGUIDESIGNER wxGDHandler : public wxEvtHandler
{
public:
    wxGDHandler();
    ~wxGDHandler();

    wxFrame         *GetMainFrame       ( wxWindow *parent = NULL );
    wxDialog        *GetAboutDialog     ( wxWindow *parent );
    wxDialog        *GetSettingsDialog  ( wxWindow *parent );
    wxNotebook      *GetEditorBook      ( wxWindow *parent );
    wxNotebook      *GetPropertyBook    ( wxWindow *parent );
    wxNotebook      *GetPaletteBook     ( wxWindow *parent );
    wxTreeCtrl      *GetTreeView        ( wxWindow *parent );
    wxToolBar       *GetToolBar         ( wxWindow *parent );
    wxImageList     *GetControlsImageList();

#ifdef __WXDEBUG__
    wxTextCtrl      *GetDebugWindow     ( wxWindow *parent );
#endif

    wxGDSettings GetSettings() const;
//=============================================================================
// Object operations
//=============================================================================
    void CreateObject( const wxString &className, int senderId );
    void SelectObject( Object object, int senderId );

    wxXmlDocument GetXRCDocument() { return m_xrcDoc; }

    shared_ptr< ObjectTree > GetTree() const { return m_tree; }

private:
    void SelectLanguage ( int language );

    void SendEvent      ( wxEvent &event, bool delayed = false );

    wxGDFrame           *m_frame;
    wxMenuBar           *m_menuBar;
    wxToolBar           *m_toolBar;
    wxGDEditorBook      *m_editBook;
    wxGDToolPalette     *m_palette;
    wxGDPropertyBook    *m_propBook;
    wxGDTreeView        *m_treeView;
    wxXmlDocument       m_xrcDoc;

    vector< wxEvtHandler * >        m_handlers;
#ifdef __WXDEBUG__
    wxGDDebugWindow                 *m_debug;
    wxLog                           *m_logOld;
#endif
    shared_ptr< ObjectTree >        m_tree;
    wxGDSettings                    m_settings;
    wxLocale                        m_locale;
};

#endif //__WXGUIDESIGNER_GUI_HANDLER_H__
