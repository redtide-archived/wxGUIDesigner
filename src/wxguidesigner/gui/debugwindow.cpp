///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/debugwindow.cpp
// Purpose:     
// Author:      Jefferson González
// Modified by:
// Created:     2012/06/07
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/textctrl.h>

#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/debugwindow.h"

wxGDDebugWindow::wxGDDebugWindow( wxGDHandler *handler, wxWindow *parent )
:
wxTextCtrl( parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                    wxTE_MULTILINE | wxTE_READONLY | wxTE_LEFT ),
m_handler( handler )
{
}

wxGDDebugWindow::~wxGDDebugWindow()
{
}

void wxGDDebugWindow::AddMessage(const wxString& message)
{
    SetDefaultStyle( wxTextAttr( wxColour( 88,184,53 ) ) );
    AppendText(_("Ok: "));

    SetDefaultStyle( wxTextAttr( wxColour( *wxBLACK ) ) );
    AppendText( message + "\n" );
}

void wxGDDebugWindow::AddWarning(const wxString& message)
{
    SetDefaultStyle( wxTextAttr( wxColour( 223,212,29 ) ) );
    AppendText(_("Warning: "));

    SetDefaultStyle( wxTextAttr( wxColour( *wxBLACK ) ) );
    AppendText( message + "\n" );
}

void wxGDDebugWindow::AddError(const wxString& message)
{
    SetDefaultStyle( wxTextAttr( *wxRED ) );
    AppendText(_("Error: "));

    SetDefaultStyle( wxTextAttr( wxColour( *wxBLACK ) ) );
    AppendText( message + "\n" );
}
