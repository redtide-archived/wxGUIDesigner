///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/dialog/prefs.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/07
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/bmpcbox.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/choice.h>
#include <wx/config.h>
#include <wx/dialog.h>
#include <wx/fontenum.h>
#include <wx/fontmap.h>
#include <wx/imaglist.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/treebook.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/gui/flags/en_GB.xpm"
#include "wxguidesigner/gui/flags/it.xpm"
#include "wxguidesigner/gui/flags/default.xpm"

#include "wxguidesigner/gui/dialog/prefs.h"
#include "wxguidesigner/settings.h"
#include "wxguidesigner/gui/handler.h"

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

    wxBoxSizer *mainSizer = new wxBoxSizer( wxVERTICAL );
    mainSizer->SetMinSize( minSize );
 
    wxImageList *imageList = new wxImageList( 16, 16 );
    m_tbkPrefs             = new wxTreebook( this, wxID_ANY  );
    m_tbkPrefs->AssignImageList( imageList );

    wxBitmap bmpPrj     = wxXmlResource::Get()->LoadBitmap("project_small");
    wxBitmap bmpLocale  = wxXmlResource::Get()->LoadBitmap("locale");
//  wxBitmap bmpDesign  = wxXmlResource::Get()->LoadBitmap("designer");
    wxBitmap bmpEditor  = wxXmlResource::Get()->LoadBitmap("editor");

    int imgIndex = -1;
    if( bmpPrj.IsOk() )
        imgIndex = imageList->Add( bmpPrj );
    m_tbkPrefs->AddPage( new wxGDPageProject( m_handler, this ), _("Project"),
                                                            true, imgIndex );
    imgIndex = -1;
    if( bmpLocale.IsOk() )
        imgIndex = imageList->Add( bmpLocale );
    m_tbkPrefs->AddPage( new wxGDPageLocale( m_handler, this ), _("Locale"),
                                                            false, imgIndex );
