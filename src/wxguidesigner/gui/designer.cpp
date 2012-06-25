///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/designer.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/01
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
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

#include "wxguidesigner/gui/designer.h"
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
