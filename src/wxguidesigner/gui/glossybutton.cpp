///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/glossybutton.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/03
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/dcbuffer.h>
#include <wx/log.h>

#include "wxguidesigner/gui/glossybutton.h"
#include "wxguidesigner/gui/utils/draw.h"

void DrawUtils::DrawGlossyGradient( wxDC &dc, wxRect &rect, wxColour &topStart,
                                    wxColour &bottomStart,  wxColour &bottomEnd,
                                    wxColour &colour, bool hover )
{
    wxColour topEnd = colour;

    int r = colour.Red()   + 45; if( r > 255 ) r = 225;
    int g = colour.Green() + 45; if( g > 255 ) g = 225;
    int b = colour.Blue()  + 45; if( b > 255)  b = 225;

    topStart = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );

    r = colour.Red()   - 75; if( r < 0 ) r = 15;
    g = colour.Green() - 75; if( g < 0 ) g = 15;
    b = colour.Blue()  - 75; if( b < 0 ) b = 15;

    bottomStart = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );

    r = colour.Red()   - 15; if( r < 0 ) r = 30;
    g = colour.Green() - 15; if( g < 0 ) g = 30;
    b = colour.Blue()  - 15; if( b < 0 ) b = 30;

    bottomEnd =  wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );

    rect.SetHeight( rect.GetHeight() / 2 );

    dc.GradientFillLinear( rect, topStart, topEnd, wxSOUTH );

    rect.Offset( 0, rect.GetHeight() );

    if( hover )
        dc.GradientFillLinear( rect, bottomStart, topStart, wxSOUTH );
    else
        dc.GradientFillLinear( rect, bottomStart, topEnd, wxSOUTH );
}

void DrawUtils::DrawBitmap( wxDC &dc, const wxBitmap &bitmap,
                            const wxRect &rect, const wxString &text )
{
    if( bitmap.IsOk() )
    {
        wxCoord x = rect.GetX() + 3;
        wxCoord y = (rect.GetHeight() - bitmap.GetHeight()) / 2;

        if( text.empty() )
            x = (rect.GetWidth() - bitmap.GetWidth()) / 2;

        dc.DrawBitmap( bitmap, x, y, true );
    }
}

IMPLEMENT_DYNAMIC_CLASS( wxGlossyButton, wxWindow )

BEGIN_EVENT_TABLE( wxGlossyButton, wxWindow )
    EVT_PAINT           ( wxGlossyButton::OnPaint           )
    EVT_ERASE_BACKGROUND( wxGlossyButton::OnEraseBackground )
    EVT_LEFT_DOWN       ( wxGlossyButton::OnLeftDown        )
    EVT_LEFT_DCLICK     ( wxGlossyButton::OnLeftDown        )
    EVT_LEFT_UP         ( wxGlossyButton::OnLeftUp          )
    EVT_ENTER_WINDOW    ( wxGlossyButton::OnHover           )
    EVT_LEAVE_WINDOW    ( wxGlossyButton::OnHover           )
END_EVENT_TABLE()

void wxGlossyButton::Init()
{
    m_topEnd = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION );
    m_topStart      =
    m_bottomStart   =
    m_bottomEnd     =
    m_topPressed    =
    m_bottomPressed = *wxBLACK;
    m_hover         = false;
}

