///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/debugwindow.h
// Purpose:     
// Author:      Jefferson González
// Modified by:
// Created:     2012/06/07
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGD_GUI_DEBUGWINDOW_H__
#define __WXGD_GUI_DEBUGWINDOW_H__

class wxTextCtrl;
class wxGDHandler;

class wxGDDebugWindow : public wxTextCtrl
{
public:
    wxGDDebugWindow( wxGDHandler *handler, wxWindow *parent );
    virtual ~wxGDDebugWindow();

    void AddMessage ( const wxString &message );
    void AddWarning ( const wxString &message );
    void AddError   ( const wxString &message );

private:
    wxGDHandler *m_handler;
};

#endif //__WXGD_GUI_DEBUGWINDOW_H__