/*
    imgIndex = -1;
    if( bmpDesign.IsOk() )
        imgIndex = imageList->Add( bmpDesign );
    m_tbkPrefs->AddPage( new wxPanel( this, wxID_ANY ), _("GUI"), false, imgIndex );
*/
    imgIndex = -1;
    if( bmpEditor.IsOk() )
        imgIndex = imageList->Add( bmpEditor );
    m_tbkPrefs->AddPage( new wxGDPageEditors( m_handler, this ), _("Editors"),
                                                            false, imgIndex );
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
// wxGDPageProject
//=============================================================================
wxGDPageProject::wxGDPageProject( wxGDHandler *handler, wxGDDialogPrefs *parent )
:
wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL ),
m_handler   ( handler ),
m_choPrjVer ( NULL ),
m_choPrjEnc ( NULL ),
m_choWxVer  ( NULL ),
m_clbCodeGen( NULL )
{
    wxBoxSizer *projectSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer *sbsProject = new wxStaticBoxSizer( new wxStaticBox(
                                    this, wxID_ANY, _("XRC") ), wxVERTICAL );

    wxPanel *pnlProject     = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer *prjSizer    = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *prjVerSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText *lblPrjVer = new wxStaticText( pnlProject, wxID_ANY,
                            _("Version:"), wxDefaultPosition, wxDefaultSize, 0 );
    lblPrjVer->Wrap( -1 );
    prjVerSizer->Add( lblPrjVer, 0, wxALIGN_CENTER_VERTICAL, 0 );

    wxConfigBase::Get()->Read( "xrc/encoding",  &m_selEnc,    0 );
    wxConfigBase::Get()->Read( "xrc/version",   &m_selXrcVer, 1 );
    wxConfigBase::Get()->Read( "xrc/wxversion", &m_selWxVer,  2 );

    if( (m_selXrcVer < 0) || (m_selXrcVer > 2)  ) m_selXrcVer = 1;
    if( (m_selWxVer  < 0) || (m_selWxVer  > 2)  ) m_selWxVer  = 2;

    wxString m_choPrjVerChoices[] = { "2.3.0.1", "2.5.3.0", _("Custom") };
    int m_choPrjVerNChoices = sizeof( m_choPrjVerChoices ) / sizeof( wxString );
    m_choPrjVer = new wxChoice( pnlProject, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, m_choPrjVerNChoices,
                                m_choPrjVerChoices, 0 );
    m_choPrjVer->SetSelection( m_selXrcVer );
    m_choPrjVer->SetToolTip( _("Choose the XRC version to use") );

    prjVerSizer->Add( m_choPrjVer, 0, wxEXPAND, 0 );

    wxStaticText *lblPrjEnc = new wxStaticText( pnlProject, wxID_ANY,
                                                _("Encoding:"),
                                                wxDefaultPosition,
                                                wxDefaultSize, 0 );
    lblPrjEnc->Wrap( -1 );
    prjVerSizer->Add( lblPrjEnc, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5 );

    wxArrayString   fontEncodings;
    wxFontEncoding  fontEncoding;
    size_t          encCount = wxFontMapper::GetSupportedEncodingsCount();

    if( (m_selEnc < 0) || (m_selEnc > (int)encCount)  ) m_selXrcVer = 0;

    for( size_t i = 0; i < encCount; i++ )
    {
        fontEncoding = wxFontMapper::GetEncoding(i);
        if( (fontEncoding == wxFONTENCODING_SYSTEM) ||
            (fontEncoding == wxFONTENCODING_DEFAULT) )
            continue;

        fontEncodings.Add( wxFontMapper::GetEncodingName(fontEncoding) );
    }

    m_choPrjEnc = new wxChoice( pnlProject, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, fontEncodings, 0 );
    m_choPrjEnc->SetSelection( m_selEnc );
    prjVerSizer->Add( m_choPrjEnc, 1, 0, 5 );

    prjSizer->Add( prjVerSizer, 0, wxEXPAND, 0 );

    wxArrayString codeGenChoices;
    m_clbCodeGen = new wxCheckListBox( pnlProject, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, codeGenChoices,
                                        wxLB_EXTENDED | wxLB_HSCROLL | wxLB_SORT |
                                        wxLB_MULTIPLE | wxLB_NEEDED_SB );
    m_clbCodeGen->Enable( false );
    m_clbCodeGen->Hide();
    m_clbCodeGen->SetToolTip( _("Choose the code language to generate") );

    prjSizer->Add( m_clbCodeGen, 0, wxEXPAND|wxTOP, 5 );

    pnlProject->SetSizer( prjSizer );
    pnlProject->Layout();
    prjSizer->Fit( pnlProject );
    sbsProject->Add( pnlProject, 1, wxEXPAND, 0 );

    projectSizer->Add( sbsProject, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxStaticBoxSizer *sbSizer6;
    sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY,
                                    _("wxWidgets") ), wxVERTICAL );
    wxPanel *pnlProject1;
    pnlProject1 = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, wxTAB_TRAVERSAL );

    wxBoxSizer *sizerwx    = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *wxVerSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText *lblWxer  = new wxStaticText( pnlProject1, wxID_ANY,
                                                _("Version:"),
                                                wxDefaultPosition,
                                                wxDefaultSize, 0 );
    lblWxer->Wrap( -1 );
    wxVerSizer->Add( lblWxer, 0, wxALIGN_CENTER_VERTICAL, 0 );

    wxString wxVerChoices[] = {"2.8", "2.9", _("All")};
    int wxVerNChoices = sizeof( wxVerChoices ) / sizeof( wxString );
    m_choWxVer = new wxChoice( pnlProject1, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, wxVerNChoices, wxVerChoices, 0 );
    m_choWxVer->SetSelection( m_selWxVer );
    m_choWxVer->SetToolTip( _("Choose the XRC version to use") );

    wxVerSizer->Add( m_choWxVer, 0, wxEXPAND, 0 );
    sizerwx->Add( wxVerSizer, 0, wxEXPAND, 0 );

    pnlProject1->SetSizer( sizerwx );
    pnlProject1->Layout();
    sizerwx->Fit( pnlProject1 );
    sbSizer6->Add( pnlProject1, 1, wxEXPAND, 0 );

    projectSizer->Add( sbSizer6, 0, wxEXPAND | wxLEFT | wxRIGHT, 5 );

    SetSizer( projectSizer );
    Layout();
    projectSizer->Fit( this );

    m_choPrjEnc->Bind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageProject::OnPrefsChanged, this );
    m_choPrjVer->Bind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageProject::OnPrefsChanged, this );
    m_choWxVer->Bind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageProject::OnPrefsChanged, this );

    Bind( wxGD_EVT_GUI_CONFIG_UPDATE, &wxGDPageProject::OnUpdatePrefs, this );
    Bind( wxGD_EVT_GUI_CONFIG_SAVE,   &wxGDPageProject::OnSavePrefs,   this );
}

