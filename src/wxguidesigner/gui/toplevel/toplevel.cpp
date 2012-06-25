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
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/gdicmn.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/gui/glossybutton.h"
#include "wxguidesigner/gui/utils/draw.h"
#include "wxguidesigner/gui/toplevel/toplevel.h"
//=============================================================================
// TitleBar
//=============================================================================
IMPLEMENT_DYNAMIC_CLASS( TitleBar, wxPanel )

void TitleBar::Init()
{
    m_titleLbl      = NULL;
    m_titleBmp      = NULL;
    m_titleBmpMin   = NULL;
    m_titleBmpMax   = NULL;
    m_titleBmpClose = NULL;
}

TitleBar::TitleBar()
{
    Init();
}

TitleBar::TitleBar( wxWindow *parent, wxWindowID id, const wxPoint &position,
                    const wxSize &size, long style, const wxString &name )
{
    Init();

    Create( parent, id, position, size, style, name );
}

bool TitleBar::Create ( wxWindow *parent, wxWindowID id, const wxPoint &pos,
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

    Bind( wxEVT_PAINT, &TitleBar::OnPaint, this );

    return true;
}

TitleBar::~TitleBar()
{
}

void SetTitleLabel( const wxString &label )
{
    
}

void TitleBar::OnPaint( wxPaintEvent & )
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
// TopLevelWindow
//=============================================================================
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
    m_titleBar = NULL;
    m_clientSizer = NULL;
}

bool TopLevelWindow::Create(wxWindow *parent, wxWindowID id,
                            const wxString& title, const wxPoint& pos,
                            const wxSize& size, long style, const wxString& name )
{
    if( !wxPanel::Create( parent, id, pos, size, style, name ) )
        return false;

    m_titleBar    = new TitleBar( this );
    m_clientSizer = new wxBoxSizer( wxVERTICAL );
    m_title       = title;

    wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( m_titleBar,    0, wxEXPAND );
    sizer->Add( m_clientSizer, 1, wxEXPAND );

    SetSizerAndFit( sizer );

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
