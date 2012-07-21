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

#include "wxguidesigner/rtti.h"

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
class wxXmlDocument;
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
class wxGDMainFrame;
class ObjectTree;

using namespace std;
using namespace std::tr1;

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
    wxImageList     *GetLargeImageList() const { return m_largeImgs; }
    wxImageList     *GetSmallImageList() const { return m_smallImgs; }

#ifdef __WXDEBUG__
    wxGDDebugWindow *GetDebugWindow     ( wxWindow *parent = NULL );
#endif
//-----------------------------------------------------------------------------
// Serialize
//-----------------------------------------------------------------------------
    bool            Load                ( const wxString &filePath );
    bool            Save                ( const wxString &filePath );
    wxXmlDocument   Serialize();
//-----------------------------------------------------------------------------
// Object operations
//-----------------------------------------------------------------------------
    void CreateObject( const wxString &className, int senderId );
    void SelectObject( Object object, int senderId );
    Object GetSelectedObject() const;
    Object GetTopLevelObject( Object object );

    void SendEvent      ( wxEvent &event, bool delayed = false );

private:
    void InitAllXmlHandlers();
    void SelectLanguage ( int language );

#ifdef __WXDEBUG__
    wxGDDebugWindow     *m_debug;
    wxLog               *m_logOld;
#endif
    wxImageList         *m_largeImgs;
    wxImageList         *m_smallImgs;
    wxMenuBar           *m_menuBar;
    wxToolBar           *m_toolBar;
    wxGDMainFrame       *m_frame;
    wxGDEditorBook      *m_editBook;
    wxGDToolPalette     *m_palette;
    wxGDPropertyBook    *m_propBook;
    wxGDTreeView        *m_treeView;

    vector< wxEvtHandler * >        m_handlers;
    RTTITree                        m_tree;
//  wxGDSettings                    m_settings;
    wxLocale                        m_locale;
};

#endif //__WXGUIDESIGNER_GUI_HANDLER_H__