wxGDPageProject::~wxGDPageProject()
{
    m_choPrjEnc->Unbind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageProject::OnPrefsChanged, this );
    m_choPrjVer->Unbind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageProject::OnPrefsChanged, this );
    m_choWxVer->Unbind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageProject::OnPrefsChanged, this );

    Unbind( wxGD_EVT_GUI_CONFIG_UPDATE, &wxGDPageProject::OnUpdatePrefs, this );
    Unbind( wxGD_EVT_GUI_CONFIG_SAVE,   &wxGDPageProject::OnSavePrefs,   this );
}

void wxGDPageProject::OnPrefsChanged( wxCommandEvent & )
{
    bool isDirty = (m_selEnc    != m_choPrjEnc->GetSelection()) ||
                   (m_selXrcVer != m_choPrjVer->GetSelection()) ||
                   (m_selWxVer  != m_choWxVer->GetSelection());

    wxCommandEvent evt( wxGD_EVT_GUI_OPTION_CHANGED );
    evt.SetInt( isDirty ? 1 : 0 );
    GetParent()->GetEventHandler()->ProcessEvent( evt );
}

void wxGDPageProject::OnUpdatePrefs( wxCommandEvent & )
{
    m_selEnc    = m_choPrjEnc->GetSelection();
    m_selXrcVer = m_choPrjVer->GetSelection();
    m_selWxVer  = m_choWxVer->GetSelection();
}

