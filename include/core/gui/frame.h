///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/frame.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_MAINFRAME_H__
#define __WXGDCORE_MAINFRAME_H__

#include <wx/frame.h>

class GUIManager;

class wxAuiManager;
class wxCloseEvent;
class wxCommandEvent;

class MainFrame : public wxFrame
{
public:
    MainFrame( wxWindow *parent = NULL );
    ~MainFrame() {}

    void OnClose( wxCloseEvent &event );

    void LoadLayout();
    void SaveLayout();
    bool SaveWarning();

    // MainMenu and ToolBar
    void OnAbout( wxCommandEvent &event );
    void OnExit( wxCommandEvent & );
    void OnNewProject( wxCommandEvent & );

private:
    friend class GUIManager;
    wxAuiManager *m_mgr;
};

#endif //__WXGDCORE_MAINFRAME_H__
