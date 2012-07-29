///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/dialog/prefs.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/07
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
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

wxDEFINE_EVENT( wxGD_EVT_GUI_OPTION_CHANGED, wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_GUI_CONFIG_UPDATE,  wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_GUI_CONFIG_SAVE,    wxCommandEvent );
//=============================================================================
// PrefsDialog
//=============================================================================
wxGD::PrefsDialog::PrefsDialog( wxWindow *parent )
:
wxDialog( parent, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER ),
m_apply     ( NULL ),
m_ok        ( NULL ),
m_cancel    ( NULL ),
m_btnSizer  ( NULL ),
m_tbkPrefs  ( NULL )
{
    wxSize minSize = wxSize( 510,360 );
    SetSizeHints( minSize, wxDefaultSize );

    wxBoxSizer *prefsSizer = new wxBoxSizer( wxVERTICAL );
    prefsSizer->SetMinSize( minSize );
 
    wxImageList *imageList = new wxImageList( 16, 16 );
    m_tbkPrefs             = new wxTreebook( this, wxID_ANY  );
    m_tbkPrefs->AssignImageList( imageList );

    wxBitmap bmpPrj     = wxXmlResource::Get()->LoadBitmap("project_small");
    wxBitmap bmpLocale  = wxXmlResource::Get()->LoadBitmap("locale");
    wxBitmap bmpDesign  = wxXmlResource::Get()->LoadBitmap("designer");
    wxBitmap bmpEditor  = wxXmlResource::Get()->LoadBitmap("editor");

    int imgIndex = -1;
    if( bmpPrj.IsOk() )
        imgIndex = imageList->Add( bmpPrj );
    m_tbkPrefs->AddPage( new PageProject(this), _("Project"), true, imgIndex );

    imgIndex = -1;
    if( bmpLocale.IsOk() )
        imgIndex = imageList->Add( bmpLocale );
    m_tbkPrefs->AddPage( new PageLocale(this), _("Locale"), false, imgIndex );

    imgIndex = -1;
    if( bmpEditor.IsOk() )
        imgIndex = imageList->Add( bmpEditor );
    m_tbkPrefs->AddPage( new PageEditors(this), _("Editors"), false, imgIndex );

    imgIndex = -1;
    if( bmpDesign.IsOk() )
        imgIndex = imageList->Add( bmpDesign );
    m_tbkPrefs->AddPage( new PageGUI(this), _("GUI"), false, imgIndex );

    prefsSizer->Add( m_tbkPrefs, 1, wxALL|wxEXPAND, 5 );

    m_apply  = new wxButton( this, wxID_APPLY ); m_apply->Enable( false );
    m_ok     = new wxButton( this, wxID_OK );    m_ok->Enable( false );
    m_cancel = new wxButton( this, wxID_CANCEL );

    m_btnSizer = new wxStdDialogButtonSizer();
    m_btnSizer->AddButton( m_ok );
    m_btnSizer->AddButton( m_apply );
    m_btnSizer->AddButton( m_cancel );
    m_btnSizer->Realize();

    prefsSizer->Add( m_btnSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 5 );
    SetSizer( prefsSizer );
    Layout();
    prefsSizer->Fit( this );
    Centre( wxBOTH );

    Bind( wxGD_EVT_GUI_OPTION_CHANGED, &PrefsDialog::OnPrefsChanged, this );
    m_ok->Bind( wxEVT_COMMAND_BUTTON_CLICKED,
                                        &PrefsDialog::OnUpdatePrefs, this );
    m_apply->Bind( wxEVT_COMMAND_BUTTON_CLICKED,
                                        &PrefsDialog::OnSavePrefs,   this );
}

wxGD::PrefsDialog::~PrefsDialog()
{
    Unbind( wxGD_EVT_GUI_OPTION_CHANGED, &PrefsDialog::OnPrefsChanged, this );
}

void wxGD::PrefsDialog::OnPrefsChanged( wxCommandEvent &event )
{
    m_apply->Enable ( event.GetInt() );
    m_ok->Enable    ( event.GetInt() );
}

