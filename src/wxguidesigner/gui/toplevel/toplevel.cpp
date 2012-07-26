///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/toplevel.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/01
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/gdicmn.h>
#include <wx/iconbndl.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/gui/toplevel/toplevel.h"

IMPLEMENT_DYNAMIC_CLASS( TopLevelWindow, wxPanel )

TopLevelWindow::TopLevelWindow()
{
    Init();
}

TopLevelWindow::TopLevelWindow( wxWindow *parent, wxWindowID id,
                                const wxString& title, const wxPoint& pos,
                                const wxSize& size, long style,
                                const wxString& name )
{
    Init();
    Create( parent, id, title, pos, size, style, name );
}

TopLevelWindow::~TopLevelWindow()
{
}

void TopLevelWindow::Init()
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

wxIcon TopLevelWindow::GetIcon() const
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
