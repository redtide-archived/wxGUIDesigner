///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/mainframe.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_MAINFRAME_H__
#define __WXGDCORE_MAINFRAME_H__

#include <wx/event.h>
#include <wx/frame.h>

class FrameHandler
{
public:
    FrameHandler( wxFrame *owner );

    // MainFrame
    void OnClose( wxCloseEvent &event );

    void SaveLayout();
    bool SaveWarning();

    // MainMenu and ToolBar
    void OnAbout( wxCommandEvent &event );
    void OnExit( wxCommandEvent & );
    void OnNewProject( wxCommandEvent & );

private:
    wxFrame      *m_frame;
};

#endif //__WXGDCORE_MAINFRAME_H__
