///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/frame.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/20
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_MAINFRAME_H__
#define __CORE_MAINFRAME_H__

#include <wx/filehistory.h>
#include <wx/frame.h>

class GUIManager;

class wxAuiManager;
class wxCloseEvent;
class wxCommandEvent;
class wxString;

class MainFrame : public wxFrame
{
public:
    MainFrame( wxWindow *parent = NULL );
    ~MainFrame() {}

    // Events
    void OnClose( wxCloseEvent &event );

    void LoadLayout();
    void SaveLayout();
    bool SaveWarning();

    // MainMenu and ToolBar
    void OnAbout( wxCommandEvent &event );
    void OnExit( wxCommandEvent & );

    // Project
    void OnNewProject( wxCommandEvent &event );
    void OnOpenProject( wxCommandEvent &event );
    void OnSaveProject( wxCommandEvent &event );

private:
    friend class GUIManager;
    wxAuiManager    *m_mgr;
    wxFileHistory   m_history;
    wxString        m_lastDir;
};

#endif //__CORE_MAINFRAME_H__
