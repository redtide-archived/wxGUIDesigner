///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/dialog.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/24
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/button.h>
#include <wx/defs.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include "wxguidesigner/gui/toplevel/toplevel.h"
#include "wxguidesigner/gui/toplevel/dialog.h"

IMPLEMENT_DYNAMIC_CLASS( Dialog, TopLevelWindow )

bool Dialog::Create( wxWindow *parent, wxWindowID id, const wxString& title,
                    const wxPoint& pos, const wxSize& size, long style,
                    const wxString& name )
{
    if( !TopLevelWindow::Create( parent, id, title, pos, size, style, name ) )
        return false;

    return true;
}

wxSizer *Dialog::CreateButtonSizer( long flags )
{
#ifdef __SMARTPHONE__
    wxDialog* dialog = (wxDialog*) this;
    if ( flags & wxOK )
        dialog->SetLeftMenu(wxID_OK);

    if ( flags & wxCANCEL )
        dialog->SetRightMenu(wxID_CANCEL);

    if ( flags & wxYES )
        dialog->SetLeftMenu(wxID_YES);

    if ( flags & wxNO )
        dialog->SetRightMenu(wxID_NO);

    return NULL;
#else // !__SMARTPHONE__

#if wxUSE_BUTTON

#ifdef __POCKETPC__
    // PocketPC guidelines recommend for Ok/Cancel dialogs to use OK button
    // located inside caption bar and implement Cancel functionality through
    // Undo outside dialog. As native behaviour this will be default here but
    // can be replaced with real wxButtons by setting the option below to 1
    if ( (flags & ~(wxCANCEL|wxNO_DEFAULT)) != wxOK ||
            wxSystemOptions::GetOptionInt(wxT("wince.dialog.real-ok-cancel")) )
#endif // __POCKETPC__
    {
        return CreateStdDialogButtonSizer(flags);
    }
#ifdef __POCKETPC__
    return NULL;
#endif // __POCKETPC__

#else // !wxUSE_BUTTON
    wxUnusedVar(flags);

    return NULL;
#endif // wxUSE_BUTTON/!wxUSE_BUTTON

#endif // __SMARTPHONE__/!__SMARTPHONE__
}

wxStdDialogButtonSizer *Dialog::CreateStdDialogButtonSizer( long flags )
{
    wxStdDialogButtonSizer *sizer = new wxStdDialogButtonSizer();

    wxButton *ok  = NULL;
    wxButton *yes = NULL;
    wxButton *no  = NULL;

    if (flags & wxOK)
    {
        ok = new wxButton(this, wxID_OK);
        sizer->AddButton(ok);
    }

    if (flags & wxCANCEL)
    {
        wxButton *cancel = new wxButton(this, wxID_CANCEL);
        sizer->AddButton(cancel);
    }

    if (flags & wxYES)
    {
        yes = new wxButton(this, wxID_YES);
        sizer->AddButton(yes);
    }

    if (flags & wxNO)
    {
        no = new wxButton(this, wxID_NO);
        sizer->AddButton(no);
    }

    if (flags & wxAPPLY)
    {
        wxButton *apply = new wxButton(this, wxID_APPLY);
        sizer->AddButton(apply);
    }

    if (flags & wxCLOSE)
    {
        wxButton *close = new wxButton(this, wxID_CLOSE);
        sizer->AddButton(close);
    }

    if (flags & wxHELP)
    {
        wxButton *help = new wxButton(this, wxID_HELP);
        sizer->AddButton(help);
    }

    if (flags & wxNO_DEFAULT)
    {
        if (no)
        {
            no->SetDefault();
            no->SetFocus();
        }
    }
    else
    {
        if (ok)
        {
            ok->SetDefault();
            ok->SetFocus();
        }
        else if (yes)
        {
            yes->SetDefault();
            yes->SetFocus();
        }
    }

    if (flags & wxOK)
        SetAffirmativeId(wxID_OK);
    else if (flags & wxYES)
        SetAffirmativeId(wxID_YES);

    sizer->Realize();

    return sizer;
}

void Dialog::SetAffirmativeId( int affirmativeId )
{
    m_affirmativeId = affirmativeId;
}
int Dialog::GetAffirmativeId() const
{
    return m_affirmativeId;
}
