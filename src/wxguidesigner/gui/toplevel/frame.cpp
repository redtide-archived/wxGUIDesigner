///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/frame.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/05
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>

#include "wxguidesigner/gui/toplevel/toplevel.h"
#include "wxguidesigner/gui/toplevel/frame.h"

IMPLEMENT_DYNAMIC_CLASS( Frame, TopLevelWindow )

bool Frame::Create( wxWindow *parent, wxWindowID id, const wxString& title,
                    const wxPoint& pos, const wxSize& size, long style,
                    const wxString& name )
{
    if( !TopLevelWindow::Create( parent, id, title, pos, size, style, name ) )
        return false;

    return true;
}
