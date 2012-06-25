///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/propdlg.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/24
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/intl.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/sysopt.h>

#include <wx/generic/propdlg.h>

#include <wx/bookctrl.h>
#include <wx/choicebk.h>
#include <wx/listbook.h>
#include <wx/notebook.h>
#include <wx/toolbook.h>
#include <wx/treebook.h>

#include "wxguidesigner/gui/toplevel/toplevel.h"
#include "wxguidesigner/gui/toplevel/dialog.h"
#include "wxguidesigner/gui/toplevel/propdlg.h"

IMPLEMENT_DYNAMIC_CLASS( PropertySheetDialog, Dialog )

BEGIN_EVENT_TABLE( PropertySheetDialog, Dialog )
    EVT_ACTIVATE( PropertySheetDialog::OnActivate )
    EVT_IDLE( PropertySheetDialog::OnIdle )
END_EVENT_TABLE()

void PropertySheetDialog::Init()
{
    m_sheetStyle = wxPROPSHEET_DEFAULT;
    m_innerSizer = NULL;
    m_bookCtrl = NULL;
    m_sheetOuterBorder = 2;
    m_sheetInnerBorder = 5;
}

bool PropertySheetDialog::Create  ( wxWindow       *parent,
                                    wxWindowID      id,
                                    const wxString  &title,
                                    const wxPoint   &pos,
                                    const wxSize    &sz,
                                    long            style,
                                    const wxString  &name )
{
//  parent = GetParentForModalDialog(parent, style);

    if(!Dialog::Create(parent, id, title, pos, sz, style|wxCLIP_CHILDREN, name))
        return false;

    // This gives more space around the edges
    m_innerSizer = new wxBoxSizer( wxVERTICAL );

#if defined(__SMARTPHONE__) || defined(__POCKETPC__)
    m_sheetOuterBorder = 0;
#endif
    GetClientSizer()->Add(m_innerSizer, 1, wxGROW|wxALL, m_sheetOuterBorder);

    m_bookCtrl = CreateBookCtrl();
    AddBookCtrl(m_innerSizer);

    return true;
}

void PropertySheetDialog::SetBookCtrl( wxBookCtrlBase *book )
{
    m_bookCtrl = book;
}

wxBookCtrlBase *PropertySheetDialog::GetBookCtrl() const
{
    return m_bookCtrl;
}

wxWindow *PropertySheetDialog::GetContentWindow() const
{
    return GetBookCtrl();
}

void PropertySheetDialog::SetInnerSize( wxSizer *sizer )
{
    m_innerSizer = sizer;
}

wxSizer *PropertySheetDialog::GetInnerSizer() const
{
    return m_innerSizer;
}

void PropertySheetDialog::SetSheetStyle( long sheetStyle )
{
    m_sheetStyle = sheetStyle;
}

long PropertySheetDialog::GetSheetStyle() const
{
    return m_sheetStyle;
}

void PropertySheetDialog::SetSheetOuterBorder( int border )
{
    m_sheetOuterBorder = border;
}

int PropertySheetDialog::GetSheetOuterBorder() const
{
    return m_sheetOuterBorder;
}

void PropertySheetDialog::SetSheetInnerBorder( int border )
{
    m_sheetInnerBorder = border;
}

int PropertySheetDialog::GetSheetInnerBorder() const
{
    return m_sheetInnerBorder;
}

void PropertySheetDialog::CreateButtons( int flags )
{
#ifdef __POCKETPC__
    // keep system option status
    const wxChar *optionName = wxT("wince.dialog.real-ok-cancel");
    const int status = wxSystemOptions::GetOptionInt(optionName);
    wxSystemOptions::SetOption(optionName,0);
#endif

    wxSizer *buttonSizer = CreateButtonSizer(flags);
    if( buttonSizer )
    {
        m_innerSizer->Add( buttonSizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT|wxRIGHT, 2);
        m_innerSizer->AddSpacer(2);
    }

#ifdef __POCKETPC__
    // restore system option
    wxSystemOptions::SetOption(optionName,status);
#endif
}

