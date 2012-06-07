///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/debugwindow.cpp
// Purpose:     
// Author:      Jefferson González
// Modified by:
// Created:     2012/06/07
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "textlogctrl.h"

//
// Default Constructor and Destructor
//
TextLogCtrl::TextLogCtrl(wxWindow* parent)
:wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_LEFT)
{
    //ctor
}

TextLogCtrl::~TextLogCtrl()
{
    //dtor
}

//
// Public methods
//

void TextLogCtrl::AddMessage(const wxString& message)
{
    SetDefaultStyle(wxTextAttr(wxColour(88, 184, 53)));
    AppendText(_("Ok: "));

    SetDefaultStyle(wxTextAttr(wxColour(*wxBLACK)));
    AppendText(message + "\n");
}

void TextLogCtrl::AddWarning(const wxString& message)
{
    SetDefaultStyle(wxTextAttr(wxColour(223, 212, 29)));
    AppendText(_("Warning: "));

    SetDefaultStyle(wxTextAttr(wxColour(*wxBLACK)));
    AppendText(message + "\n");
}

void TextLogCtrl::AddError(const wxString& message)
{
    SetDefaultStyle(wxTextAttr(*wxRED));
    AppendText(_("Error: "));

    SetDefaultStyle(wxTextAttr(wxColour(*wxBLACK)));
    AppendText(message + "\n");
}