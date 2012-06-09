///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/manager.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/01/13
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/dialog.h>
#include <wx/filefn.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/stc/stc.h>
#include <wx/toolbar.h>
#include <wx/treectrl.h>

#include "wxguidesigner/interfaces/iobject.h"
//#include "wxguidesigner/codegenerator/codegenerator.h"
#include "wxguidesigner/ipc.h"
#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/flags.h"
#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/manager.h"

wxGUIDesigner::wxGUIDesigner()
:
IGUIDesigner(),
m_handler( new wxGDHandler ),
m_ipcFile( new IPCFile ),
m_currPrj(),
m_currDir(),
m_isChanged( false )
{
    wxFlagsManager::Get();
    ClassInfoDB::Get();
}

wxGUIDesigner::~wxGUIDesigner()
{
    delete m_handler;
    ClassInfoDB::Free();
    wxFlagsManager::Free();
}

wxGUIDesigner *wxGUIDesigner::ms_instance = NULL;

wxGUIDesigner *wxGUIDesigner::Get()
{
    if( !ms_instance )
        ms_instance = new wxGUIDesigner;

    return ms_instance;
}

void wxGUIDesigner::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void wxGUIDesigner::NewProject()
{
    m_currDir = wxEmptyString;
    m_currPrj = wxEmptyString;
    m_ipcFile->Reset();
}

bool wxGUIDesigner::LoadProject( const wxString &filePath, bool checkInstance )
{
    if( !wxFileName::FileExists( filePath ) )
    {
        return false;
    }

    if( checkInstance && !CheckSingleInstance( filePath ) )
        return false;

    if( m_handler->GetTree()->Load( filePath ) )
    {
        m_currDir = wxPathOnly( filePath );
        m_currPrj = filePath;
        return true;
    }

    return false;
}

bool wxGUIDesigner::SaveProject( const wxString &filePath )
{
    if( !m_ipcFile->CheckSingleInstance( filePath, false ) )
    {
        wxString msg =  _("You cannot save over a file that is currently open in another instance.\n") +
                        _("Would you like to switch to that instance?");

        int answer = wxMessageBox( msg, _("Open in Another Instance"),
                                    wxICON_QUESTION | wxYES_NO,
                                    wxTheApp->GetTopWindow() );
        if( answer == wxYES )
            m_ipcFile->CheckSingleInstance( filePath, true );

        return false;
    }

    if( m_handler->GetTree()->Serialize( filePath ) )
    {
        m_currDir = wxPathOnly( filePath );
        m_currPrj = filePath;
        return true;
    }

    return false;
}

wxFrame *wxGUIDesigner::GetMainFrame( wxWindow *parent )
{
    return m_handler->GetMainFrame( parent );
}

wxDialog *wxGUIDesigner::GetAboutDialog( wxWindow *parent )
{
    return m_handler->GetAboutDialog( parent );
}
wxPanel *wxGUIDesigner::GetDesignPanel()
{
    return m_handler->GetDesignPanel();
}
wxNotebook *wxGUIDesigner::GetEditorBook( wxWindow *parent )
{
    return m_handler->GetEditorBook( parent );
}
wxNotebook *wxGUIDesigner::GetPropertyBook( wxWindow *parent )
{
    return m_handler->GetPropertyBook( parent );
}
wxNotebook *wxGUIDesigner::GetPaletteBook( wxWindow *parent )
{
    return m_handler->GetPaletteBook( parent );
}
wxTreeCtrl *wxGUIDesigner::GetTreeView( wxWindow *parent )
{
    return m_handler->GetTreeView( parent );
}
wxToolBar *wxGUIDesigner::GetToolBar( wxWindow *parent )
{
    return m_handler->GetToolBar( parent );
}
wxStyledTextCtrl *wxGUIDesigner::GetEditor( wxWindow *parent, const wxString &name )
{
    return m_handler->GetEditor( parent, name );
}

bool wxGUIDesigner::CheckSingleInstance( const wxString &filePath, bool switchTo )
{
    return m_ipcFile->CheckSingleInstance( filePath, switchTo );
}

void wxGUIDesigner::GenerateCode()
{
    // CodeGenerator::Get()->Generate();
}
