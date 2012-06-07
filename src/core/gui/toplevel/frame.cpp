///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/toplevel/frame.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/05
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/panel.h>

#include "core/gui/toplevel/toplevel.h"
#include "core/gui/toplevel/frame.h"

bool Frame::Create( wxWindow *parent, wxWindowID id, const wxString& title,
                    const wxPoint& pos, const wxSize& size, long style,
                    const wxString& name )
{
    if( !TopLevelWindow::Create( parent, id, title, pos, size, style, name ) )
        return false;

    return true;
}
