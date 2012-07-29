///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/dialog.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/24
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_TOPLEVEL_DIALOG_H__
#define __WXGUIDESIGNER_GUI_TOPLEVEL_DIALOG_H__

#include <wx/dialog.h>

class wxPoint;
class wxSize;
class wxSizer;
class wxString;
class wxWindow;

class TopLevelWindow;

class Dialog : public TopLevelWindow 
{
public:
    Dialog() {}
    Dialog( wxWindow        *parent,
            wxWindowID      id,
            const wxString  &title,
            const wxPoint   &pos    = wxDefaultPosition,
            const wxSize    &size   = wxDefaultSize,
            long            style   = wxDEFAULT_DIALOG_STYLE,
            const wxString  &name   = "gd_dialog" )
    {
        Create( parent, id, title, pos, size, style, name );
    }

    virtual ~Dialog() {}

    bool Create(wxWindow        *parent,
                wxWindowID      id,
                const wxString  &title,
                const wxPoint   &pos    = wxDefaultPosition,
                const wxSize    &size   = wxDefaultSize,
                long            style   = wxDEFAULT_DIALOG_STYLE,
                const wxString  &name   = "gd_dialog");

    wxSizer                 *CreateButtonSizer          ( long flags );
    wxStdDialogButtonSizer  *CreateStdDialogButtonSizer ( long flags );

    void                    SetAffirmativeId( int affirmativeId );
    int                     GetAffirmativeId() const;

private:
    int m_affirmativeId;

    DECLARE_DYNAMIC_CLASS( Dialog )
};

#endif //__WXGUIDESIGNER_GUI_TOPLEVEL_DIALOG_H__
