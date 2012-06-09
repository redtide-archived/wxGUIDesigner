///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/toplevel.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/01
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/gui/toplevel/toplevel.h"

TopLevelWindow::~TopLevelWindow()
{
}

bool TopLevelWindow::Create(wxWindow *parent, wxWindowID id,
                            const wxString& title, const wxPoint& pos,
                            const wxSize& size, long style, const wxString& name )
{
    if( !wxPanel::Create( parent, id, pos, size, style, name ) )
        return false;

    m_title = title;

    return true;
}

wxIcon wxTopLevelWindowBase::GetIcon() const
{
    return m_icons.IsEmpty() ? wxIcon() : m_icons.GetIcon( -1 );
}

void TopLevelWindow::SetIcon( const wxIcon &icon )
{
    // passing wxNullIcon to SetIcon() is possible (it means that we shouldn't
    // have any icon), but adding an invalid icon to wxIconBundle is not
    wxIconBundle icons;
    if ( icon.IsOk() )
        icons.AddIcon(icon);

    SetIcons(icons);
}

void TopLevelWindow::SetIcons( const wxIconBundle& icons )
{
    m_icons = icons;
}