void wxGD::PrefsDialog::OnUpdatePrefs( wxCommandEvent & )
{
    wxCommandEvent evt( wxGD_EVT_GUI_CONFIG_UPDATE );
    for( size_t i = 0; i < m_tbkPrefs->GetPageCount(); i++ )
    {
        wxWindow *page = m_tbkPrefs->GetPage(i);
        page->GetEventHandler()->ProcessEvent( evt );
    }

    Destroy();
}

void wxGD::PrefsDialog::OnSavePrefs( wxCommandEvent & )
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
// PageProject
//=============================================================================
wxGD::PageProject::PageProject( PrefsDialog *parent )
:
wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL ),
m_choPrjVer ( NULL ),
m_choPrjEnc ( NULL ),
m_chkCompat ( NULL ),
m_clbCodeGen( NULL )
{
    wxPanel    *pnlProject  = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer *prjSizer    = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *prjVerSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText *lblPrjVer = new wxStaticText( pnlProject, wxID_ANY,
                            _("Version:"), wxDefaultPosition, wxDefaultSize, 0 );
    prjVerSizer->Add( lblPrjVer, 0, wxALIGN_CENTER_VERTICAL, 0 );

    wxConfigBase::Get()->Read( "xrc/encoding",  &m_selEnc,    0 );
    wxConfigBase::Get()->Read( "xrc/version",   &m_selXrcVer, 1 );
    wxConfigBase::Get()->Read( "code/28compat", &m_compat, true );

    if( (m_selXrcVer < 0) || (m_selXrcVer > 2)  ) m_selXrcVer = 1;

    wxString choPrjVerChoices[] = { "2.3.0.1", "2.5.3.0", _("Custom") };
    int choPrjVerNChoices = sizeof( choPrjVerChoices ) / sizeof( wxString );
    m_choPrjVer = new wxChoice( pnlProject, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, choPrjVerNChoices,
                                                choPrjVerChoices, 0 );
    m_choPrjVer->SetSelection( m_selXrcVer );
    m_choPrjVer->SetToolTip( _("Choose the XRC version to use") );

    prjVerSizer->Add( m_choPrjVer, 0, wxEXPAND, 0 );

    wxStaticText *lblPrjEnc = new wxStaticText( pnlProject, wxID_ANY,
                                                _("Encoding:"),
                                                wxDefaultPosition,
                                                wxDefaultSize, 0 );
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
    prjSizer->Fit( pnlProject );

    pnlProject->SetSizer( prjSizer );
    pnlProject->Layout();
// ----------------------------------------------------------------------------
// XRC Options
// ----------------------------------------------------------------------------
    wxStaticBoxSizer *sbsProject = new wxStaticBoxSizer(
                                    new wxStaticBox( this, wxID_ANY, _("XRC") ),
                                                        wxVERTICAL );
    sbsProject->Add( pnlProject, 1, wxEXPAND, 0 );

    wxBoxSizer *projectSizer = new wxBoxSizer( wxVERTICAL );
    projectSizer->Add( sbsProject, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
// ----------------------------------------------------------------------------
// wxWidgets Options
// ----------------------------------------------------------------------------
    wxPanel *pnlWx = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBoxSizer *sbsWx = new wxStaticBoxSizer(
                                    new wxStaticBox( this, wxID_ANY,
                                        _("wxWidgets") ), wxVERTICAL );
    sbsWx->Add( pnlWx, 1, wxEXPAND );

    m_chkCompat = new wxCheckBox( pnlWx, wxID_ANY, _("2.8 Compatibility") );
    wxBoxSizer *sizerWx = new wxBoxSizer( wxVERTICAL );

    sizerWx->Add( m_chkCompat );
    sizerWx->Fit( pnlWx );

    pnlWx->SetSizer( sizerWx );
    pnlWx->Layout();

    projectSizer->Add( sbsWx, 0, wxEXPAND | wxLEFT | wxRIGHT, 5 );
    projectSizer->Fit( this );

    SetSizer( projectSizer );
    Layout();

    m_choPrjEnc->Bind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &PageProject::OnPrefsChanged, this );
    m_choPrjVer->Bind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &PageProject::OnPrefsChanged, this );
    m_chkCompat->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageProject::OnPrefsChanged, this );

    Bind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageProject::OnUpdatePrefs, this );
    Bind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageProject::OnSavePrefs,   this );
}

