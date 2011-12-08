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
      bool OnInit();

#if wxUSE_ON_FATAL_EXCEPTION && wxUSE_STACKWALKER
      void OnFatalException();
#endif
      int OnRun();
      int OnExit();
      ~wxGUIDesignerApp();
/*
#ifdef __WXMAC__
      wxString m_mac_file_name;
      void MacOpenFile(const wxString &fileName);
#endif
*/
private:
      wxFrame *m_frame;
};

DECLARE_APP( wxGUIDesignerApp )

#endif //__WXGD_APP_H__
