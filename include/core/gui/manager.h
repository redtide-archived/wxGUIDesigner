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

#include "core/dllimpexp.h"
#include <map>

class MainFrame;
class EditorHandler;
class PropBookHandler;
class PaletteHandler;
class TreeViewHandler;

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

class wxPropertyGridXmlHandler;

class DLLIMPEXP_CORE GUIManager
{
public:
    wxDialog        *GetAboutDialog      ( wxWindow *parent = 0L );
    wxFrame         *GetMainFrame        ( wxWindow *parent = 0L );
    wxMenuBar       *GetMainMenu         ( wxWindow *parent = 0L );
    wxPanel         *GetDesignPanel();
    wxNotebook      *GetEditorBook       ( wxWindow *parent = 0L );
    wxNotebook      *GetPropertyBook     ( wxWindow *parent = 0L );
    wxNotebook      *GetPaletteBook      ( wxWindow *parent = 0L );
    wxTreeCtrl      *GetTreeView         ( wxWindow *parent = 0L );
    wxToolBar       *GetToolBar          ( wxWindow *parent = 0L );

    wxPropertyGrid  *GetPropertiesGrid() { return m_pgProps; }
    wxPropertyGrid  *GetEventsGrid()     { return m_pgEvents; }

    wxStyledTextCtrl *GetEditor( wxWindow *parent, const wxString &name );

    int GetImageIndex( const wxString &classname );

    static GUIManager *Get();
    void Free();

private:
    GUIManager();
    virtual ~GUIManager();

    void OnWindowPaint( wxPaintEvent &event );

    typedef std::map< wxString, wxStyledTextCtrl * > CodeEditors;
    typedef std::map< wxString, int >                ImageIds;

    ImageIds        m_imgIds;
    CodeEditors     m_editors;
    MainFrame       *m_frame;
    wxMenuBar       *m_menuBar;
    wxToolBar       *m_toolBar;
    wxNotebook      *m_editBook;
    wxNotebook      *m_propBook;
    wxNotebook      *m_palette;
    wxTreeCtrl      *m_treeView;
    wxImageList     *m_ilsPropBook;
    wxPropertyGrid  *m_pgProps;
    wxPropertyGrid  *m_pgEvents;
    EditorHandler   *m_editBookHndlr;
    PaletteHandler  *m_paletteHndlr;
    PropBookHandler *m_propBookHndlr;
    TreeViewHandler *m_treeViewHndlr;

    static GUIManager *ms_instance;
};

#endif //__CORE_GUI_MANAGER_H__
