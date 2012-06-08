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
#include <wx/choice.h>
#include <wx/config.h>
#include <wx/dialog.h>
#include <wx/imaglist.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/treebook.h>
#include <wx/xrc/xmlres.h>

#include "core/gui/dialog/prefs.h"
#include "core/settings.h"
#include "core/gui/handler.h"

wxDEFINE_EVENT( wxGD_EVT_GUI_OPTION_CHANGED, wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_GUI_CONFIG_UPDATE,  wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_GUI_CONFIG_SAVE,    wxCommandEvent );
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

    wxBitmap bmpLocale  = wxXmlResource::Get()->LoadBitmap("locale");
//  wxBitmap bmpDesign  = wxXmlResource::Get()->LoadBitmap("designer");
    wxBitmap bmpEditor  = wxXmlResource::Get()->LoadBitmap("editor");

    int imgIndex = -1;
    if( bmpLocale.IsOk() )
        imgIndex = imageList->Add( bmpLocale );
    m_tbkPrefs->AddPage( new wxGDPageLocale( m_handler, this ), _("Locale"), true, imgIndex );
/*
    imgIndex = -1;
    if( bmpDesign.IsOk() )
        imgIndex = imageList->Add( bmpDesign );
    m_tbkPrefs->AddPage( new wxPanel( this, wxID_ANY ), _("GUI"), false, imgIndex );
*/
    imgIndex = -1;
    if( bmpEditor.IsOk() )
        imgIndex = imageList->Add( bmpEditor );
    m_tbkPrefs->AddPage( new wxGDPageEditors( m_handler, this ), _("Editors"), false, imgIndex );

    mainSizer->Add( m_tbkPrefs, 1, wxALL|wxEXPAND, 5 );

    m_apply  = new wxButton( this, wxID_APPLY ); m_apply->Enable( false );
    m_ok     = new wxButton( this, wxID_OK );    m_ok->Enable( false );
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

    Bind( wxGD_EVT_GUI_OPTION_CHANGED, &wxGDDialogPrefs::OnPrefsChanged, this );
    m_ok->Bind( wxEVT_COMMAND_BUTTON_CLICKED,
                                        &wxGDDialogPrefs::OnUpdatePrefs, this );
    m_apply->Bind( wxEVT_COMMAND_BUTTON_CLICKED,
                                        &wxGDDialogPrefs::OnSavePrefs,   this );
}

wxGDDialogPrefs::~wxGDDialogPrefs()
{
    Unbind( wxGD_EVT_GUI_OPTION_CHANGED, &wxGDDialogPrefs::OnPrefsChanged, this );
}

void wxGDDialogPrefs::OnPrefsChanged( wxCommandEvent &event )
{
    m_apply->Enable ( event.GetInt() );
    m_ok->Enable    ( event.GetInt() );
}

void wxGDDialogPrefs::OnUpdatePrefs( wxCommandEvent & )
{
    wxCommandEvent evt( wxGD_EVT_GUI_CONFIG_UPDATE );
    for( size_t i = 0; i < m_tbkPrefs->GetPageCount(); i++ )
    {
        wxWindow *page = m_tbkPrefs->GetPage(i);
        page->GetEventHandler()->ProcessEvent( evt );
    }

    Destroy();
}

void wxGDDialogPrefs::OnSavePrefs( wxCommandEvent & )
{
    wxCommandEvent evt( wxGD_EVT_GUI_CONFIG_SAVE );
    for( size_t i = 0; i < m_tbkPrefs->GetPageCount(); i++ )
    {
        wxWindow *page = m_tbkPrefs->GetPage(i);
        page->GetEventHandler()->ProcessEvent( evt );
    }

    Destroy();
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
m_lblLang   ( NULL ),
m_enabled   ( false ),
m_selected  ( 0 )
{
    wxBoxSizer       *localeSizer = new wxBoxSizer( wxVERTICAL );
    wxStaticBoxSizer *sbsLocale   = new wxStaticBoxSizer(
                                    new wxStaticBox( this, wxID_ANY,
                                    _("Locale") ), wxVERTICAL );

    m_pnlLocale = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxTAB_TRAVERSAL );
    m_lblLang   = new wxStaticText( m_pnlLocale, wxID_ANY, _("Locale to use:") );

    m_chkLang   = new wxCheckBox( m_pnlLocale, wxID_ANY, _("Enable Localization") );

    m_bcbLang   = new wxBitmapComboBox( m_pnlLocale, wxID_ANY, wxEmptyString,
                                        wxDefaultPosition, wxDefaultSize,
                                                    0, NULL, wxCB_READONLY );
    wxBitmap bmp        = wxNullBitmap;
    wxBitmap bmpDefault = wxXmlResource::Get()->LoadBitmap("default");

    if( bmpDefault.IsOk() )
        bmp = bmpDefault;

    m_bcbLang->Append( _("System Default"), bmp );
/*
    wxGDSettings settings = m_handler->GetSettings();
    m_selected = settings->GetInt("locale/selected", 0);
    m_enabled  = settings->GetBool("locale/enabled", false);
*/
    // Load locale config
    wxConfigBase::Get()->Read( "locale/enabled",  &m_enabled,  false );
    wxConfigBase::Get()->Read( "locale/selected", &m_selected, 0    );

    m_bcbLang->SetSelection( m_selected );
    m_bcbLang->Enable      ( m_enabled );
    m_chkLang->SetValue    ( m_enabled );

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

    m_chkLang->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        &wxGDPageLocale::OnPrefsChanged, this );

    m_bcbLang->Bind( wxEVT_COMMAND_COMBOBOX_SELECTED,
                        &wxGDPageLocale::OnPrefsChanged, this );

    Bind( wxGD_EVT_GUI_CONFIG_UPDATE, &wxGDPageLocale::OnUpdatePrefs, this );
    Bind( wxGD_EVT_GUI_CONFIG_SAVE,   &wxGDPageLocale::OnSavePrefs,   this );
}