wxGD::PageProject::~PageProject()
{
    m_choPrjEnc->Unbind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &PageProject::OnPrefsChanged, this );
    m_choPrjVer->Unbind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &PageProject::OnPrefsChanged, this );
    m_chkCompat->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageProject::OnPrefsChanged, this );

    Unbind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageProject::OnUpdatePrefs, this );
    Unbind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageProject::OnSavePrefs,   this );
}

void wxGD::PageProject::OnPrefsChanged( wxCommandEvent & )
{
    bool isDirty = (m_selEnc    != m_choPrjEnc->GetSelection()) ||
                   (m_selXrcVer != m_choPrjVer->GetSelection()) ||
                   (m_compat    != m_chkCompat->IsChecked());

    wxCommandEvent evt( wxGD_EVT_GUI_OPTION_CHANGED );
    evt.SetInt( isDirty ? 1 : 0 );
    GetParent()->GetEventHandler()->ProcessEvent( evt );
}

void wxGD::PageProject::OnUpdatePrefs( wxCommandEvent & )
{
    m_selEnc    = m_choPrjEnc->GetSelection();
    m_selXrcVer = m_choPrjVer->GetSelection();
    m_compat    = m_chkCompat->IsChecked();
}

void wxGD::PageProject::OnSavePrefs( wxCommandEvent & )
{
    wxConfigBase::Get()->Write( "xrc/encoding",  m_choPrjEnc->GetSelection() );
    wxConfigBase::Get()->Write( "xrc/version",   m_choPrjVer->GetSelection() );
    wxConfigBase::Get()->Write( "code/28compat", m_chkCompat->IsChecked() );
    wxConfigBase::Get()->Flush();
}
//=============================================================================
// PageGUI
//=============================================================================
wxGD::PageGUI::PageGUI( PrefsDialog *parent )
:
wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL ),
m_chkIcons  ( NULL )
{
    wxPanel *pnlPalette = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxTAB_TRAVERSAL );

    m_chkIcons = new wxCheckBox( pnlPalette, wxID_ANY, _("Small Icons") );
    m_chkIcons->SetToolTip( _("Use small icons for the controls palette") );

    // Load locale config
    wxConfigBase::Get()->Read( "gui/smallicons", &m_smallIcons, true );
    m_chkIcons->SetValue( m_smallIcons );

    wxBoxSizer *paletteSizer = new wxBoxSizer( wxVERTICAL );
    paletteSizer->Add( m_chkIcons, 0, 0, 0 );
    paletteSizer->Fit( pnlPalette );

    pnlPalette->SetSizer( paletteSizer );
    pnlPalette->Layout();

    wxStaticBoxSizer *sbsPalette = new wxStaticBoxSizer(
                                    new wxStaticBox( this, wxID_ANY,
                                                _("Palette") ), wxVERTICAL );
    sbsPalette->Add( pnlPalette, 1, wxEXPAND, 0 );

    wxBoxSizer *guiSizer = new wxBoxSizer( wxVERTICAL );
    guiSizer->Add( sbsPalette, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    SetSizer( guiSizer );
    Layout();

    m_chkIcons->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                      &PageGUI::OnPrefsChanged, this );
    Bind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageGUI::OnUpdatePrefs,  this );
    Bind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageGUI::OnSavePrefs,    this );
}

wxGD::PageGUI::~PageGUI()
{
    m_chkIcons->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                        &PageGUI::OnPrefsChanged, this );
    Unbind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageGUI::OnUpdatePrefs,  this );
    Unbind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageGUI::OnSavePrefs,    this );
}

