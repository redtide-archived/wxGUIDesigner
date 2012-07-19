///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/debugwindow.cpp
// Purpose:     
// Author:      Jefferson González
// Modified by:
// Created:     2012/06/07
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/textctrl.h>
#include <wx/tokenzr.h>

#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/debugwindow.h"

wxGDDebugWindow::wxGDDebugWindow( wxGDHandler *handler, wxWindow *parent )
:
wxTextCtrl( parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                    wxTE_MULTILINE | wxTE_READONLY | wxTE_LEFT ),
m_handler( handler )
{
    Bind( wxEVT_COMMAND_TEXT_UPDATED, &wxGDDebugWindow::OnTextUpdated, this );
}

wxGDDebugWindow::~wxGDDebugWindow()
{
}

void wxGDDebugWindow::OnTextUpdated( wxCommandEvent &event )
{
    wxString msg = event.GetString();

    wxArrayString lines = wxStringTokenize( msg, "\n" );
    for( size_t i = 0; i < lines.GetCount(); i++ )
    {
        if( lines.Item(i).Contains(_("Error: ") ) )
            SetDefaultStyle( wxTextAttr( wxColour(210,0,0) ) );
        else if( lines.Item(i).Contains(_("Warning: ") ) )
            SetDefaultStyle( wxTextAttr( wxColour(255,150,0) ) );
        else
            SetDefaultStyle( wxTextAttr( wxColour(0,150,0) ) );
    }
}