void wxGDPageProject::OnSavePrefs( wxCommandEvent & )
{
    wxConfigBase::Get()->Write( "xrc/encoding",  m_choPrjEnc->GetSelection() );
    wxConfigBase::Get()->Write( "xrc/version",   m_choPrjVer->GetSelection() );
    wxConfigBase::Get()->Write( "xrc/wxversion", m_choWxVer->GetSelection() );
    wxConfigBase::Get()->Flush();
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
m_enabled   ( false ),
m_selected  ( 0 )
{
    wxBoxSizer       *localeSizer = new wxBoxSizer( wxVERTICAL );
    wxStaticBoxSizer *sbsLocale   = new wxStaticBoxSizer(
                                    new wxStaticBox( this, wxID_ANY,
                                    _("Locale") ), wxVERTICAL );

    wxPanel *pnlLocale = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxTAB_TRAVERSAL );

    m_chkLang   = new wxCheckBox( pnlLocale, wxID_ANY, _("Enable Localization") );

    m_bcbLang   = new wxBitmapComboBox( pnlLocale, wxID_ANY, wxEmptyString,
                                        wxDefaultPosition, wxDefaultSize,
                                                    0, NULL, wxCB_READONLY );

    m_bcbLang->Append( _("System Default"), wxBitmap( default_xpm ) );
    m_bcbLang->Append( _("English"),        wxBitmap( en_GB_xpm ) );
    m_bcbLang->Append( _("Italian"),        wxBitmap( it_xpm )  );
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

    sbsLocale->Add( pnlLocale, 1, wxEXPAND, 5 );
    localeSizer->Add( sbsLocale, 0, wxEXPAND | wxLEFT | wxRIGHT, 5 );

    m_bcbLang->SetToolTip( _("Select language to use") );
    m_chkLang->SetToolTip( _("Enable / Disable wxGUIDesigner localization") );

    wxBoxSizer *sizerLang = new wxBoxSizer( wxVERTICAL );
    sizerLang->Add( m_bcbLang, 1, wxALIGN_RIGHT | wxALL | wxEXPAND, 5 );

    wxBoxSizer *pnlSizer = new wxBoxSizer( wxVERTICAL );
    pnlSizer->Add( m_chkLang,   0, wxEXPAND | wxLEFT | wxTOP, 5 );
    pnlSizer->Add( sizerLang, 0, wxEXPAND, 5 );

    pnlLocale->SetSizer( pnlSizer );
    pnlLocale->Layout();
    pnlSizer->Fit( pnlLocale );

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
    m_chkLang->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        &wxGDPageLocale::OnPrefsChanged, this );

    m_bcbLang->Unbind( wxEVT_COMMAND_COMBOBOX_SELECTED,
                        &wxGDPageLocale::OnPrefsChanged, this );

    Unbind( wxGD_EVT_GUI_CONFIG_UPDATE, &wxGDPageLocale::OnUpdatePrefs, this );
    Unbind( wxGD_EVT_GUI_CONFIG_SAVE,   &wxGDPageLocale::OnSavePrefs,   this );
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
    wxBoxSizer   *mainSizer = new wxBoxSizer( wxVERTICAL );
    wxStaticText *lblEditor = new wxStaticText( this, wxID_ANY, _("Editor:") );
    lblEditor->Wrap( -1 );

    m_bcbEditor = new wxBitmapComboBox( this, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 

    wxFlexGridSizer *editSizer = new wxFlexGridSizer( 0,2,0,0 );
    editSizer->AddGrowableCol( 1 );
    editSizer->SetFlexibleDirection( wxBOTH );
    editSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    editSizer->Add( lblEditor, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxTOP, 5 );
    editSizer->Add( m_bcbEditor, 0, wxEXPAND | wxRIGHT | wxTOP, 5 );
    mainSizer->Add( editSizer, 0, wxEXPAND, 5 );

    wxFlexGridSizer *fgsGuides = new wxFlexGridSizer( 0,2,0,0 );
    fgsGuides->AddGrowableCol( 1 );
    fgsGuides->SetFlexibleDirection( wxBOTH );
    fgsGuides->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    wxPanel *pnlGuides = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxTAB_TRAVERSAL );

    m_chkLineNums =  new wxCheckBox( pnlGuides, wxID_ANY, _("Show Line Numbers") );
    m_chkGuides =    new wxCheckBox( pnlGuides, wxID_ANY, _("Show Indentation Guides") );
    m_chkWSpace =    new wxCheckBox( pnlGuides, wxID_ANY, _("View Whitespace") );
    m_chkEOL =       new wxCheckBox( pnlGuides, wxID_ANY, _("View EOL") );
    m_chkBackspace = new wxCheckBox( pnlGuides, wxID_ANY, _("Backspace Unindents") );

    wxConfigBase::Get()->Read( "editors/ShowLineNumbers", &m_showLines, true );
    wxConfigBase::Get()->Read( "editors/ShowIndentationGuides", &m_showGuides, true );
    wxConfigBase::Get()->Read( "editors/ViewWhitespace", &m_viewWSpace, true );
    wxConfigBase::Get()->Read( "editors/ViewEOL", &m_viewEOL, false );
    wxConfigBase::Get()->Read( "editors/BackspaceUnindents", &m_useBSIndent, true );
    wxConfigBase::Get()->Read( "editors/UseTabs", &m_useTabs, false );
    wxConfigBase::Get()->Read( "editors/UseTabsIndent", &m_useTabsIndent, true );
    wxConfigBase::Get()->Read( "editors/TabsWidth", &m_selTabsW, 4 );
    wxConfigBase::Get()->Read( "editors/CaretWidth", &m_caretW, 1 );

    if( (m_selTabsW < 1) || (m_selTabsW > 8 ) ) m_selTabsW = 4;
    if( (m_caretW   < 0) || (m_caretW   > 10) ) m_caretW   = 1;

    m_chkLineNums->SetValue( m_showLines );
    m_chkGuides->SetValue( m_showGuides );
    m_chkWSpace->SetValue( m_viewWSpace );
    m_chkEOL->SetValue( m_viewEOL );
    m_chkBackspace->SetValue( m_useBSIndent );

    fgsGuides->Add( m_chkLineNums, 0, 0, 5 );
    fgsGuides->Add( m_chkGuides, 0, 0, 5 );
    fgsGuides->Add( m_chkWSpace, 0, 0, 5 );
    fgsGuides->Add( m_chkEOL, 0, 0, 5 );
    fgsGuides->Add( m_chkBackspace );

    pnlGuides->SetSizer( fgsGuides );
    pnlGuides->Layout();
    fgsGuides->Fit( pnlGuides );

    wxStaticBoxSizer *sbsGuides = new wxStaticBoxSizer( new wxStaticBox(
                                    this, wxID_ANY, _("Guides") ), wxVERTICAL );

    sbsGuides->Add( pnlGuides, 0, wxEXPAND, 5 );
    mainSizer->Add( sbsGuides, 0, wxEXPAND | wxLEFT | wxRIGHT, 5 );

    wxPanel *pnlTabs = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                    wxDefaultSize, wxTAB_TRAVERSAL );

    m_chkUseTabs = new wxCheckBox( pnlTabs, wxID_ANY, _("Use Tabs") );
    m_chkTabsIndent = new wxCheckBox( pnlTabs, wxID_ANY, _("Tabs Indent") );

    m_chkUseTabs->SetValue( m_useTabs );
    m_chkTabsIndent->SetValue( m_useTabsIndent );

    wxStaticText *lblTabsWidth = new wxStaticText( pnlTabs, wxID_ANY,
                                                        _("Tabs Width:") );
    lblTabsWidth->Wrap( -1 );
    wxString tabsWidthChoices[] = {"1","2","3","4","5","6","7","8"};
    int tabsWidthNChoices = sizeof( tabsWidthChoices ) / sizeof( wxString );
    m_choTabsWidth = new wxChoice( pnlTabs, wxID_ANY, wxDefaultPosition,
                        wxDefaultSize, tabsWidthNChoices, tabsWidthChoices, 0 );
    m_choTabsWidth->SetSelection( m_selTabsW -1 );

    wxFlexGridSizer *fgsTabs = new wxFlexGridSizer( 0,2,0,0 );
    fgsTabs->AddGrowableCol( 1 );
    fgsTabs->SetFlexibleDirection( wxBOTH );
    fgsTabs->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    fgsTabs->Add( m_chkUseTabs, 0, 0, 5 );
    fgsTabs->Add( m_chkTabsIndent, 0, wxLEFT, 5 );
    fgsTabs->Add( lblTabsWidth, 0, wxALIGN_CENTER_VERTICAL, 5 );
    fgsTabs->Add( m_choTabsWidth, 0, wxEXPAND, 5 );

    pnlTabs->SetSizer( fgsTabs );
    pnlTabs->Layout();
    fgsTabs->Fit( pnlTabs );

    wxStaticBoxSizer *sbsTabs = new wxStaticBoxSizer( new wxStaticBox(
                                    this, wxID_ANY, _("Tabs") ),  wxVERTICAL );
    sbsTabs->Add( pnlTabs, 0, wxEXPAND, 0 );
    mainSizer->Add( sbsTabs, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxFlexGridSizer *fgsMisc = new wxFlexGridSizer( 0,2,0,0 );
    fgsMisc->AddGrowableCol( 1 );
    fgsMisc->SetFlexibleDirection( wxBOTH );
    fgsMisc->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    wxPanel *pnlMisc = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                    wxDefaultSize, wxTAB_TRAVERSAL );

    wxStaticText *lblCaretW = new wxStaticText( pnlMisc, wxID_ANY,
                                                        _("Caret Width:") );
    lblCaretW->Wrap( -1 );

    m_spnCaretW = new wxSpinCtrl( pnlMisc, wxID_ANY, wxT("1"), wxDefaultPosition,
                                wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, m_caretW );
    fgsMisc->Add( lblCaretW, 0, wxALL, 5 );
    fgsMisc->Add( m_spnCaretW, 1, wxEXPAND, 5 );

    wxBoxSizer *sizerMisc = new wxBoxSizer( wxVERTICAL );
    sizerMisc->Add( fgsMisc, 0, wxEXPAND, 5 );
    pnlMisc->SetSizer( sizerMisc );
    pnlMisc->Layout();
    sizerMisc->Fit( pnlMisc );

    wxStaticBoxSizer *sbsMisc = new wxStaticBoxSizer( new wxStaticBox( this,
                                            wxID_ANY, _("Misc") ), wxVERTICAL );
    sbsMisc->Add( pnlMisc, 1, wxEXPAND, 0 );
    mainSizer->Add( sbsMisc, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5 );

    SetSizer( mainSizer );
    Layout();

    m_chkLineNums->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkGuides->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkWSpace->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkEOL->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkBackspace->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkUseTabs->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkTabsIndent->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_choTabsWidth->Bind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_spnCaretW->Bind( wxEVT_COMMAND_SPINCTRL_UPDATED,
                                    &wxGDPageEditors::OnPrefsChanged, this );

    Bind( wxGD_EVT_GUI_CONFIG_UPDATE, &wxGDPageEditors::OnUpdatePrefs, this );
    Bind( wxGD_EVT_GUI_CONFIG_SAVE,   &wxGDPageEditors::OnSavePrefs,   this );
}

