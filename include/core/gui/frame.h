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
#ifndef __WXGD_FRAME_H__
#define __WXGD_FRAME_H__

#include <wx/aui/framemanager.h>
#include <wx/filehistory.h>
#include <wx/frame.h>

class wxGDHandler;
//class wxFrame;
class wxWindow;
class wxPanel;
class wxCloseEvent;
class wxCommandEvent;
class wxString;

class wxGDFrame : public wxFrame
{
public:
    wxGDFrame( wxGDHandler *handler, wxWindow *parent = NULL );
    ~wxGDFrame();

    wxAuiManager *GetAUIManager() const { return m_mgr; }
    wxPanel      *GetAUIPanel()   const { return m_panel; }

    void LoadLayout();
    void SaveLayout();
    bool SaveWarning();

    void OnClose        ( wxCloseEvent   &event );
    void OnAbout        ( wxCommandEvent &event );
    void OnPreferences  ( wxCommandEvent &event );
    void OnExit         ( wxCommandEvent &event );

    // Project
    void OnNewProject   ( wxCommandEvent &event );
    void OnOpenProject  ( wxCommandEvent &event );
    void OnOpenRecent   ( wxCommandEvent &event );
    void OnSaveAsProject( wxCommandEvent &event );
    void OnSaveProject  ( wxCommandEvent &event );

    // Undo/Redo
    void OnUndo         ( wxCommandEvent &event );
    void OnRedo         ( wxCommandEvent &event );

    // Clipboard
    void OnCut          ( wxCommandEvent &event );
    void OnCopy         ( wxCommandEvent &event );
    void OnPaste        ( wxCommandEvent &event );
    void OnDelete       ( wxCommandEvent &event );

    // CodeGenerator
    void OnGenerateCode ( wxCommandEvent &event );

    // Widget's alignment
    void OnAlignLeft    ( wxCommandEvent &event );
    void OnAlignCenterH ( wxCommandEvent &event );
    void OnAlignRight   ( wxCommandEvent &event );
    void OnAlignTop     ( wxCommandEvent &event );
    void OnAlignCenterV ( wxCommandEvent &event );
    void OnAlignBottom  ( wxCommandEvent &event );

    void OnExpand       ( wxCommandEvent &event );
    void OnStretch      ( wxCommandEvent &event );

    // Widget's borders
    void OnBorderLeft   ( wxCommandEvent &event );
    void OnBorderRight  ( wxCommandEvent &event );
    void OnBorderTop    ( wxCommandEvent &event );
    void OnBorderBottom ( wxCommandEvent &event );

private:
    wxGDHandler     *m_handler;
    wxPanel         *m_panel;
    wxAuiManager    *m_mgr;
    wxFileHistory   m_history;
    wxString        m_lastDir;
};

#endif //__WXGD_FRAME_H__