void wxGD::PageGUI::OnPrefsChanged( wxCommandEvent & )
{
    bool isDirty = (m_smallIcons != m_chkIcons->IsChecked());

    wxCommandEvent evt( wxGD_EVT_GUI_OPTION_CHANGED );
    evt.SetInt( isDirty ? 1 : 0 );
    GetParent()->GetEventHandler()->ProcessEvent( evt );
}

void wxGD::PageGUI::OnUpdatePrefs( wxCommandEvent & )
{
    m_smallIcons =  m_chkIcons->IsChecked();
}

void wxGD::PageGUI::OnSavePrefs( wxCommandEvent & )
{
    wxConfigBase::Get()->Write( "gui/smallicons", m_chkIcons->IsChecked() );
    wxConfigBase::Get()->Flush();
}
//=============================================================================
// PageLocale
//=============================================================================
wxGD::PageLocale::PageLocale( PrefsDialog *parent )
:
wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL ),
m_bcbLang   ( NULL ),
m_chkLang   ( NULL ),
m_enabled   ( false ),
m_selected  ( 0 )
{
    wxPanel *pnlLocale = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBoxSizer *sbsLocale = new wxStaticBoxSizer(
                                    new wxStaticBox( this, wxID_ANY,
                                    _("Locale") ), wxVERTICAL );
    sbsLocale->Add( pnlLocale, 1, wxEXPAND, 5 );

    m_chkLang   = new wxCheckBox( pnlLocale, wxID_ANY, _("Enable Localization") );

    m_bcbLang   = new wxBitmapComboBox( pnlLocale, wxID_ANY, wxEmptyString,
                                        wxDefaultPosition, wxDefaultSize,
                                                    0, NULL, wxCB_READONLY );
    m_bcbLang->Append( _("System Default"), wxBitmap( default_xpm ) );
    m_bcbLang->Append( _("English"),        wxBitmap( en_GB_xpm ) );
    m_bcbLang->Append( _("Italian"),        wxBitmap( it_xpm )  );

    wxBoxSizer *sizerLang = new wxBoxSizer( wxVERTICAL );
    sizerLang->Add( m_bcbLang, 1, wxALIGN_RIGHT | wxALL | wxEXPAND, 5 );

    wxBoxSizer *pnlSizer = new wxBoxSizer( wxVERTICAL );
    pnlSizer->Add( m_chkLang,   0, wxEXPAND | wxLEFT | wxTOP, 5 );
    pnlSizer->Add( sizerLang, 0, wxEXPAND, 5 );

    pnlLocale->SetSizer( pnlSizer );
    pnlLocale->Layout();
    pnlSizer->Fit( pnlLocale );

    wxConfigBase::Get()->Read( "locale/enabled",  &m_enabled,  false );
    wxConfigBase::Get()->Read( "locale/selected", &m_selected, 0    );

    m_bcbLang->SetSelection( m_selected );
    m_bcbLang->Enable      ( m_enabled );
    m_chkLang->SetValue    ( m_enabled );

    m_bcbLang->SetToolTip( _("Select language to use") );
    m_chkLang->SetToolTip( _("Enable / Disable wxGUIDesigner localization") );

    wxBoxSizer *localeSizer = new wxBoxSizer( wxVERTICAL );
    localeSizer->Add( sbsLocale, 0, wxEXPAND | wxLEFT | wxRIGHT, 5 );
    localeSizer->Fit( this );

    SetSizer( localeSizer );
    Layout();

    m_chkLang->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageLocale::OnPrefsChanged, this );

    m_bcbLang->Bind( wxEVT_COMMAND_COMBOBOX_SELECTED,
                                    &PageLocale::OnPrefsChanged, this );

    Bind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageLocale::OnUpdatePrefs, this );
    Bind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageLocale::OnSavePrefs,   this );
}

wxGD::PageLocale::~PageLocale()
{
    m_chkLang->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        &PageLocale::OnPrefsChanged, this );

    m_bcbLang->Unbind( wxEVT_COMMAND_COMBOBOX_SELECTED,
                        &PageLocale::OnPrefsChanged, this );

    Unbind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageLocale::OnUpdatePrefs, this );
    Unbind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageLocale::OnSavePrefs,   this );
}

