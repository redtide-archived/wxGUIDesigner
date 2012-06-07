///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/toplevel/wizard.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/01
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/wizard.h>

#include "core/gui/toplevel/wizard.h"

#include <wx/log.h>

wxDEFINE_EVENT( wxGD_EVT_WIZARD_PAGE_CHANGED,  WizardEvent );
wxDEFINE_EVENT( wxGD_EVT_WIZARD_PAGE_CHANGING, WizardEvent );
wxDEFINE_EVENT( wxGD_EVT_WIZARD_CANCEL,        WizardEvent );
wxDEFINE_EVENT( wxGD_EVT_WIZARD_FINISHED,      WizardEvent );
wxDEFINE_EVENT( wxGD_EVT_WIZARD_HELP,          WizardEvent );
wxDEFINE_EVENT( wxGD_EVT_WIZARD_PAGE_SHOWN,    WizardEvent );

IMPLEMENT_DYNAMIC_CLASS( Wizard,            wxPanel     )
IMPLEMENT_ABSTRACT_CLASS(WizardPage,        wxPanel     )
IMPLEMENT_DYNAMIC_CLASS( WizardPageSimple,  wxWizardPage)
//=============================================================================
// WizardPage
//=============================================================================
void WizardPage::Init()
{
    m_bitmap = wxNullBitmap;
}

WizardPage::WizardPage( Wizard *parent, const wxBitmap &bitmap )
{
    Create( parent, bitmap );
}

bool WizardPage::Create( Wizard *parent, const wxBitmap &bitmap )
{
    if ( !wxPanel::Create(parent, wxID_ANY) )
        return false;

    m_bitmap = bitmap;
    Hide();
    return true;
}
//=============================================================================
// WizardPageSimple
//=============================================================================
WizardPage *WizardPageSimple::GetPrev() const
{
    return m_prev;
}

WizardPage *WizardPageSimple::GetNext() const
{
    return m_next;
}
//=============================================================================
// Wizard
//=============================================================================
void Wizard::Init()
{
    m_page              = NULL;
    m_help              = NULL;
    m_prev              = NULL;
    m_next              = NULL;
    m_canc              = NULL;
    m_sizerBmpAndPage   = NULL;
    m_sizerPage         = NULL;
    m_staticBmp         = NULL;
    m_bitmap            = wxNullBitmap;
}

bool Wizard::Create( wxWindow *parent, int id, const wxString& title,
                    const wxBitmap &bitmap, const wxPoint &pos, long style )
{
    if( !TopLevelWindow::Create
        ( parent, id, title, pos, wxDefaultSize, style, "gd_wizard" ) )
        return false;

    wxBoxSizer *windowSizer  = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *mainColumn   = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *buttonRow    = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *backNextPair = new wxBoxSizer( wxHORIZONTAL );
    m_sizerBmpAndPage        = new wxBoxSizer( wxHORIZONTAL );
    m_sizerPage              = new wxBoxSizer( wxVERTICAL );
    m_staticBmp              = new wxStaticBitmap( this, wxID_ANY, bitmap );

    m_sizerBmpAndPage->SetMinSize( wxSize( 270, 270 ) );
    m_sizerBmpAndPage->Add( m_staticBmp, 0, wxALL, 5 );
    m_sizerBmpAndPage->Add( 5, 0, 0,        wxEXPAND, 0 );
    m_sizerBmpAndPage->Add( m_sizerPage, 1, wxEXPAND, 0 );

    m_help = new wxButton( this, wxID_HELP );
    m_prev = new wxButton( this, wxID_BACKWARD );
    m_next = new wxButton( this, wxID_FORWARD );
    m_canc = new wxButton( this, wxID_CANCEL );

    m_prev->Enable( false );
    m_next->Enable( false );

    backNextPair->Add( m_prev, 0, wxBOTTOM | wxLEFT | wxTOP, 0 );
    backNextPair->Add( 10, 0, 0,  wxEXPAND, 0 );
    backNextPair->Add( m_next, 0, wxBOTTOM | wxRIGHT | wxTOP, 0 );

    buttonRow->Add( m_help, 0, wxALL, 5 );
    buttonRow->Add( backNextPair, 0, wxALL, 5 );
    buttonRow->Add( m_canc, 0, wxALL, 5 );

    mainColumn->Add( m_sizerBmpAndPage, 1, wxEXPAND );
    mainColumn->Add( 0, 5, 0, wxEXPAND );
    mainColumn->Add( new wxStaticLine( this ), 0, wxEXPAND | wxALL, 5 );
    mainColumn->Add( 0, 5, 0, wxEXPAND ); 
    mainColumn->Add( buttonRow, 0, wxALIGN_RIGHT );

    windowSizer->Add( mainColumn, 1, wxALL | wxEXPAND, 5 );

    SetSizer( windowSizer );
    Layout();
    windowSizer->Fit( this );

    m_help->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnHelp,        this );
    m_prev->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnBackOrNext,  this );
    m_next->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnBackOrNext,  this );
    m_canc->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnCancel,      this );

    Bind( wxGD_EVT_WIZARD_PAGE_CHANGED,   &Wizard::OnWizEvent, this );
    Bind( wxGD_EVT_WIZARD_PAGE_CHANGING,  &Wizard::OnWizEvent, this );
    Bind( wxGD_EVT_WIZARD_CANCEL,         &Wizard::OnWizEvent, this );
    Bind( wxGD_EVT_WIZARD_FINISHED,       &Wizard::OnWizEvent, this );
    Bind( wxGD_EVT_WIZARD_HELP,           &Wizard::OnWizEvent, this );

    if( style &~ wxWIZARD_EX_HELPBUTTON )
        m_help->Hide();

    return true;
}

