///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/editor.cpp
// Purpose:     wxGDEditorBook: Visual and code editor wxNotebook impl.
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/30
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/stc/stc.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/events.h"
#include "wxguidesigner/utils.h"
#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/designer.h"
#include "wxguidesigner/gui/glossybutton.h"
#include "wxguidesigner/gui/editor.h"
//=============================================================================
// wxGDEditorBook
//=============================================================================
wxGDEditorBook::wxGDEditorBook( wxGDHandler *handler, wxWindow *parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler( handler )
{
    SetOwnBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
//-----------------------------------------------------------------------------
// Scrolled window, the main visual editor page
//-----------------------------------------------------------------------------
    // Working area, block events propagation to our application
    m_scrolled = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxHSCROLL | wxVSCROLL |
                                                        wxWS_EX_BLOCK_EVENTS );
    m_scrolled->SetScrollRate( 5, 5 );
    m_scrolled->SetBackgroundColour( wxColour( 192, 192, 192 ) );
//-----------------------------------------------------------------------------
// 'Invisible' resize area, where to catch and handle user mouse events
//-----------------------------------------------------------------------------
    m_resizer = new wxGDResizingPanel( m_scrolled );
    m_resizer->SetBackgroundColour( wxColour( 192, 192, 192 ) );
//-----------------------------------------------------------------------------
// The main designer
//-----------------------------------------------------------------------------
    m_designer = new wxPanel( m_resizer, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxBORDER_NONE );
//-----------------------------------------------------------------------------
// The 'fake TopLevelWindow' titlebar
//-----------------------------------------------------------------------------
    m_title = new wxGDTitleBarPanel( m_designer );
    wxBoxSizer* titleSizer = new wxBoxSizer( wxHORIZONTAL );

    // Titlebar bitmap
    wxBitmap bmp = wxXmlResource::Get()->LoadBitmap("icon_msw");
    m_titleBmp   = new wxStaticBitmap( m_title, wxID_ANY, bmp );
    titleSizer->Add( m_titleBmp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );

    // Titlebar label
    m_titleLbl = new wxStaticText( m_title, wxID_ANY, wxT("Title") );
    m_titleLbl->Wrap( -1 );

    // Titlebar appearance
    wxColour colour = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION );
    int r = colour.Red()   - 75; if( r < 0 ) r = 15;
    int g = colour.Green() - 75; if( g < 0 ) g = 15;
    int b = colour.Blue()  - 75; if( b < 0 ) b = 15;
    wxColour caption = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );
    // TODO: wxRendererNative for wxMAC and wxMSW

    // Titlebar label color, depending if background dark or bright
    wxColour label = wxGDConv::IsDark( caption ) ? *wxWHITE : *wxBLACK;
    m_titleLbl->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
    m_titleLbl->SetForegroundColour( label );
    titleSizer->Add( m_titleLbl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 3 );

    // Titlebar buttons
    wxBoxSizer* titleBtnSizer = new wxBoxSizer( wxHORIZONTAL );

    // Minimize button
    bmp = wxXmlResource::Get()->LoadBitmap("minimize");
    m_titleBmpMin = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMin );

    // Maximize button
    bmp = wxXmlResource::Get()->LoadBitmap("maximize");
    m_titleBmpMax = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMax );

    // Close button
    bmp = wxArtProvider::GetBitmap( wxART_CLOSE );
    m_titleBmpClose = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpClose );

    titleSizer->Add( titleBtnSizer, 0, wxALL, 3 );
    m_title->SetSizer( titleSizer );
    m_title->Layout();
    titleSizer->Fit( m_title );

    wxBoxSizer* designerSizer = new wxBoxSizer( wxVERTICAL );
    designerSizer->Add( m_title, 0, wxEXPAND );
//-----------------------------------------------------------------------------
// The client area, this is our 'GetClientSize()' except when has scrollbars
//-----------------------------------------------------------------------------
    m_client = new wxPanel( m_designer );

    designerSizer->Add( m_client, 1, wxEXPAND );
    m_designer->SetSizer( designerSizer );
    m_designer->Layout();
    designerSizer->Fit( m_designer );

    wxBoxSizer* reSizer = new wxBoxSizer( wxVERTICAL );
    reSizer->Add( m_designer, 1, wxBOTTOM | wxRIGHT | wxEXPAND, 3 );
    m_resizer->SetSizer( reSizer );
    m_resizer->Layout();
    reSizer->Fit( m_resizer );

    // Editor imagelist
    wxImageList *imageList = new wxImageList( 16,16 );
    AssignImageList( imageList );

    bmp = wxXmlResource::Get()->LoadBitmap("designer");
    int imgIndex = -1;
    if( bmp.IsOk() )
        imgIndex = imageList->Add( bmp );

    AddPage( m_scrolled, _("Designer"), true, imgIndex );

    m_designer->Bind( wxEVT_SIZE, &wxGDEditorBook::OnDesignerResize, this );

    Bind( wxGD_EVT_OBJECT_CREATED,  &wxGDEditorBook::OnObjectCreated,  this );
    Bind( wxGD_EVT_OBJECT_SELECTED, &wxGDEditorBook::OnObjectSelected, this );
}

wxGDEditorBook::~wxGDEditorBook()
{
}

void wxGDEditorBook::OnDesignerResize( wxSizeEvent &event )
{
    // Update the scrolled window virtual size to show scroll bars
    wxSize size = m_resizer->GetSize();
    m_scrolled->SetVirtualSize( size.GetX() + 15, size.GetY() + 15 );
    event.Skip();
}

void wxGDEditorBook::OnObjectCreated( wxGDObjectEvent &event )
{
    // TODO: Update generated code
}

void wxGDEditorBook::OnObjectDeleted( wxGDObjectEvent &event )
{
    // TODO: Clear generated code
}

void wxGDEditorBook::OnObjectSelected( wxGDObjectEvent &event )
{
    // TODO: Draw the coloured rectangle
}
