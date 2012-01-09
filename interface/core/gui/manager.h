///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/manager.h
// Purpose:     interface of GUIManager manager
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/12
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    @class GUIManager

    wxGUIDesigner provides access to all its GUI components trough this
    singleton class when using it either as a stand-alone application or
    when running as plugin inside another application.

    @library{core}
*/
class GUIManager
{
public:
    /**
        Gets the about dialog.
    */
    wxDialog        *GetAboutDialog      ( wxWindow *parent = NULL );
    /**
        Gets the main frame.
    */
    wxFrame         *GetMainFrame        ( wxWindow *parent = NULL );
    /**
        Gets the main menu bar.
    */
    wxMenuBar       *GetMainMenu         ( wxWindow *parent = NULL );
    /**
        Gets the visual designer editor panel.
    */
    wxPanel         *GetDesignPanel();
    /**
        Gets the visual editor notebook.
    */
    wxNotebook      *GetEditorBook       ( wxWindow *parent = NULL );
    /**
        Gets the 'Properties' notebook.
    */
    wxNotebook      *GetPropertyBook     ( wxWindow *parent = NULL );
    /**
        Gets the 'Palette' notebook.
    */
    wxNotebook      *GetPaletteBook      ( wxWindow *parent = NULL );
    /**
        Gets the 'Project' tree control.
    */
    wxTreeCtrl      *GetTreeView         ( wxWindow *parent = NULL );
    /**
        Gets the main tool bar.
    */
    wxToolBar       *GetToolBar          ( wxWindow *parent = NULL );
    /**
        Gets the 'Properties' property grid.
    */
    wxPropertyGrid  *GetPropertiesGrid();
    /**
        Gets the 'Events' property grid.
    */
    wxPropertyGrid  *GetEventsGrid();
    /*
        Create a new project object.

    void NewProject();
    */
    /**
        Gets the static instance of this class.
    */
    static GUIManager *Get();
    /**
        Free resources used by this class and destroy it.
    */
    void Free();
};
