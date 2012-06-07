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

#include <wx/textctrl.h>

class TextLogCtrl : public wxTextCtrl
{
public:
    TextLogCtrl(wxWindow* parent);
    virtual ~TextLogCtrl();

    void AddMessage(const wxString& message);
    void AddWarning(const wxString& message);
    void AddError(const wxString& message);
};

#endif //__WXGD_GUI_DEBUGWINDOW_H__