wxSize wxGlossyButton::DoGetBestSize() const
{
    wxSize labelSize = wxDefaultSize;
    GetTextExtent( m_label, &labelSize.x, &labelSize.y );
    return wxSize( wxMax(40, labelSize.x + 20), wxMax(20, labelSize.y + 10) );
}
//=============================================================================
// Event Handlers
//=============================================================================
void wxGlossyButton::OnPaint( wxPaintEvent &event )
{
    wxBufferedPaintDC dc( this );
    wxColour background = m_topEnd;
    wxColour foreground = GetForegroundColour();
    wxRect   clientRect = GetClientRect();
    wxRect gradientRect = clientRect;

    bool captured = ( GetCapture() == this );

    if( !captured )
    {
        // Update defaults also when changing the current theme at runtime
        m_topEnd = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION );

        foreground  = wxGDDraw::IsDark( m_topEnd ) ? *wxWHITE : *wxBLACK;

        SetBackgroundColour( m_topEnd );
        SetForegroundColour( foreground );
    }
    else
    {
        gradientRect.SetHeight( gradientRect.GetHeight() + 1 );

#if wxVERSION_NUMBER < 2900
        int r = m_topEnd.Red()   * 0.75;
        int g = m_topEnd.Green() * 0.75;
        int b = m_topEnd.Blue()  * 0.75;

        m_topEnd.Set( r, g, b );
#else
        m_topEnd = m_topEnd.ChangeLightness( 75 );
#endif
    }

    DrawUtils::DrawGlossyGradient( dc, gradientRect, m_topStart, m_bottomStart,
                                        m_bottomEnd, m_topEnd, m_hover );
    dc.SetPen( wxPen( background ) );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRectangle( 0, 0, clientRect.GetWidth(), clientRect.GetHeight() );
    dc.SetFont( GetFont() );
    dc.SetTextForeground( foreground );

    if( GetCapture() == this )
        clientRect.Offset( 1,1 );

    DrawUtils::DrawBitmap( dc, m_bitmap, clientRect, m_label );

    dc.DrawLabel( m_label, clientRect,
                    wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL );
}

void wxGlossyButton::OnHover( wxMouseEvent &event )
{
    m_hover = !m_hover;

    wxClientDC dc( this );
    wxRect rect = GetClientRect();
    wxRect grad = rect;

    DrawUtils::DrawGlossyGradient( dc, grad, m_topStart, m_bottomStart,
                                    m_bottomEnd, m_topEnd, m_hover );

    if( GetCapture() == this )
        rect.Offset( 1,1 );

    DrawUtils::DrawBitmap( dc, m_bitmap, rect, m_label );

    dc.DrawLabel( m_label, rect,
                    wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL );
    Refresh();
}

void wxGlossyButton::OnLeftDown( wxMouseEvent &event )
{
    if( GetCapture() != this )
    {
        CaptureMouse();
        Refresh();
    }
}

void wxGlossyButton::OnLeftUp( wxMouseEvent &event )
{
    if( GetCapture() == this )
    {
        ReleaseMouse();
        Refresh();

        if( GetClientRect().Contains( event.GetPosition() ) )
        {
            wxCommandEvent evt( wxEVT_COMMAND_BUTTON_CLICKED, GetId() );
            GetEventHandler()->AddPendingEvent( evt );
        }
    }
}

void wxGlossyButton::OnEraseBackground( wxEraseEvent &event )
{
    // Empty to avoid flicker on wxMSW
}
//=============================================================================
// Getters / Setters
//=============================================================================
wxString wxGlossyButton::GetLabel() const
{
    return m_label;
}

void wxGlossyButton::SetLabel( const wxString &label )
{
    m_label = label;
}

wxBitmap wxGlossyButton::GetBitmap() const
{
    return m_bitmap;
}

void wxGlossyButton::SetBitmap( const wxBitmap &bitmap )
{
    m_bitmap = bitmap;
}

wxColour wxGlossyButton::GetGradientColourTopStart() const
{
    return m_topStart;
}

wxColour wxGlossyButton::GetGradientColourTopEnd() const
{
    return m_topEnd;
}

wxColour wxGlossyButton::GetGradientColourBottomStart() const
{
    return m_bottomStart;
}

wxColour wxGlossyButton::GetGradientColourBottomEnd() const
{
    return m_bottomEnd;
}

wxColour wxGlossyButton::GetColourPressedTop() const
{
    return m_topPressed;
}

wxColour wxGlossyButton::GetColourPressedBottom() const
{
    return m_bottomPressed;
}

void wxGlossyButton::SetGradientColourTopStart( const wxColour &colour )
{
    m_topStart = colour;
}

void wxGlossyButton::SetGradientColourTopEnd( const wxColour &colour )
{
    m_topEnd = colour;
}

void wxGlossyButton::SetGradientColourBottomStart( const wxColour &colour )
{
    m_bottomStart = colour;
}

void wxGlossyButton::SetGradientColourBottomEnd( const wxColour &colour )
{
    m_bottomEnd = colour;
}

void wxGlossyButton::SetColourPressedTop( const wxColour &colour )
{
    m_topPressed = colour;
}

void wxGlossyButton::SetColourPressedBottom( const wxColour &colour )
{
    m_bottomPressed = colour;
}