Wizard::~Wizard()
{
    m_help->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnHelp,      this );
    m_prev->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnBackOrNext,this );
    m_next->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnBackOrNext,this );
    m_canc->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &Wizard::OnCancel,    this );

    Unbind( wxGD_EVT_WIZARD_PAGE_CHANGED,   &Wizard::OnWizEvent, this );
    Unbind( wxGD_EVT_WIZARD_PAGE_CHANGING,  &Wizard::OnWizEvent, this );
    Unbind( wxGD_EVT_WIZARD_CANCEL,         &Wizard::OnWizEvent, this );
    Unbind( wxGD_EVT_WIZARD_FINISHED,       &Wizard::OnWizEvent, this );
    Unbind( wxGD_EVT_WIZARD_HELP,           &Wizard::OnWizEvent, this );

    m_staticBmp->SetBitmap( wxNullBitmap );
    m_bitmap = wxNullBitmap;
    m_page = NULL;
    m_pages.clear();
}

void Wizard::OnHelp( wxCommandEvent &event )
{
    if( m_page )
    {
        WizardEvent help( wxGD_EVT_WIZARD_HELP, GetId(), true, m_page );
        m_page->GetEventHandler()->ProcessEvent( help );
    }
}

wxBoxSizer* Wizard::GetPageSizer() const
{
    return m_sizerPage;
}

void Wizard::SetSelection( size_t index )
{
    size_t count = m_pages.size();
    if ( index >= 0 && index < count )
    {
        bool hasPrev = index > 0;
        bool hasNext = index < count - 1;

        m_page = m_pages.at(index);

        m_prev->Enable( hasPrev );

        wxString label;
        if( !hasNext )
            label = _("&Finish");

        if ( label != m_next->GetLabel() )
            m_next->SetLabel( label );

        m_next->SetDefault();
    }
}

void Wizard::OnBackOrNext( wxCommandEvent &event )
{
    size_t index = 0;
    for( ; index < m_pages.size(); index++ )
        if( m_pages.at(index) == m_page )
            break;

    bool forward = ( event.GetEventObject() == m_next );

    forward == true ? ++index : --index;

    SetSelection( index );

    WizardEvent changed( wxGD_EVT_WIZARD_PAGE_CHANGED,
                             GetId(), forward, m_page );

    m_page->GetEventHandler()->ProcessEvent( changed );
}

void Wizard::OnCancel( wxCommandEvent &event )
{
    WizardEvent cancel( wxGD_EVT_WIZARD_CANCEL,
                            GetId(), false, m_page );

    GetEventHandler()->ProcessEvent( cancel );
}

void Wizard::OnWizEvent( WizardEvent &event )
{
    if ( event.IsAllowed() )
    {
        wxEventType eventType = event.GetEventType();
        if( eventType == wxGD_EVT_WIZARD_PAGE_CHANGED )
        {
            for ( size_t i = 0; i < m_pages.size(); i++ )
                m_pages.at(i)->Hide();

            event.GetPage()->Show();
            Layout();
        }
    }
}

void Wizard::AddPage( WizardPageSimple* page )
{
    m_page = page;
    m_pages.push_back(page);

    size_t count = m_pages.size();

    for( size_t index = 0; index < count; index++ )
        m_pages.at(index)->Hide();

    page->Show();

    m_sizerPage->Add( page, 1, wxEXPAND, 0 );

    Layout();

    if( count == 1 )
    {
        m_next->Enable( true );
        m_next->SetLabel(_("&Finish") );
    }
    else if( count == 2 )
    {
        m_prev->Enable( true );
    }
}

WizardPageSimple *Wizard::GetPage( size_t index ) const
{
    return m_pages.at(index);
}

size_t Wizard::GetPageCount() const
{
    return m_pages.size();
}

size_t Wizard::GetPageIndex( WizardPageSimple *page ) const
{
    for( size_t index = 0; index < m_pages.size(); ++index )
    {
        if( m_pages.at(index) == page )
            return index;
    }

    return 0;
}

void Wizard::SetBitmap( const wxBitmap &bitmap )
{
    m_bitmap = bitmap;
    if ( m_staticBmp )
    {
        m_staticBmp->SetBitmap( m_bitmap );
        wxSize pageSize = m_sizerBmpAndPage->GetSize();
        pageSize.IncTo( wxSize( 0, m_bitmap.GetHeight() ) );
        m_sizerBmpAndPage->SetMinSize( pageSize );
    }
}
//=============================================================================
// WizardEvent
//=============================================================================
WizardEvent::WizardEvent( wxEventType type, int id, bool direction,
                                  WizardPageSimple *page )
:
wxNotifyEvent   ( type, id ),
m_direction     ( direction ),
m_page          ( page )
{
}

wxEvent *WizardEvent::Clone() const
{
    return new WizardEvent( *this );
}

WizardPageSimple *WizardEvent::GetPage() const
{
    return m_page;
}

bool WizardEvent::GetDirection() const
{
    return m_direction;
}