wxGDPageLocale::~wxGDPageLocale()
{
    m_chkLang->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        &wxGDPageLocale::OnPrefsChanged, this );

    m_bcbLang->Bind( wxEVT_COMMAND_COMBOBOX_SELECTED,
                        &wxGDPageLocale::OnPrefsChanged, this );
}

void wxGDPageLocale::OnPrefsChanged( wxCommandEvent & )
{
    bool isDirty = (m_enabled  != m_chkLang->IsChecked())   ||
                   (m_selected != m_bcbLang->GetSelection());

    m_bcbLang->Enable( m_chkLang->IsChecked() );

    wxCommandEvent evt( wxGD_EVT_GUI_OPTION_CHANGED );
    evt.SetInt( isDirty ? 1 : 0 );
    GetParent()->GetEventHandler()->ProcessEvent( evt );
}

void wxGDPageLocale::OnUpdatePrefs( wxCommandEvent & )
{
    m_enabled  = m_chkLang->IsChecked();
    m_selected = m_bcbLang->GetSelection();
}

void wxGDPageLocale::OnSavePrefs( wxCommandEvent & )
{
    wxConfigBase::Get()->Write( "locale/enabled",  m_chkLang->IsChecked() );
    wxConfigBase::Get()->Write( "locale/selected", m_bcbLang->GetSelection() );
    wxConfigBase::Get()->Flush();
}
//=============================================================================
// wxGDPageEditors
//=============================================================================
wxGDPageEditors::wxGDPageEditors( wxGDHandler *handler, wxGDDialogPrefs *parent )
:
wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL ),
m_handler       ( handler ),
m_bcbEditor     ( NULL ),
m_chkLineNums   ( NULL ),
m_chkGuides     ( NULL ),
m_chkWSpace     ( NULL ),
m_chkEOL        ( NULL ),
m_chkBackspace  ( NULL ),
m_chkUseTabs    ( NULL ),
m_chkTabsIndent ( NULL ),
m_choTabsWidth  ( NULL ),
m_spnCaretW     ( NULL )
{
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* editSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
    editSizer->AddGrowableCol( 1 );
    editSizer->SetFlexibleDirection( wxBOTH );
    editSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    wxStaticText* lblEditor;
    lblEditor = new wxStaticText( this, wxID_ANY, _("Editor:"), wxDefaultPosition, wxDefaultSize, 0 );
    lblEditor->Wrap( -1 );
    editSizer->Add( lblEditor, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5 );

    m_bcbEditor = new wxBitmapComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
    editSizer->Add( m_bcbEditor, 0, wxEXPAND|wxRIGHT|wxTOP, 5 );

    mainSizer->Add( editSizer, 0, wxEXPAND, 5 );

    wxStaticBoxSizer* sbsGuides;
    sbsGuides = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Guides") ), wxVERTICAL );

    wxPanel* pnlGuides;
    pnlGuides = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* fgsGuides;
    fgsGuides = new wxFlexGridSizer( 0, 2, 0, 0 );
    fgsGuides->AddGrowableCol( 1 );
    fgsGuides->SetFlexibleDirection( wxBOTH );
    fgsGuides->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_chkLineNums = new wxCheckBox( pnlGuides, wxID_ANY, _("Show Line Numbers"), wxDefaultPosition, wxDefaultSize, 0 );
    fgsGuides->Add( m_chkLineNums, 0, 0, 5 );

    m_chkGuides = new wxCheckBox( pnlGuides, wxID_ANY, _("Show Indentation Guides"), wxDefaultPosition, wxDefaultSize, 0 );
    fgsGuides->Add( m_chkGuides, 0, 0, 5 );

    m_chkWSpace = new wxCheckBox( pnlGuides, wxID_ANY, _("View Whitespace"), wxDefaultPosition, wxDefaultSize, 0 );
    fgsGuides->Add( m_chkWSpace, 0, 0, 5 );

    m_chkEOL = new wxCheckBox( pnlGuides, wxID_ANY, _("View EOL"), wxDefaultPosition, wxDefaultSize, 0 );
    fgsGuides->Add( m_chkEOL, 0, 0, 5 );

    m_chkBackspace = new wxCheckBox( pnlGuides, wxID_ANY, _("Backspace Unindents"), wxDefaultPosition, wxDefaultSize, 0 );
    fgsGuides->Add( m_chkBackspace, 0, 0, 0 );

    pnlGuides->SetSizer( fgsGuides );
    pnlGuides->Layout();
    fgsGuides->Fit( pnlGuides );
    sbsGuides->Add( pnlGuides, 0, wxEXPAND, 5 );

    mainSizer->Add( sbsGuides, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxStaticBoxSizer* sbsTabs;
    sbsTabs = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tabs") ), wxVERTICAL );

    wxPanel* pnlTabs;
    pnlTabs = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* fgsTabs;
    fgsTabs = new wxFlexGridSizer( 0, 2, 0, 0 );
    fgsTabs->AddGrowableCol( 1 );
    fgsTabs->SetFlexibleDirection( wxBOTH );
    fgsTabs->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_chkUseTabs = new wxCheckBox( pnlTabs, wxID_ANY, _("Use Tabs"), wxDefaultPosition, wxDefaultSize, 0 );
    fgsTabs->Add( m_chkUseTabs, 0, 0, 5 );

    m_chkTabsIndent = new wxCheckBox( pnlTabs, wxID_ANY, _("Tabs Indent"), wxDefaultPosition, wxDefaultSize, 0 );
    fgsTabs->Add( m_chkTabsIndent, 0, wxLEFT, 5 );

    wxStaticText* lblTabsWidth;
    lblTabsWidth = new wxStaticText( pnlTabs, wxID_ANY, _("Tabs Width:"), wxDefaultPosition, wxDefaultSize, 0 );
    lblTabsWidth->Wrap( -1 );
    fgsTabs->Add( lblTabsWidth, 0, wxALIGN_CENTER_VERTICAL, 5 );

    wxString m_choTabsWidthChoices[] = { _("1"), _("2"), _("4"), _("8") };
    int m_choTabsWidthNChoices = sizeof( m_choTabsWidthChoices ) / sizeof( wxString );
    m_choTabsWidth = new wxChoice( pnlTabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choTabsWidthNChoices, m_choTabsWidthChoices, 0 );
    m_choTabsWidth->SetSelection( 2 );
    fgsTabs->Add( m_choTabsWidth, 0, wxEXPAND, 5 );

    pnlTabs->SetSizer( fgsTabs );
    pnlTabs->Layout();
    fgsTabs->Fit( pnlTabs );
    sbsTabs->Add( pnlTabs, 0, wxEXPAND, 0 );

    mainSizer->Add( sbsTabs, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxStaticBoxSizer* sbsMisc;
    sbsMisc = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Misc") ), wxVERTICAL );

    wxPanel* pnlMisc;
    pnlMisc = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* sizerMisc;
    sizerMisc = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* fgsMisc;
    fgsMisc = new wxFlexGridSizer( 0, 2, 0, 0 );
    fgsMisc->AddGrowableCol( 1 );
    fgsMisc->SetFlexibleDirection( wxBOTH );
    fgsMisc->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    wxStaticText* lblCaretW;
    lblCaretW = new wxStaticText( pnlMisc, wxID_ANY, _("Caret Width:"), wxDefaultPosition, wxDefaultSize, 0 );
    lblCaretW->Wrap( -1 );
    fgsMisc->Add( lblCaretW, 0, wxALL, 5 );

    m_spnCaretW = new wxSpinCtrl( pnlMisc, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 1 );
    fgsMisc->Add( m_spnCaretW, 1, wxEXPAND, 5 );

    sizerMisc->Add( fgsMisc, 0, wxEXPAND, 5 );

    pnlMisc->SetSizer( sizerMisc );
    pnlMisc->Layout();
    sizerMisc->Fit( pnlMisc );
    sbsMisc->Add( pnlMisc, 1, wxEXPAND, 0 );

    mainSizer->Add( sbsMisc, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

    SetSizer( mainSizer );
    Layout();
}

wxGDPageEditors::~wxGDPageEditors()
{
}

void wxGDPageEditors::OnPrefsChanged( wxCommandEvent & )
{
    bool isDirty = false;

    wxCommandEvent evt( wxGD_EVT_GUI_OPTION_CHANGED );
    evt.SetInt( isDirty ? 1 : 0 );
    GetParent()->GetEventHandler()->ProcessEvent( evt );
}

void wxGDPageEditors::OnUpdatePrefs( wxCommandEvent & )
{

}

void wxGDPageEditors::OnSavePrefs( wxCommandEvent & )
{
    
}
