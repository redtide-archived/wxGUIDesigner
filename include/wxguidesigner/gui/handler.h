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
class wxXmlNode;
class wxXmlResource;

#ifdef __WXDEBUG__
    class wxLog;
    class wxTextCtrl;
#endif


namespace wxGD
{
class EditorBook;
class PropertyBook;
class ToolPalette;
class TreeView;
class MainFrame;

namespace RTTI
{
class ObjectTree;
};

#ifdef __WXDEBUG__
    class DebugWindow;
#endif

class DLLIMPEXP_WXGUIDESIGNER Handler : public wxEvtHandler
{
public:
    Handler();
    ~Handler();

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
    DebugWindow *GetDebugWindow     ( wxWindow *parent = NULL );
#endif
//-----------------------------------------------------------------------------
// Serialize
//-----------------------------------------------------------------------------
    bool            Load                ( const wxString &filePath );
    bool            Save                ( const wxString &filePath );
    void            Serialize();
    void            SerializeObject     ( RTTI::Object object, wxXmlNode *rootNode );
//-----------------------------------------------------------------------------
// Object operations
//-----------------------------------------------------------------------------
    void            CreateObject( const wxString &className, int senderId );
    void            SelectObject( RTTI::Object object, int senderId );
    RTTI::Object    GetSelectedObject() const;

    void            SendEvent( wxEvent &event, bool delayed = false );

private:
    void            InitAllXmlHandlers();
    void            SelectLanguage ( int language );

#ifdef __WXDEBUG__
    DebugWindow     *m_debug;
    wxLog           *m_logOld;
#endif
    wxImageList     *m_largeImgs;
    wxImageList     *m_smallImgs;
    wxMenuBar       *m_menuBar;
    wxToolBar       *m_toolBar;
    MainFrame       *m_frame;
    EditorBook      *m_editBook;
    ToolPalette     *m_palette;
    PropertyBook    *m_propBook;
    TreeView        *m_treeView;

    std::vector< wxEvtHandler * >   m_handlers;
    RTTI::Tree                  m_tree;
    wxLocale                        m_locale;
};
};

#endif //__WXGUIDESIGNER_GUI_HANDLER_H__
