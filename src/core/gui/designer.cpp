///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/designer.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/01
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/dcbuffer.h>
#include <wx/event.h>
#include <wx/panel.h>
#include <wx/renderer.h>
#include <wx/settings.h>

#include <wx/log.h>

#include "core/gui/designer.h"
//=============================================================================
// wxGDTitleBarPanel
//=============================================================================
wxGDTitleBarPanel::wxGDTitleBarPanel( wxWindow *parent, wxWindowID id,
                                        const wxPoint &position,
                                        const wxSize &size, long style )
:
wxPanel( parent, id, position, size, style )
{
    Bind( wxEVT_PAINT, &wxGDTitleBarPanel::OnPaint, this );
}

wxGDTitleBarPanel::~wxGDTitleBarPanel()
{
}

void wxGDTitleBarPanel::OnPaint( wxPaintEvent & )
{
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

    wxBufferedPaintDC dc(this);
    wxRect rect = GetRect();

    rect.SetHeight( rect.GetHeight() / 2 );
    dc.GradientFillLinear( rect, topStart, topEnd, wxSOUTH);
    rect.Offset( 0, rect.GetHeight() );
    dc.GradientFillLinear( rect, bottomStart, bottomEnd, wxSOUTH );

    dc.SetPen( wxPen( bottomStart ) );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRectangle( 0, 0, rect.GetWidth(), rect.GetHeight() );

    dc.SetPen( wxPen( *wxWHITE ) );
    wxCoord x = rect.GetWidth() - 24;
    wxCoord y = rect.GetHeight() - 11;

    dc.DrawBitmap( wxArtProvider::GetBitmap(wxART_CLOSE, wxART_OTHER, wxSize(22,22) ),
                    x, y, true );
}
//=============================================================================
// wxGDResizingPanel
//=============================================================================
wxGDResizingPanel::wxGDResizingPanel( wxWindow *parent, wxWindowID id,
                                        const wxPoint &position,
                                        const wxSize &size, long style )
:
wxPanel( parent, id, position, size, style ),
m_sizing(NONE),
m_curX(-1),
m_curY(-1),
m_resizeBorder(9),
m_minSize(-1, 24),
m_baseMinSize(-1, 24)
{
    m_minSize.x += 9;
    m_minSize.y += 9;

    Bind( wxEVT_MOTION,     &wxGDResizingPanel::OnMouseMotion, this );
    Bind( wxEVT_LEFT_DOWN,  &wxGDResizingPanel::OnLeftDown,    this );
    Bind( wxEVT_LEFT_UP,    &wxGDResizingPanel::OnLeftUp,      this );
}

wxGDResizingPanel::~wxGDResizingPanel()
{
}

void wxGDResizingPanel::OnMouseMotion( wxMouseEvent &event )
{
    if ( m_sizing != NONE )
    {/*
        wxScreenDC dc;
        wxPen pen( *wxBLACK, 1, wxDOT );

        dc.SetPen( pen );
        dc.SetBrush( *wxTRANSPARENT_BRUSH );
        dc.SetLogicalFunction( wxINVERT );

        wxPoint pos = GetParent()->ClientToScreen( GetPosition() );

        if ( m_curX >= 0 && m_curY >= 0 )
            dc.DrawRectangle( pos.x, pos.y, m_curX, m_curY );
*/
        if ( m_sizing == RIGHT || m_sizing == RIGHTBOTTOM )
            m_curX = event.GetX() + m_difX;
        else
            m_curX = GetSize().x;

        if ( m_sizing == BOTTOM || m_sizing == RIGHTBOTTOM )
            m_curY = event.GetY() + m_difY;
        else
            m_curY = GetSize().y;

        // User min size
        wxSize minSize = GetMinSize();
        if ( m_curX < minSize.x ) m_curX = minSize.x;
        if ( m_curY < minSize.y ) m_curY = minSize.y;

        // Internal min size
        if ( m_curX < m_minSize.x ) m_curX = m_minSize.x;
        if ( m_curY < m_minSize.y ) m_curY = m_minSize.y;

        wxSize maxSize = GetMaxSize();
        if ( m_curX > maxSize.x && maxSize.x != wxDefaultCoord ) m_curX = maxSize.x;
        if ( m_curY > maxSize.y && maxSize.y != wxDefaultCoord ) m_curY = maxSize.y;
/*
        dc.DrawRectangle( pos.x, pos.y, m_curX, m_curY );

        dc.SetLogicalFunction( wxCOPY );
        dc.SetPen( wxNullPen );
        dc.SetBrush( wxNullBrush );*/
    }

    else
    {
        int x, y;
        GetClientSize( &x, &y );

        if ( ( event.GetX() >= x - m_resizeBorder && event.GetY() >= y - m_resizeBorder ) ||
                ( event.GetX() < m_resizeBorder && event.GetY() < m_resizeBorder ) )
        {
            SetCursor( wxCursor( wxCURSOR_SIZENWSE ) );
        }
        else if ( ( event.GetX() < m_resizeBorder && event.GetY() >= y - m_resizeBorder ) ||
                  ( event.GetX() > x - m_resizeBorder && event.GetY() < m_resizeBorder ) )
        {
            SetCursor( wxCursor( wxCURSOR_SIZENESW ) );
        }
        else if ( event.GetX() >= x - m_resizeBorder || event.GetX() < m_resizeBorder )
        {
            SetCursor( wxCursor( wxCURSOR_SIZEWE ) );
        }
        else if ( event.GetY() >= y - m_resizeBorder || event.GetY() < m_resizeBorder )
        {
            SetCursor( wxCursor( wxCURSOR_SIZENS ) );
        }
        else
        {
            SetCursor( *wxSTANDARD_CURSOR );
        }

//      m_titleBar->SetCursor( *wxSTANDARD_CURSOR );
//      m_frameContent->SetCursor( *wxSTANDARD_CURSOR );
    }
            SetSize( m_curX, m_curY );
}

void wxGDResizingPanel::OnLeftDown( wxMouseEvent &event )
{
    if ( m_sizing == NONE )
    {
        if ( event.GetX() >= GetSize().x - m_resizeBorder && event.GetY() >= GetSize().y - m_resizeBorder )
            m_sizing = RIGHTBOTTOM;
        else if ( event.GetX() >= GetSize().x - m_resizeBorder )
            m_sizing = RIGHT;
        else if ( event.GetY() >= GetSize().y - m_resizeBorder )
            m_sizing = BOTTOM;

        if ( m_sizing != NONE )
        {
            m_difX = GetSize().x - event.GetX();
            m_difY = GetSize().y - event.GetY();
            CaptureMouse();
            OnMouseMotion( event );
        }
    }
}

void wxGDResizingPanel::OnLeftUp( wxMouseEvent & )
{
    if ( m_sizing != NONE )
    {
        m_sizing = NONE;
        ReleaseMouse();
/*
        wxScreenDC dc;
        wxPen pen( *wxBLACK, 1, wxDOT );

        dc.SetPen( pen );
        dc.SetBrush( *wxTRANSPARENT_BRUSH );
        dc.SetLogicalFunction( wxINVERT );

        wxPoint pos = GetParent()->ClientToScreen( GetPosition() );

        dc.DrawRectangle( pos.x, pos.y, m_curX, m_curY );

        dc.SetLogicalFunction( wxCOPY );
        dc.SetPen( wxNullPen );
        dc.SetBrush( wxNullBrush );

        SetSize( m_curX, m_curY );
*/
        m_curX = m_curY = -1;
    }
}