void PropertySheetDialog::LayoutDialog( int centreFlags )
{
#if !defined(__SMARTPHONE__) && !defined(__POCKETPC__)
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    if( centreFlags )
        Centre(centreFlags);
#else
    wxUnusedVar(centreFlags);
#endif
#if defined(__SMARTPHONE__)
    if( m_bookCtrl )
        m_bookCtrl->SetFocus();
#endif
}

wxBookCtrlBase *PropertySheetDialog::CreateBookCtrl()
{
    int style = wxCLIP_CHILDREN | wxBK_DEFAULT;

    wxBookCtrlBase* bookCtrl = NULL;

#if wxUSE_NOTEBOOK
    if(GetSheetStyle() & wxPROPSHEET_NOTEBOOK)
        bookCtrl = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
#endif
#if wxUSE_CHOICEBOOK
    if(GetSheetStyle() & wxPROPSHEET_CHOICEBOOK)
        bookCtrl = new wxChoicebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
#endif
#if wxUSE_TOOLBOOK
#if defined(__WXMAC__) && wxUSE_TOOLBAR && wxUSE_BMPBUTTON
    if(GetSheetStyle() & wxPROPSHEET_BUTTONTOOLBOOK)
        bookCtrl = new wxToolbook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style|wxTBK_BUTTONBAR );
    else
#endif
    if((GetSheetStyle() & wxPROPSHEET_TOOLBOOK) || (GetSheetStyle() & wxPROPSHEET_BUTTONTOOLBOOK))
        bookCtrl = new wxToolbook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
#endif
#if wxUSE_LISTBOOK
    if(GetSheetStyle() & wxPROPSHEET_LISTBOOK)
        bookCtrl = new wxListbook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
#endif
#if wxUSE_TREEBOOK
    if(GetSheetStyle() & wxPROPSHEET_TREEBOOK)
        bookCtrl = new wxTreebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
#endif
    if(!bookCtrl)
        bookCtrl = new wxBookCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );

    if(GetSheetStyle() & wxPROPSHEET_SHRINKTOFIT)
        bookCtrl->SetFitToCurrentPage(true);

    return bookCtrl;
}

void PropertySheetDialog::AddBookCtrl( wxSizer *sizer )
{
#if defined(__POCKETPC__) && wxUSE_NOTEBOOK
    // The book control has to be sized larger than the dialog because of a border bug
    // in WinCE
    int borderSize = -2;
    sizer->Add( m_bookCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxRIGHT, borderSize );
#else
    sizer->Add( m_bookCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, m_sheetInnerBorder );
#endif
}

void PropertySheetDialog::OnActivate( wxActivateEvent &event )
{
#if defined(__SMARTPHONE__)
    // Attempt to focus the choice control: not yet working, but might
    // be a step in the right direction. OnActivate overrides the default
    // handler in toplevel.cpp that sets the focus for the first child of
    // of the dialog (the choicebook).
    if( event.GetActive() )
    {
        wxChoicebook* choiceBook = wxDynamicCast(GetBookCtrl(), wxChoicebook);
        if( choiceBook )
            choiceBook->SetFocus();
    }
    else
#endif
        event.Skip();
}

void PropertySheetDialog::OnIdle( wxIdleEvent &event )
{
    event.Skip();

    if( (GetSheetStyle() & wxPROPSHEET_SHRINKTOFIT) && GetBookCtrl() )
    {
        int sel = GetBookCtrl()->GetSelection();
        if( sel != -1 && sel != m_selectedPage )
        {
            GetBookCtrl()->InvalidateBestSize();
            InvalidateBestSize();
            SetSizeHints(-1, -1, -1, -1);

            m_selectedPage = sel;
            LayoutDialog(0);
        }
    }
}
