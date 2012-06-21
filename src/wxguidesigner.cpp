///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner.cpp
// Purpose:     Main application
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/app.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include <wx/frame.h>

#ifdef __WXMSW__
    #include <wx/sysopt.h>
#endif

#include <wxguidesigner/manager.h>

class wxGUIDesignerApp : public wxApp
{
public:
    wxGUIDesignerApp() {}
    ~wxGUIDesignerApp() {}

private:
    bool  OnInit();
    int   OnRun();
    int   OnExit();
};

wxDECLARE_APP   ( wxGUIDesignerApp );
wxIMPLEMENT_APP ( wxGUIDesignerApp );

int wxGUIDesignerApp::OnRun()
{
    wxApp::SetAppDisplayName("wxGUIDesigner");
    wxSetWorkingDirectory( wxStandardPaths::Get().GetDataDir() );
    delete wxConfigBase::Set( new wxConfig( GetAppDisplayName() ) );

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

//  wxConfigBase::Set( NULL );

    return wxApp::OnExit();
}
