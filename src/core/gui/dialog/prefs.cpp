///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/dialog/prefs.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/07
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/bmpcbox.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/imaglist.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/treebook.h>
#include <wx/xrc/xmlres.h>

#include "core/gui/dialog/prefs.h"
#include "core/settings.h"
#include "core/gui/handler.h"
//=============================================================================
// wxGDDialogPrefs
//=============================================================================
wxGDDialogPrefs::wxGDDialogPrefs( wxGDHandler *handler, wxWindow *parent )
:
wxDialog( parent, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER ),
m_handler   ( handler ),
m_apply     ( NULL ),
m_ok        ( NULL ),
m_cancel    ( NULL ),
m_btnSizer  ( NULL ),
m_tbkPrefs  ( NULL )
{
    wxSize minSize = wxSize( 510,360 );
    SetSizeHints( minSize, wxDefaultSize );

    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    mainSizer->SetMinSize( minSize );
 
    wxImageList *imageList = new wxImageList( 16, 16 );
    m_tbkPrefs             = new wxTreebook( this, wxID_ANY  );
    m_tbkPrefs->AssignImageList( imageList );

    wxBitmap bmpLocale  = wxXmlResource::Get()->LoadBitmap("language");
    wxBitmap bmpDesign  = wxXmlResource::Get()->LoadBitmap("designer");

    int imgIndex = -1;
    if( bmpLocale.IsOk() )
        imgIndex = imageList->Add( bmpLocale );
    m_tbkPrefs->AddPage( new wxGDPageLocale( m_handler, this ), _("Locale"), true, imgIndex );

    imgIndex = -1;
    if( bmpDesign.IsOk() )
        imgIndex = imageList->Add( bmpDesign );
    m_tbkPrefs->AddPage( new wxPanel( this, wxID_ANY ), _("GUI"), false, imgIndex );

    mainSizer->Add( m_tbkPrefs, 1, wxALL|wxEXPAND, 5 );

    m_apply  = new wxButton( this, wxID_APPLY );
    m_ok     = new wxButton( this, wxID_OK );
    m_cancel = new wxButton( this, wxID_CANCEL );

    m_btnSizer = new wxStdDialogButtonSizer();
    m_btnSizer->AddButton( m_ok );
    m_btnSizer->AddButton( m_apply );
    m_btnSizer->AddButton( m_cancel );
    m_btnSizer->Realize();

    mainSizer->Add( m_btnSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 5 );
    SetSizer( mainSizer );
    Layout();
    mainSizer->Fit( this );
    Centre( wxBOTH );

    Bind( wxEVT_UPDATE_UI, &wxGDDialogPrefs::OnUpdateUI, this );
}

wxGDDialogPrefs::~wxGDDialogPrefs()
{
    Unbind( wxEVT_UPDATE_UI, &wxGDDialogPrefs::OnUpdateUI, this );
}

void wxGDDialogPrefs::OnUpdateUI( wxUpdateUIEvent &event )
{

    for( size_t n = 0; n < m_tbkPrefs->GetPageCount(); n++ )
    {
        if( event.GetEventObject() == m_tbkPrefs->GetPage(n) )
        {
            m_apply->Enable( event.GetInt() );
            m_ok->Enable( event.GetInt() );
            break;
        }
    }

    event.Skip();
}
//=============================================================================
// wxGDPageLocale
//=============================================================================
wxGDPageLocale::wxGDPageLocale( wxGDHandler *handler, wxGDDialogPrefs *parent )
:
wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL ),
m_handler   ( handler ),
m_bcbLang   ( NULL ),
m_chkLang   ( NULL ),
m_pnlLocale ( NULL ),
m_lblLang   ( NULL )
{
    wxBoxSizer       *localeSizer = new wxBoxSizer( wxVERTICAL );
    wxStaticBoxSizer *sbsLocale   = new wxStaticBoxSizer(
                                    new wxStaticBox( this, wxID_ANY,
                                    _("Locale") ), wxVERTICAL );

    m_pnlLocale = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxTAB_TRAVERSAL );
    m_lblLang   = new wxStaticText( m_pnlLocale, wxID_ANY, _("Locale to use:") );

    m_chkLang   = new wxCheckBox( m_pnlLocale, wxID_ANY, _("Enable Localization"),
                                        wxDefaultPosition, wxDefaultSize, 0 );
    m_bcbLang   = new wxBitmapComboBox( m_pnlLocale, wxID_ANY, wxEmptyString,
                                      wxDefaultPosition, wxDefaultSize,
                                                    0, NULL, wxCB_READONLY );
    wxGDSettings settings = m_handler->GetSettings();
    m_bcbLang->SetSelection( settings->GetInt("language",         0) );
    m_chkLang->SetValue    ( settings->GetInt("language/enabled", 0) );
    m_bcbLang->Enable      ( settings->GetInt("language/enabled", 0) );

    wxBitmap bmp        = wxNullBitmap;
    wxBitmap bmpDefault = wxXmlResource::Get()->LoadBitmap("default");

    if( bmpDefault.IsOk() )
        bmp = bmpDefault;

    m_bcbLang->Append( _("System Default"), bmp );

    sbsLocale->Add( m_pnlLocale, 1, wxEXPAND, 5 );
    localeSizer->Add( sbsLocale, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    m_bcbLang->SetToolTip( _("Select language to use") );
    m_chkLang->SetToolTip( _("Enable / Disable wxGUIDesigner localization") );
    m_lblLang->Wrap( -1 );

    wxBoxSizer* sizerLang = new wxBoxSizer( wxVERTICAL );
    sizerLang->Add( m_lblLang, 0, wxALIGN_CENTER_VERTICAL, 5 );
    sizerLang->Add( m_bcbLang, 1, wxALIGN_RIGHT | wxALL | wxEXPAND, 5 );

    wxBoxSizer* pnlSizer = new wxBoxSizer( wxVERTICAL );
    pnlSizer->Add( m_chkLang,   0, wxEXPAND | wxLEFT | wxTOP, 5 );
    pnlSizer->Add( sizerLang, 0, wxEXPAND, 5 );

    m_pnlLocale->SetSizer( pnlSizer );
    m_pnlLocale->Layout();
    pnlSizer->Fit( m_pnlLocale );

    SetSizer( localeSizer );
    Layout();
    localeSizer->Fit( this );

    Bind( wxEVT_UPDATE_UI, &wxGDPageLocale::OnUpdateUI, this );
    m_chkLang->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        &wxGDPageLocale::OnToggleLocale, this );
}

wxGDPageLocale::~wxGDPageLocale()
{
    Unbind( wxEVT_UPDATE_UI, &wxGDPageLocale::OnUpdateUI, this );
    m_chkLang->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                            &wxGDPageLocale::OnToggleLocale, this );
}

void wxGDPageLocale::OnUpdateUI( wxUpdateUIEvent& event )
{
    
}

void wxGDPageLocale::OnToggleLocale( wxCommandEvent& event )
{
    
}
