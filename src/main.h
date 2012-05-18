///////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGD_APP_H__
#define __WXGD_APP_H__

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/log.h>

class MainFrame;

class wxGUIDesignerApp : public wxApp
{
public:
      bool  OnInit();
      int   OnRun();
      int   OnExit();
      ~wxGUIDesignerApp();

private:
      wxFrame *m_frame;
};

wxDECLARE_APP( wxGUIDesignerApp );

#endif //__WXGD_APP_H__
