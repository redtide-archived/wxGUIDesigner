///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/manager.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/01/13
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_MANAGER_H__
#define __WXGUIDESIGNER_MANAGER_H__

#include "wxguidesigner/dllimpexp.h"

#include <boost/tr1/memory.hpp>

#include "wxguidesigner/ipc.h"

class wxDialog;
class wxMenuBar;
class wxNotebook;
class wxFrame;
class wxPanel;
class wxPropertyGrid;
class wxStyledTextCtrl;
class wxToolBar;
class wxTreeCtrl;

namespace wxGD
{
class Handler;
}

class DLLIMPEXP_WXGUIDESIGNER wxGUIDesigner
{
public:
    static wxGUIDesigner *Get();
    static void Free();

    // Handler
    wxFrame         *GetMainFrame        ( wxWindow *parent = NULL );
    wxDialog        *GetAboutDialog      ( wxWindow *parent );
    wxNotebook      *GetEditorBook       ( wxWindow *parent );
    wxNotebook      *GetPropertyBook     ( wxWindow *parent );
    wxNotebook      *GetPaletteBook      ( wxWindow *parent );
    wxTreeCtrl      *GetTreeView         ( wxWindow *parent );
    wxToolBar       *GetToolBar          ( wxWindow *parent );
    wxPropertyGrid  *GetPropertiesGrid();
    wxPropertyGrid  *GetEventsGrid();
//  wxStyledTextCtrl *GetEditor( wxWindow *parent, const wxString &name );

    // Project
    void        NewProject();
    bool        LoadProject( const wxString &filePath, bool check = true );
    bool        SaveProject( const wxString &filePath );
    wxString    GetProjectFileName() const  { return m_currPrj; }
    wxString    GetProjectFilePath() const  { return m_currDir; }
    bool        IsProjectModified()         { return m_isChanged; }
    bool        CheckSingleInstance( const wxString &filePath,
                                        bool switchTo = true );
    void        GenerateCode();

private:
    wxGUIDesigner();
    ~wxGUIDesigner();

    static wxGUIDesigner                *ms_instance;
    wxGD::Handler                       *m_handler;
    std::tr1::shared_ptr< wxGD::IPCFile > m_ipcFile;
    wxString                            m_currPrj;
    wxString                            m_currDir;
    bool                                m_isChanged;
};

#endif //__WXGUIDESIGNER_MANAGER_H__
