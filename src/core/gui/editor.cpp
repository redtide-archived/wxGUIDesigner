///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/editor.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/30
// Revision:    $Hash$
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

#include "core/events.h"
#include "core/utils.h"
#include "core/object/tree.h"
#include "core/gui/handler.h"
#include "core/gui/designer.h"
#include "core/gui/glossybutton.h"
#include "core/gui/editor.h"

wxGDEditorBook::wxGDEditorBook( wxGDHandler *handler, wxWindow *parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler( handler )
{
    SetOwnBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
//-----------------------------------------------------------------------------
// Scrolled window, the main visual editor page
//-----------------------------------------------------------------------------
    // Working area, block event propagation to our application
    m_scrolled = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxHSCROLL | wxVSCROLL |
                                                        wxWS_EX_BLOCK_EVENTS );
    m_scrolled->SetScrollRate( 5, 5 );
    m_scrolled->SetBackgroundColour( wxColour( 192, 192, 192 ) );

    // 'Invisible' resize area, where to catch and handle user mouse events
    m_resizer = new wxGDResizingPanel( m_scrolled );
    m_resizer->SetBackgroundColour( wxColour( 192, 192, 192 ) );

    wxBoxSizer* reSizer = new wxBoxSizer( wxVERTICAL );
    reSizer->SetMinSize( wxSize(-1,90) ); 

    m_designer = new wxPanel( m_resizer, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxBORDER_NONE );

    wxBoxSizer* designerSizer = new wxBoxSizer( wxVERTICAL );

    m_title = new wxGDTitleBarPanel( m_designer );

    wxBoxSizer* titleSizer = new wxBoxSizer( wxHORIZONTAL );

    wxBitmap bmp = wxXmlResource::Get()->LoadBitmap("icon_msw");

    m_titleBmp = new wxStaticBitmap( m_title, wxID_ANY, bmp );

    titleSizer->Add( m_titleBmp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );

    m_titleLbl = new wxStaticText( m_title, wxID_ANY, wxT("Title") );
    m_titleLbl->Wrap( -1 );

    wxColour colour = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION );
    int r = colour.Red()   - 75; if( r < 0 ) r = 15;
    int g = colour.Green() - 75; if( g < 0 ) g = 15;
    int b = colour.Blue()  - 75; if( b < 0 ) b = 15;
    wxColour caption = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );

    wxColour label = wxGDConv::IsDark( caption ) ? *wxWHITE : *wxBLACK;
    m_titleLbl->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
    m_titleLbl->SetForegroundColour( label );

    titleSizer->Add( m_titleLbl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 3 );

    wxBoxSizer* titleBtnSizer;
    titleBtnSizer = new wxBoxSizer( wxHORIZONTAL );

    bmp = wxXmlResource::Get()->LoadBitmap("minimize");

    m_titleBmpMin = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMin );

    bmp = wxXmlResource::Get()->LoadBitmap("maximize");

    m_titleBmpMax = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMax );

    bmp = wxArtProvider::GetBitmap( wxART_CLOSE );

    m_titleBmpClose = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpClose );

    titleSizer->Add( titleBtnSizer, 0, wxALL, 3 );

    m_title->SetSizer( titleSizer );
    m_title->Layout();
    titleSizer->Fit( m_title );
    designerSizer->Add( m_title, 0, wxEXPAND, 0 );

    // The client area, this is our 'GetClientSize()' except when has scrollbars
    m_client = new wxPanel( m_designer );

    designerSizer->Add( m_client, 1, wxEXPAND, 0 );

    m_designer->SetSizer( designerSizer );
    m_designer->Layout();
    designerSizer->Fit( m_designer );
    reSizer->Add( m_designer, 1, wxBOTTOM | wxRIGHT | wxEXPAND, 3 );

    m_resizer->SetSizer( reSizer );
    m_resizer->Layout();
    reSizer->Fit( m_resizer );

    wxImageList *imageList = new wxImageList(16,16);
    AssignImageList( imageList );

    bmp = wxXmlResource::Get()->LoadBitmap("designer");
    int imgIndex = -1;
    if( bmp.IsOk() )
        imgIndex = imageList->Add( bmp );

    AddPage( m_scrolled, _("Designer"), true, imgIndex );

    Bind( wxGD_EVT_OBJECT_CREATED,  &wxGDEditorBook::OnObjectCreated,  this );
    Bind( wxGD_EVT_OBJECT_SELECTED, &wxGDEditorBook::OnObjectSelected, this );
}

wxGDEditorBook::~wxGDEditorBook()
{
    
}

void wxGDEditorBook::OnObjectCreated( wxGDObjectEvent &event )
{
    // TODO: Update the text

}

void wxGDEditorBook::OnObjectDeleted( wxGDObjectEvent &event )
{
    
}

void wxGDEditorBook::OnObjectExpanded( wxGDObjectEvent &event )
{
    
}

void wxGDEditorBook::OnObjectSelected( wxGDObjectEvent &event )
{
    // 
}
