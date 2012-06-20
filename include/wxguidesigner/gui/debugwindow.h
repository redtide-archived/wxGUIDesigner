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

#endif //__WXGUIDESIGNER_GUI_DEBUGWINDOW_H__