///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/auidockart.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/06/03
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/dc.h>
#include <wx/settings.h>

#include "wxguidesigner/gui/auidockart.h"

wxGDAUIDockArt::wxGDAUIDockArt()
{
}

wxGDAUIDockArt::~wxGDAUIDockArt()
{
}

wxString wxAuiChopText( wxDC &dc, const wxString &text, int maxSize )
{
    wxCoord x,y;

    // First check if the text fits with no problems
    dc.GetTextExtent( text, &x, &y );
    if( x <= maxSize )
        return text;

    size_t i, len = text.Length();
    size_t lastGoodLength = 0;
    for( i = 0; i < len; ++i )
    {
        wxString s = text.Left(i);
        s += "...";

        dc.GetTextExtent( s, &x, &y );
        if ( x > maxSize )
            break;

        lastGoodLength = i;
    }

    wxString ret = text.Left( lastGoodLength );
    ret += "...";
    return ret;
}

bool wxGDAUIDockArt::IsDark( const wxColour &colour ) const
{
    int average = ( colour.Red() + colour.Green() + colour.Blue() ) / 3;
    if ( average < 127 )
        return true;

    return false;
}

void wxGDAUIDockArt::DrawCaption( wxDC &dc, wxWindow *, const wxString &text,
                                    const wxRect &rect, wxAuiPaneInfo &pane )
{
/*
    // Black glossy gradient
    topStart    = wxColour( 150,150,150 );
    topEnd      = wxColour( 90,90,90 );
    bottomStart = *wxBLACK;
    bottomEnd   = wxColour( 60,60,60 );
*/
// Draw caption glossy gradient
    wxColour topStart, topEnd, bottomStart, bottomEnd; int r, g, b;

    topEnd = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION );

    r = topEnd.Red()   + 45; if( r > 255 ) r = 225;
    g = topEnd.Green() + 45; if( g > 255 ) g = 225;
    b = topEnd.Blue()  + 45; if( b > 255)  b = 225;

    topStart = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );

    r = topEnd.Red()   - 75; if( r < 0 ) r = 15;
    g = topEnd.Green() - 75; if( g < 0 ) g = 15;
    b = topEnd.Blue()  - 75; if( b < 0 ) b = 15;

    bottomStart = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );

    r = topEnd.Red()   - 15; if( r < 0 ) r = 30;
    g = topEnd.Green() - 15; if( g < 0 ) g = 30;
    b = topEnd.Blue()  - 15; if( b < 0 ) b = 30;

    bottomEnd = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );

    wxRect gradientRect = rect;

    gradientRect.SetHeight( gradientRect.GetHeight() / 2 );
    dc.GradientFillLinear( gradientRect, topStart, topEnd, wxSOUTH);
    gradientRect.Offset( 0, gradientRect.GetHeight() );
    dc.GradientFillLinear( gradientRect, bottomStart, bottomEnd, wxSOUTH );
/*
    // Draw the caption border
    // TODO: in some circumstances this draws weird artifacts on the titlebar,
    // like small rectangles, could be a refresh problem?
    dc.SetPen( wxPen( bottomStart ) );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRectangle( 1, 1, rect.GetWidth()-1, rect.GetHeight()-1 );
*/
// Draw Icon
    int captionOffset = 0;
    if ( pane.icon.IsOk() )
    {
        DrawIcon( dc, rect, pane );
        captionOffset += pane.icon.GetWidth() + 3;
    }

// Draw caption label
    wxCoord w, h;
    wxColour labelColour = IsDark( bottomStart ) ? *wxWHITE : *wxBLACK;
    dc.GetTextExtent( "ABCDEFHXfgkj", &w, &h );
    dc.SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
    dc.SetTextForeground( labelColour );

    wxRect clipRect = rect;
    clipRect.width -= 3; // Text offset
    clipRect.width -= 2; // Button padding

    if( pane.HasCloseButton() )
        clipRect.width -= m_buttonSize;

    if( pane.HasPinButton() )
        clipRect.width -= m_buttonSize;

    if( pane.HasMaximizeButton() )
        clipRect.width -= m_buttonSize;

    wxString drawText = wxAuiChopText( dc, text, clipRect.width );

    dc.SetClippingRegion( clipRect );
    dc.DrawText( drawText, rect.x + 3 + captionOffset,
                            rect.y + ( rect.height / 2 ) - ( h / 2 ) -1 );
    dc.DestroyClippingRegion();
}
