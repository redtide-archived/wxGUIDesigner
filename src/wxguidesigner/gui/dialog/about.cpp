///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/dialog/about.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/07/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/html/htmlwin.h>
#include <wx/hyperlink.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/gui/dialog/about.h"

wxGD::Dialog::About::About( wxWindow* parent )
:
wxDialog( parent, wxID_ANY, _("About") + " wxGUIDesigner",
            wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE )
{
    wxBoxSizer          *mainSizer  = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer          *logoSizer  = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer          *descSizer  = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer          *aboutSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer          *crditSizer = new wxBoxSizer( wxVERTICAL );
    wxFlexGridSizer     *fgSizer    = new wxFlexGridSizer( 0, 2, 0, 0 );
    wxStaticBoxSizer    *linksSizer = new wxStaticBoxSizer( new wxStaticBox(
                                    this, wxID_ANY, wxEmptyString ), wxVERTICAL );

    wxNotebook  *book       = new wxNotebook( this, wxID_ANY );
    wxPanel     *aboutPage  = new wxPanel ( book, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxTAB_TRAVERSAL );
    wxPanel     *creditPage = new wxPanel ( book, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxTAB_TRAVERSAL );
    aboutPage->SetBackgroundColour( *wxWHITE );
    aboutPage->SetSizer( aboutSizer );
    aboutPage->Layout();

    creditPage->SetSizer( crditSizer );
    creditPage->Layout();

    book->SetBackgroundColour( *wxWHITE );
    book->AddPage( aboutPage, _("About"), true );
    book->AddPage( creditPage, _("Credits"), false );

    wxPanel *logoPanel = new wxPanel( aboutPage, wxID_ANY, wxDefaultPosition,
                                    wxDefaultSize, wxNO_BORDER | wxTAB_TRAVERSAL );

    logoPanel->SetBackgroundColour( *wxWHITE );
    logoPanel->SetSizer( logoSizer );
    logoPanel->Layout();

    wxPanel *descPanel = new wxPanel( aboutPage, wxID_ANY, wxDefaultPosition,
                                    wxDefaultSize, wxNO_BORDER | wxTAB_TRAVERSAL );

    descPanel->SetBackgroundColour( *wxWHITE );
    descPanel->SetSizer( descSizer );
    descPanel->Layout();

    wxBitmap bmpLogo = wxXmlResource::Get()->LoadBitmap("logo");

    wxStaticBitmap *logo = new wxStaticBitmap( logoPanel, wxID_ANY, bmpLogo );

    wxStaticText *logoLabel = new wxStaticText( logoPanel, wxID_ANY,
                                                "wxGUIDesigner" );
    logoLabel->SetFont( wxFont( 21, wxFONTFAMILY_DEFAULT ) );

    wxStaticText *descLabel = new wxStaticText( descPanel, wxID_ANY,
                                        _("An Open Source GUI designer for") );

    wxHyperlinkCtrl *wxLink = new wxHyperlinkCtrl( descPanel, wxID_ANY,
                                                "wxWidgets", "wxwidgets.org",
                                                wxDefaultPosition,
                                                wxDefaultSize,
                                                wxHL_DEFAULT_STYLE );
    wxLink->SetBackgroundColour( *wxWHITE );

    wxHyperlinkCtrl *homeLink = new wxHyperlinkCtrl( this, wxID_ANY,
                                                "wxGUIDesigner",
                                                "wxguidesigner.github.com",
                                                wxDefaultPosition,
                                                wxDefaultSize,
                                                wxHL_DEFAULT_STYLE );

    wxHyperlinkCtrl *forumLink = new wxHyperlinkCtrl( this, wxID_ANY, _("Forum"),
                                "sourceforge.net/projects/wxguidesigner/forums",
                                                wxDefaultPosition,
                                                wxDefaultSize,
                                                wxHL_DEFAULT_STYLE );

    wxHtmlWindow *htmDevs = new wxHtmlWindow( aboutPage, wxID_ANY,
                                                wxDefaultPosition,
                                                wxSize( -1,120 ),
                                                wxHW_NO_SELECTION |
                                                wxHW_SCROLLBAR_NEVER );

    wxHtmlWindow *htmCredits = new wxHtmlWindow( creditPage, wxID_ANY,
                                                wxDefaultPosition, wxDefaultSize,
                                                wxHW_SCROLLBAR_AUTO );

    wxButton *okButton = new wxButton( this, wxID_OK );
    okButton->SetDefault(); 

    linksSizer->Add( fgSizer, 1, wxEXPAND, 5 );

    fgSizer->SetFlexibleDirection( wxBOTH );
    fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    fgSizer->Add( homeLink, 0, wxTOP, 5 );
    fgSizer->Add( forumLink, 0, wxRIGHT|wxTOP, 5 );

    crditSizer->Add( htmCredits, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
    crditSizer->Fit( creditPage );

    aboutSizer->Add( logoPanel, 0, wxALIGN_CENTER, 5 );
    aboutSizer->Add( descPanel, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
    aboutSizer->Add( htmDevs,   1, wxEXPAND | wxFIXED_MINSIZE, 5 );
    aboutSizer->Fit( aboutPage );

    descSizer->Add( descLabel,  0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 3 );
    descSizer->Add( wxLink,     0, wxALIGN_CENTER_VERTICAL, 5 );
    descSizer->Fit( descPanel );

    logoSizer->Add( logo,       0, wxALIGN_CENTER_VERTICAL | wxFIXED_MINSIZE |
                                    wxBOTTOM | wxRIGHT | wxTOP, 5 );
    logoSizer->Add( logoLabel,  0, wxALIGN_CENTER_VERTICAL |
                                    wxALIGN_CENTER_HORIZONTAL, 5 );
    logoSizer->Fit( logoPanel );

    mainSizer->Add( book,       1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5 );
    mainSizer->Add( linksSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5 );
    mainSizer->Add( okButton,   0, wxBOTTOM | wxALIGN_BOTTOM |
                                    wxALIGN_CENTER_HORIZONTAL, 5 );
    mainSizer->Fit( this );

    this->SetSizer( mainSizer );
    this->Layout();
    this->Centre( wxBOTH );
}

wxGD::Dialog::About::~About()
{
}
