///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/wizard.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/01
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_TOPLEVEL_WIZARD_H__
#define __WXGUIDESIGNER_GUI_TOPLEVEL_WIZARD_H__

#include <vector>

class wxBitmap;
class wxBoxSizer;
class wxButton;
class wxCommandEvent;
class wxDialog;
class wxPanel;
class wxStaticBitmap;
class wxStaticLine;

class Dialog;
class Wizard;
class WizardPageSimple;
class WizardEvent;

wxDECLARE_EVENT( wxGD_EVT_WIZARD_PAGE_CHANGED,  WizardEvent );
wxDECLARE_EVENT( wxGD_EVT_WIZARD_PAGE_CHANGING, WizardEvent );
wxDECLARE_EVENT( wxGD_EVT_WIZARD_CANCEL,        WizardEvent );
wxDECLARE_EVENT( wxGD_EVT_WIZARD_HELP,          WizardEvent );
wxDECLARE_EVENT( wxGD_EVT_WIZARD_FINISHED,      WizardEvent );
wxDECLARE_EVENT( wxGD_EVT_WIZARD_PAGE_SHOWN,    WizardEvent );

class WizardPage : public wxPanel
{
public:
    WizardPage() { Init(); }

    WizardPage( Wizard *parent, const wxBitmap &bitmap = wxNullBitmap );

    bool Create( Wizard *parent, const wxBitmap& bitmap = wxNullBitmap );

    virtual WizardPage *GetPrev() const = 0;
    virtual WizardPage *GetNext() const = 0;

    virtual wxBitmap GetBitmap() const { return m_bitmap; }

#if wxUSE_VALIDATORS
    virtual bool TransferDataToWindow()
    {
        return GetValidator() ? GetValidator()->TransferToWindow()
                              : wxPanel::TransferDataToWindow();
    }

    virtual bool TransferDataFromWindow()
    {
        return GetValidator() ? GetValidator()->TransferFromWindow()
                              : wxPanel::TransferDataFromWindow();
    }

    virtual bool Validate()
    {
        return GetValidator() ? GetValidator()->Validate(this)
                              : wxPanel::Validate();
    }
#endif // wxUSE_VALIDATORS

protected:
    void Init();

    wxBitmap m_bitmap;

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY( WizardPage )
};
//=============================================================================
// WizardPageSimple
//=============================================================================
class WizardPageSimple : public WizardPage 
{
public:
    WizardPageSimple() { Init(); }

    WizardPageSimple(   Wizard          *parent,
                        WizardPage      *prev   = NULL,
                        WizardPage      *next   = NULL,
                        const wxBitmap  &bitmap = wxNullBitmap )
    {
        Create(parent, prev, next, bitmap);
    }

    bool Create(Wizard          *parent = NULL,
                WizardPage      *prev   = NULL,
                WizardPage      *next   = NULL,
                const wxBitmap  &bitmap = wxNullBitmap)
    {
        m_prev = prev;
        m_next = next;
        return WizardPage::Create( parent, bitmap );
    }

    void SetPrev( WizardPage *prev ) { m_prev = prev; }
    void SetNext( WizardPage *next ) { m_next = next; }

    static void Chain( WizardPageSimple *first, WizardPageSimple *second )
    {
        wxCHECK_RET( first && second,
                     wxT("NULL passed to WizardPageSimple::Chain") );

        first->SetNext( second );
        second->SetPrev( first );
    }

    virtual WizardPage *GetPrev() const;
    virtual WizardPage *GetNext() const;

private:
    void Init()
    {
        m_prev = m_next = NULL;
    }

    WizardPage  *m_prev,
                *m_next;

    DECLARE_DYNAMIC_CLASS_NO_COPY( WizardPageSimple )
};
//=============================================================================
// Wizard
//=============================================================================
class Wizard : public Dialog 
{
public:
    Wizard() { Init(); }

    Wizard( wxWindow        *parent,
            int             id      = wxID_ANY,
            const wxString  &title  = wxEmptyString,
            const wxBitmap  &bitmap = wxNullBitmap,
            const wxPoint   &pos    = wxDefaultPosition,
            long            style   = wxDEFAULT_DIALOG_STYLE )
    {
        Init();

        Create( parent, id, title, bitmap, pos, style );
    }

    bool Create( wxWindow       *parent,
                 int            id      = wxID_ANY,
                 const wxString &title  = wxEmptyString,
                 const wxBitmap &bitmap = wxNullBitmap,
                 const wxPoint  &pos    = wxDefaultPosition,
                 long           style   = wxDEFAULT_DIALOG_STYLE );

    virtual ~Wizard();

    void SetSelection   ( size_t pageIndex );
    void SetBitmap      ( const wxBitmap &bitmap );
    void AddPage        ( WizardPage *page );

    size_t              GetPageCount()                      const;
    size_t              GetPageIndex( WizardPage *page )    const;
    wxBoxSizer          *GetPageSizer()                     const;
    WizardPage          *GetPage( size_t index )            const;

private:
    void Init();

    void OnBackOrNext   ( wxCommandEvent  &event );
    void OnHelp         ( wxCommandEvent  &event );
    void OnCancel       ( wxCommandEvent  &event );
    void OnWizEvent     ( WizardEvent &event );

    typedef std::vector< WizardPage* > WizardPages;

    wxBitmap                m_bitmap;
    wxBoxSizer      *m_clientSizer;
    wxBoxSizer              *m_sizerBmpAndPage;
    wxBoxSizer              *m_sizerPage;
    wxButton                *m_help;
    wxButton                *m_prev;
    wxButton                *m_next;
    wxButton                *m_canc;
    wxStaticBitmap          *m_staticBmp;
    WizardPage              *m_page;
    WizardPages             m_pages;

    DECLARE_DYNAMIC_CLASS( Wizard )
};
//=============================================================================
// WizardEvent
//=============================================================================
class WizardEvent : public wxNotifyEvent
{
public:
    WizardEvent(wxEventType type      = wxEVT_NULL,
                int         id        = wxID_ANY,
                bool        direction = true,
                WizardPage *page      = NULL);

    virtual wxEvent         *Clone()        const;
    WizardPage              *GetPage()      const;
    bool                    GetDirection()  const;

private:
    bool                    m_direction;
    WizardPage              *m_page;
};

#endif //__WXGUIDESIGNER_GUI_TOPLEVEL_WIZARD_H__
