///////////////////////////////////////////////////////////////////////////////
// Name:        core/manager.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_MANAGER_H__
#define __WXGDCORE_MANAGER_H__

#include <wx/dialog.h>
#include <wx/frame.h>
#include <wx/imaglist.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/toolbar.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include "defs.h"

#include "handlers/mainframe.h"
#include "handlers/editor.h"
#include "handlers/palette.h"

#include "handlers/xh_aui.h"
#include "handlers/xh_propgrid.h"
#include "handlers/xh_stc.h"

#include "plugin.h"

class DLLIMPEXP_CORE wxGUIDesigner
{
public:
    wxDialog        *GetAboutDialog      ( wxWindow *parent = NULL );
    wxFrame         *GetFrame            ( wxWindow *parent = NULL );
    wxMenuBar       *GetMainMenu         ( wxWindow *parent = NULL );
    wxPanel         *GetDesignPanel();
    wxNotebook      *GetEditorBook       ( wxWindow *parent = NULL );
    wxNotebook      *GetPropertyBook     ( wxWindow *parent = NULL );
    wxNotebook      *GetPaletteBook      ( wxWindow *parent = NULL );
    wxTreeCtrl      *GetTreeView         ( wxWindow *parent = NULL );
    wxToolBar       *GetToolBar          ( wxWindow *parent = NULL );

    wxPropertyGrid  *GetPropertiesGrid() { return m_pgProps; }
    wxPropertyGrid  *GetEventsGrid()     { return m_pgEvents; }

    void NewProject();
    void CreateObject( const wxString &name, wxWindow *parent );

    static wxGUIDesigner *Get();
    void Free();

protected:
    wxGUIDesigner();
    virtual ~wxGUIDesigner();

    void OnWindowPaint( wxPaintEvent &event );

    wxFrame     *m_frame;
    wxImageList *m_ilsPropBook;
    wxMenuBar   *m_menuBar;
    wxToolBar   *m_toolBar;
    wxNotebook  *m_editBook;
    wxNotebook  *m_propBook;
    wxNotebook  *m_palette;
    wxTreeCtrl  *m_treeView;

    wxPropertyGrid  *m_pgProps;
    wxPropertyGrid  *m_pgEvents;

    wxXmlResource            *m_xmlResource;
    wxAuiXmlHandler          *m_auiXmlHandler;
    wxPropertyGridXmlHandler *m_pgXmlHandler;

    EditorHandler   *m_editorHandler;
    FrameHandler    *m_frameHandler;
    PaletteHandler  *m_paletteHandler;

    PluginManager   *m_plugMgr;

    static wxGUIDesigner *ms_instance;
};

#endif //__WXGDCORE_MANAGER_H__
