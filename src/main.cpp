///////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "main.h"

#include <core/manager.h>

#include <wx/config.h>
#include <wx/stdpaths.h>
#include <wx/frame.h>

#ifdef __WXMSW__
    #include <wx/sysopt.h>
#endif

wxIMPLEMENT_APP( wxGUIDesignerApp );

int wxGUIDesignerApp::OnRun()
{
    wxApp::SetAppDisplayName("wxGUIDesigner");

    delete wxConfigBase::Set( new wxConfig("wxGUIDesigner") );

#ifdef __WXMSW__
    wxSystemOptions::SetOption( "msw.remap", 0 );
    wxSystemOptions::SetOption( "msw.staticbox.optimized-paint", 0 );
#endif

    wxYield();

    m_frame = wxGUIDesigner::Get()->GetMainFrame( NULL );

    if ( !m_frame )
    {
        wxLogError(_("Error while loading the main frame.") );
        return 1;
    }

    wxSetWorkingDirectory( wxStandardPaths::Get().GetDataDir() );

    m_frame->Show( true );
    SetTopWindow( m_frame );

    wxGUIDesigner::Get()->NewProject();

    return wxApp::OnRun();
}

bool wxGUIDesignerApp::OnInit()
{
    return true;
}

int wxGUIDesignerApp::OnExit()
{
    return wxApp::OnExit();
}

wxGUIDesignerApp::~wxGUIDesignerApp()
{
}
