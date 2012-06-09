///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner.cpp
// Purpose:     Main application
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/20
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/app.h>
#include <wx/stdpaths.h>
#include <wx/frame.h>

#ifdef __WXMSW__
    #include <wx/sysopt.h>
#endif

#include <wxguidesigner/manager.h>

class wxGUIDesignerApp : public wxApp
{
public:
      bool  OnInit();
      int   OnRun();
      int   OnExit();
      ~wxGUIDesignerApp();
};

wxDECLARE_APP   ( wxGUIDesignerApp );
wxIMPLEMENT_APP ( wxGUIDesignerApp );

int wxGUIDesignerApp::OnRun()
{
    wxApp::SetAppDisplayName("wxGUIDesigner");

#ifdef __WXMSW__
    wxSystemOptions::SetOption( "msw.remap", 0 );
    wxSystemOptions::SetOption( "msw.staticbox.optimized-paint", 0 );
#endif

    wxYield();

    wxFrame *frame = wxGUIDesigner::Get()->GetMainFrame();

    if( !frame )
    {
        wxLogError(_("Error while loading the main frame.") );
        return 1;
    }

    wxSetWorkingDirectory( wxStandardPaths::Get().GetDataDir() );

    frame->Show( true );
    SetTopWindow( frame );

    wxGUIDesigner::Get()->NewProject();

    return wxApp::OnRun();
}

bool wxGUIDesignerApp::OnInit()
{
    return true;
}

int wxGUIDesignerApp::OnExit()
{
    wxGUIDesigner::Free();
    return wxApp::OnExit();
}

wxGUIDesignerApp::~wxGUIDesignerApp()
{
}