wxGDPageEditors::~wxGDPageEditors()
{
    m_chkLineNums->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkGuides->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkWSpace->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkEOL->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkBackspace->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkUseTabs->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_chkTabsIndent->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_choTabsWidth->Unbind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &wxGDPageEditors::OnPrefsChanged, this );
    m_spnCaretW->Unbind( wxEVT_COMMAND_SPINCTRL_UPDATED,
                                    &wxGDPageEditors::OnPrefsChanged, this );

    Unbind( wxGD_EVT_GUI_CONFIG_UPDATE, &wxGDPageEditors::OnUpdatePrefs, this );
    Unbind( wxGD_EVT_GUI_CONFIG_SAVE,   &wxGDPageEditors::OnSavePrefs,   this );
}

void wxGDPageEditors::OnPrefsChanged( wxCommandEvent & )
{
    bool isDirty =  (m_showLines     != m_chkLineNums->IsChecked())         ||
                    (m_showGuides    != m_chkGuides->IsChecked())           ||
                    (m_viewWSpace    != m_chkWSpace->IsChecked())           ||
                    (m_viewEOL       != m_chkEOL->IsChecked())              ||
                    (m_useBSIndent   != m_chkBackspace->IsChecked())        ||
                    (m_useTabs       != m_chkUseTabs->IsChecked())          ||
                    (m_useTabsIndent != m_chkTabsIndent->IsChecked())       ||
                    (m_selTabsW      != m_choTabsWidth->GetSelection() +1 ) ||
                    (m_caretW        != m_spnCaretW->GetValue());

    wxCommandEvent evt( wxGD_EVT_GUI_OPTION_CHANGED );
    evt.SetInt( isDirty ? 1 : 0 );
    GetParent()->GetEventHandler()->ProcessEvent( evt );
}

