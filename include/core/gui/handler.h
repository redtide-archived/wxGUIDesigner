///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/manager.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __CORE_GUI_MANAGER_H__
#define __CORE_GUI_MANAGER_H__

#include <wx/xml/xml.h>

#include "core/dllimpexp.h"
#include "core/defs.h"
#include <map>

class wxEvent;
class wxEvtHandler;
class wxDialog;
class wxFrame;
class wxImageList;
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

class wxGDArtProvider;
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

class DLLIMPEXP_CORE wxGDHandler : public wxEvtHandler
{
public:
    wxGDHandler();
    ~wxGDHandler();

    wxFrame         *GetMainFrame       ( wxWindow *parent = NULL );
    wxDialog        *GetAboutDialog     ( wxWindow *parent );
    wxDialog        *GetSettingsDialog  ( wxWindow *parent );
    wxPanel         *GetDesignPanel();
    wxNotebook      *GetEditorBook      ( wxWindow *parent );
    wxNotebook      *GetPropertyBook    ( wxWindow *parent );
    wxNotebook      *GetPaletteBook     ( wxWindow *parent );
    wxTreeCtrl      *GetTreeView        ( wxWindow *parent );
    wxToolBar       *GetToolBar         ( wxWindow *parent );
    wxStyledTextCtrl *GetEditor( wxWindow *parent, const wxString &name );

    wxGDSettings GetSettings() const;

    int GetImageIndex( const wxString &className );

//=============================================================================
// Object operations
//=============================================================================
    void CreateObject( const wxString &className, int senderId );
    void SelectObject( Object object, int senderId );

    wxXmlDocument GetXRCDocument() { return m_xrcDoc; }

    shared_ptr< ObjectTree > GetTree() const { return m_tree; }

private:
    void SendEvent( wxEvent &event, bool delayed = false );

    void OnWindowPaint( wxPaintEvent &event );

    typedef map< wxString, wxStyledTextCtrl * > CodeEditors;
    typedef map< wxString, int >                ImageIds;

    ImageIds            m_imgIds;
    CodeEditors         m_editors;
    wxGDFrame           *m_frame;
    wxMenuBar           *m_menuBar;
    wxToolBar           *m_toolBar;
    wxGDEditorBook      *m_editBook;
    wxGDToolPalette     *m_palette;
    wxGDPropertyBook    *m_propBook;
    wxGDTreeView        *m_treeView;
    wxGDDialogPrefs     *m_prefs;
    wxImageList         *m_ilsPropBook;
    wxXmlDocument       m_xrcDoc;

    vector< wxEvtHandler * >        m_handlers;
    shared_ptr< wxGDArtProvider >   m_icons;
    shared_ptr< ObjectTree >        m_tree;
    wxGDSettings                    m_settings;
};

#endif //__CORE_GUI_MANAGER_H__
