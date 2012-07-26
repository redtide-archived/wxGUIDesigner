///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/editor/designer.cpp
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
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/xml/xml.h>
#include <wx/xrc/xmlres.h>

#include <wx/log.h>

#include "wxguidesigner/rtti/tree.h"

#include "wxguidesigner/gui/utils/draw.h"
#include "wxguidesigner/gui/editor/glossybutton.h"
#include "wxguidesigner/gui/editor/designer.h"
//=============================================================================
// wxGDTitleBar
//=============================================================================
IMPLEMENT_DYNAMIC_CLASS( wxGDTitleBar, wxPanel )

void wxGDTitleBar::Init()
{
    m_titleLbl      = NULL;
    m_titleBmp      = NULL;
    m_titleBmpMin   = NULL;
    m_titleBmpMax   = NULL;
    m_titleBmpClose = NULL;
}

wxGDTitleBar::wxGDTitleBar()
{
    Init();
}

wxGDTitleBar::wxGDTitleBar( wxWindow *parent, wxWindowID id, const wxPoint &position,
                    const wxSize &size, long style, const wxString &name )
{
    Init();

    Create( parent, id, position, size, style, name );
}

bool wxGDTitleBar::Create ( wxWindow *parent, wxWindowID id, const wxPoint &pos,
                        const wxSize &size, long style, const wxString &name )
{
    if( !wxPanel::Create( parent, id, pos, size, style, name ) )
        return false;

    wxBoxSizer* titleSizer = new wxBoxSizer( wxHORIZONTAL );

    // Titlebar bitmap
    wxBitmap bmp = wxXmlResource::Get()->LoadBitmap("icon_msw");
    m_titleBmp   = new wxStaticBitmap( this, wxID_ANY, bmp );
    titleSizer->Add( m_titleBmp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );

    // Titlebar label
    m_titleLbl = new wxStaticText( this, wxID_ANY, wxT("Title") );
    m_titleLbl->Wrap( -1 );

    // Titlebar appearance
    wxColour colour = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION );
    int r = colour.Red()   - 75; if( r < 0 ) r = 15;
    int g = colour.Green() - 75; if( g < 0 ) g = 15;
    int b = colour.Blue()  - 75; if( b < 0 ) b = 15;
    wxColour caption = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );
    // TODO: wxRendererNative for wxMAC and wxMSW

    // Titlebar label color, depending if background dark or bright
    wxColour label = wxGDDraw::IsDark( caption ) ? *wxWHITE : *wxBLACK;
    m_titleLbl->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
    m_titleLbl->SetForegroundColour( label );
    titleSizer->Add( m_titleLbl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 3 );

    // Titlebar buttons
    wxBoxSizer* titleBtnSizer = new wxBoxSizer( wxHORIZONTAL );

    // Minimize button
    bmp = wxXmlResource::Get()->LoadBitmap("minimize");
    m_titleBmpMin = new wxGlossyButton( this, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMin );

    // Maximize button
    bmp = wxXmlResource::Get()->LoadBitmap("maximize");
    m_titleBmpMax = new wxGlossyButton( this, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMax );

    // Close button
    bmp = wxArtProvider::GetBitmap( wxART_CLOSE );
    m_titleBmpClose = new wxGlossyButton( this, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpClose );

    titleSizer->Add( titleBtnSizer, 0, wxALL, 3 );
    this->SetSizer( titleSizer );
    this->Layout();
    titleSizer->Fit( this );

    Bind( wxEVT_PAINT, &wxGDTitleBar::OnPaint, this );

    return true;
}

wxGDTitleBar::~wxGDTitleBar()
{
}

void wxGDTitleBar::SetTitleLabel( const wxString &label )
{
    
}