void wxGDPageEditors::OnUpdatePrefs( wxCommandEvent & )
{
//  m_selEditor     = m_bcbEditor->GetSelection();
    m_showLines     = m_chkLineNums->IsChecked();
    m_showGuides    = m_chkGuides->IsChecked();
    m_viewWSpace    = m_chkWSpace->IsChecked();
    m_viewEOL       = m_chkEOL->IsChecked();
    m_useBSIndent   = m_chkBackspace->IsChecked();
    m_useTabs       = m_chkUseTabs->IsChecked();
    m_useTabsIndent = m_chkTabsIndent->IsChecked();
    m_selTabsW      = m_choTabsWidth->GetSelection() +1;
    m_caretW        = m_spnCaretW->GetValue();
}

void wxGDPageEditors::OnSavePrefs( wxCommandEvent & )
{
    wxConfigBase::Get()->Write( "editors/ShowLineNumbers",       m_chkLineNums->IsChecked() );
    wxConfigBase::Get()->Write( "editors/ShowIndentationGuides", m_chkGuides->IsChecked() );
    wxConfigBase::Get()->Write( "editors/ViewWhitespace",        m_chkWSpace->IsChecked() );
    wxConfigBase::Get()->Write( "editors/ViewEOL",               m_chkEOL->IsChecked() );
    wxConfigBase::Get()->Write( "editors/BackspaceUnindents",    m_chkBackspace->IsChecked() );
    wxConfigBase::Get()->Write( "editors/UseTabs",               m_chkUseTabs->IsChecked() );
    wxConfigBase::Get()->Write( "editors/UseTabsIndent",         m_chkTabsIndent->IsChecked() );
    wxConfigBase::Get()->Write( "editors/TabsWidth",             m_choTabsWidth->GetSelection() +1 );
    wxConfigBase::Get()->Write( "editors/CaretWidth",            m_spnCaretW->GetValue() );
    wxConfigBase::Get()->Flush();
}
