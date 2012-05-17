///////////////////////////////////////////////////////////////////////////////
// Name:        core/manager.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/01/13
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <wx/filefn.h>
#include <wx/frame.h>
#include <wx/msgdlg.h>

#include "core/manager.h"
#include "core/ipc.h"
#include "core/gui/iconprovider.h"
#include "core/gui/manager.h"
#include "core/object/database.h"
#include "core/object/flags.h"
#include "core/object/tree.h"

wxGUIDesigner::wxGUIDesigner() : m_ipcFile( new IPCFile ), m_currPrj(), m_currDir()
{
    wxFlagsManager::Get();
    ClassInfoDB::Get();
}

wxGUIDesigner::~wxGUIDesigner()
{
    GUIManager::Free();
    ClassInfoDB::Free();
    wxFlagsManager::Free();
}

wxGUIDesigner *wxGUIDesigner::ms_instance = NULL;

wxGUIDesigner *wxGUIDesigner::Get()
{
    if ( !ms_instance )
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
    if ( !wxFileName::FileExists( filePath ) )
    {
        wxLogError(_( "The file '%s' doesn't exists" ), filePath );
        return false;
    }

    if ( checkInstance && !CheckSingleInstance( filePath ) )
        return false;

    if( ObjectTree::Get()->Load( filePath ) )
    {
        m_currDir = wxPathOnly( filePath );
        m_currPrj = filePath;
        return true;
    }

    return false;
}

bool wxGUIDesigner::SaveProject( const wxString &filePath )
{
    if ( !m_ipcFile->CheckSingleInstance( filePath, false ) )
    {
        wxString msg =  _("You cannot save over a file that is currently open in another instance.\n") +
                        _("Would you like to switch to that instance?");

        int answer = wxMessageBox( msg, _("Open in Another Instance"),
                                    wxICON_QUESTION | wxYES_NO,
                                    wxTheApp->GetTopWindow() );
        if ( answer == wxYES )
            m_ipcFile->CheckSingleInstance( filePath, true );

        return false;
    }

    if( ObjectTree::Get()->Serialize( filePath ) )
    {
        m_currDir = wxPathOnly( filePath );
        m_currPrj = filePath;
        return true;
    }

    return false;
}

wxFrame *wxGUIDesigner::GetMainFrame( wxWindow *parent )
{
    return GUIManager::Get()->GetMainFrame( parent );
}

bool wxGUIDesigner::CheckSingleInstance( const wxString &filePath, bool switchTo )
{
    return m_ipcFile->CheckSingleInstance( filePath, switchTo );
}
