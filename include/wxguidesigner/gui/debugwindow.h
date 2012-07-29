///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/debugwindow.h
// Purpose:     
// Author:      Jefferson González
// Modified by:
// Created:     2012/06/07
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_DEBUGWINDOW_H__
#define __WXGUIDESIGNER_GUI_DEBUGWINDOW_H__

class wxTextCtrl;
class wxWindow;
class wxString;
class wxCommandEvent;

namespace wxGD
{
class Handler;

class DebugWindow : public wxTextCtrl
{
public:
    DebugWindow( Handler *handler, wxWindow *parent );
    virtual ~DebugWindow();

private:
    void OnTextUpdated( wxCommandEvent &event );

    Handler *m_handler;
};
};

#endif //__WXGUIDESIGNER_GUI_DEBUGWINDOW_H__
