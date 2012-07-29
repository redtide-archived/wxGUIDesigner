///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/aui/tbarart.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/06/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/bitmap.h>
#include <wx/dc.h>
#include <wx/gdicmn.h>
#include <wx/renderer.h>
#include <wx/aui/auibar.h>
#include <wx/aui/framemanager.h>

#include "wxguidesigner/gui/aui/tbarart.h"

void wxGD::AUIToolBarArt::DrawBackground( wxDC &dc, wxWindow *, const wxRect &rect )
{
    wxColour topEnd      = wxColour( 225,225,225 );
    wxColour topStart    = *wxWHITE;
    wxColour bottomStart = wxColour( 210,210,210 );
    wxColour bottomEnd   = wxColour( 240,240,240 );

    wxRect gradientRect = rect;

    gradientRect.SetHeight( gradientRect.GetHeight() / 2 );
    dc.GradientFillLinear( gradientRect, topStart, topEnd, wxSOUTH );
    gradientRect.Offset( 0, gradientRect.GetHeight() );
    dc.GradientFillLinear( gradientRect, bottomStart, bottomEnd, wxSOUTH );
}

void wxGD::AUIToolBarArt::DrawOverflowButton( wxDC &dc, wxWindow *wnd,
                                            const wxRect &rect, int state )
{
    // TODO: Looks like wxAUI has some bug about keeping the overflow
    // button pressed state, also tooltips should appears under the
    // mouse cursor/current tool item instead to be displayed above
    // the tool item where the tooltip was shown in the first time.

    wxAuiToolBar *tb = wxDynamicCast( wnd, wxAuiToolBar );

    if( tb && !tb->GetToolBarFits() )
    {
        int flags = 0;

        if( state & wxAUI_BUTTON_STATE_HOVER )
            flags = wxCONTROL_CURRENT;

        if( state & wxAUI_BUTTON_STATE_PRESSED )
            flags = flags | wxCONTROL_PRESSED;

        if( flags )
            wxRendererNative::GetDefault().
            DrawPushButton( wnd, dc, rect, flags );

        wxRendererNative::GetDefault().
            DrawDropArrow( wnd, dc, rect, flags );
    }
}