void wxGD::PageLocale::OnPrefsChanged( wxCommandEvent & )
{
    bool isDirty = (m_enabled  != m_chkLang->IsChecked())   ||
                   (m_selected != m_bcbLang->GetSelection());

    m_bcbLang->Enable( m_chkLang->IsChecked() );

    wxCommandEvent evt( wxGD_EVT_GUI_OPTION_CHANGED );
    evt.SetInt( isDirty ? 1 : 0 );
    GetParent()->GetEventHandler()->ProcessEvent( evt );
}

void wxGD::PageLocale::OnUpdatePrefs( wxCommandEvent & )
{
    m_enabled  = m_chkLang->IsChecked();
    m_selected = m_bcbLang->GetSelection();
}

void wxGD::PageLocale::OnSavePrefs( wxCommandEvent & )
{
    wxConfigBase::Get()->Write( "locale/enabled",  m_chkLang->IsChecked() );
    wxConfigBase::Get()->Write( "locale/selected", m_bcbLang->GetSelection() );
    wxConfigBase::Get()->Flush();
}
//=============================================================================
// PageEditors
//=============================================================================
wxGD::PageEditors::PageEditors( PrefsDialog *parent )
:
wxPanel( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL ),
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
    wxBoxSizer   *editSizer = new wxBoxSizer( wxVERTICAL );
    wxStaticText *lblEditor = new wxStaticText( this, wxID_ANY, _("Editor:") );
    lblEditor->Wrap( -1 );

    m_bcbEditor = new wxBitmapComboBox( this, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 

    wxFlexGridSizer *fgsEdit = new wxFlexGridSizer( 0,2,0,0 );
    fgsEdit->AddGrowableCol( 1 );
    fgsEdit->SetFlexibleDirection( wxBOTH );
    fgsEdit->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    fgsEdit->Add( lblEditor, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxTOP, 5 );
    fgsEdit->Add( m_bcbEditor, 0, wxEXPAND | wxRIGHT | wxTOP, 5 );
    editSizer->Add( fgsEdit, 0, wxEXPAND, 5 );

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
    editSizer->Add( sbsGuides, 0, wxEXPAND | wxLEFT | wxRIGHT, 5 );

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
    editSizer->Add( sbsTabs, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

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
    editSizer->Add( sbsMisc, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5 );

    SetSizer( editSizer );
    Layout();

    m_chkLineNums->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkGuides->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkWSpace->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkEOL->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkBackspace->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkUseTabs->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkTabsIndent->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_choTabsWidth->Bind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &PageEditors::OnPrefsChanged, this );
    m_spnCaretW->Bind( wxEVT_COMMAND_SPINCTRL_UPDATED,
                                    &PageEditors::OnPrefsChanged, this );

    Bind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageEditors::OnUpdatePrefs, this );
    Bind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageEditors::OnSavePrefs,   this );
}

wxGD::PageEditors::~PageEditors()
{
    m_chkLineNums->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkGuides->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkWSpace->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkEOL->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkBackspace->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkUseTabs->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_chkTabsIndent->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED,
                                    &PageEditors::OnPrefsChanged, this );
    m_choTabsWidth->Unbind( wxEVT_COMMAND_CHOICE_SELECTED,
                                    &PageEditors::OnPrefsChanged, this );
    m_spnCaretW->Unbind( wxEVT_COMMAND_SPINCTRL_UPDATED,
                                    &PageEditors::OnPrefsChanged, this );

    Unbind( wxGD_EVT_GUI_CONFIG_UPDATE, &PageEditors::OnUpdatePrefs, this );
    Unbind( wxGD_EVT_GUI_CONFIG_SAVE,   &PageEditors::OnSavePrefs,   this );
}

void wxGD::PageEditors::OnPrefsChanged( wxCommandEvent & )
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

void wxGD::PageEditors::OnUpdatePrefs( wxCommandEvent & )
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

void wxGD::PageEditors::OnSavePrefs( wxCommandEvent & )
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