void wxGDTitleBar::OnPaint( wxPaintEvent & )
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
    wxCoord x = rect.GetWidth()  - 24;
    wxCoord y = rect.GetHeight() - 11;

    dc.DrawBitmap( wxArtProvider::GetBitmap(wxART_CLOSE, wxART_OTHER,
                    wxSize(22,22) ),
                        x, y, true );
}
//=============================================================================
// wxGDResizingPanel
//=============================================================================
wxGDResizingPanel::wxGDResizingPanel( wxGDEditor *parent )
:
wxPanel( parent, wxID_ANY, wxPoint( 9,9 ), wxDefaultSize, wxFULL_REPAINT_ON_RESIZE ),
m_sizing        ( NONE ),
m_curX          ( -1 ),
m_curY          ( -1 ),
m_resizeBorder  ( 9 ),
m_minSize       ( -1, 24 ),
m_baseMinSize   ( -1, 24 ),
m_editor        ( parent )
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
    {
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

        m_editor->UpdateVirtualSize();
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
        m_curX = m_curY = -1;
    }
}
//=============================================================================
// wxGDEditor
//=============================================================================
wxGDEditor::wxGDEditor( wxWindow *parent )
:
wxScrolled< wxPanel >( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                        wxHSCROLL | wxVSCROLL | wxWS_EX_BLOCK_EVENTS ),
m_border    ( NULL ),
m_client    ( NULL ),
m_designer  ( NULL ),
m_resizer   ( NULL )
{
    SetScrollRate( 5, 5 );
    SetBackgroundColour( wxColour( 192, 192, 192 ) );
//-----------------------------------------------------------------------------
// 'Invisible' resize area, where to catch and handle user mouse events
//-----------------------------------------------------------------------------
    m_resizer = new wxGDResizingPanel( this );
    m_resizer->SetBackgroundColour( wxColour( 192, 192, 192 ) );
//-----------------------------------------------------------------------------
// The main designer
//-----------------------------------------------------------------------------
    m_designer = new wxPanel( m_resizer, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxBORDER_NONE );
//-----------------------------------------------------------------------------
// The 'fake TopLevelWindow' titlebar
//-----------------------------------------------------------------------------
    m_title = new wxGDTitleBar( m_designer );
    m_title->Hide();
//-----------------------------------------------------------------------------
// The 'client' area, 'GetClientSize()' except when has scrollbars
//-----------------------------------------------------------------------------
    m_client = new wxPanel( m_designer );

    wxBoxSizer* designerSizer = new wxBoxSizer( wxVERTICAL );
    designerSizer->Add( m_title, 0, wxEXPAND );
    designerSizer->Add( m_client, 1, wxEXPAND );

    m_designer->SetSizer( designerSizer );
    m_designer->Layout();
    designerSizer->Fit( m_designer );

    wxBoxSizer* reSizer = new wxBoxSizer( wxVERTICAL );
    reSizer->Add( m_designer, 1, wxBOTTOM | wxRIGHT | wxEXPAND, 3 );

    m_resizer->SetSizer( reSizer );
    m_resizer->Layout();
    reSizer->Fit( m_resizer );
}

wxGDEditor::~wxGDEditor()
{
}

void wxGDEditor::Cleanup()
{
    m_client->DestroyChildren();
}

void wxGDEditor::ShowTitleBar( bool show )
{
    m_title->Show( show );
}

void wxGDEditor::UpdateVirtualSize()
{
    // Update the scrolled window virtual size to show scroll bars
    wxSize size = m_resizer->GetSize();
    SetVirtualSize( size.GetX() + 15, size.GetY() + 15 );
}

void wxGDEditor::UpdateDesigner(const wxString &topClassName,
                                const wxString &topName)
{
    Cleanup();

    wxObject *wxobject =
    wxXmlResource::Get()->LoadObject( m_client, topName, topClassName );

    if( !wxobject )
        return;

    wxWindow *wxwindow = wxDynamicCast( wxobject, wxWindow );
    if( !wxwindow )
        return;

    // SetupWindow( wxwindow );

    ShowTitleBar();

    wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( wxwindow, 1, wxEXPAND );

    m_client->SetSizer( sizer );
    m_client->Layout();

    m_resizer->SetSize( wxSize( 300,300 ) );
    m_resizer->Layout();

    UpdateVirtualSize();
}
