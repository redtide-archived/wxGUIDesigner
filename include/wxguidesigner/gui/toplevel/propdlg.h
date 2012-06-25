///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/propdlg.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/24
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_TOPLEVEL_PROPDLG_H__
#define __WXGUIDESIGNER_GUI_TOPLEVEL_PROPDLG_H__

#include <wx/defs.h>
#include <wx/dialog.h>

#if wxUSE_BOOKCTRL

class wxActivateEvent;
class wxBookCtrlBase;
class wxIdleEvent;
class wxPoint;
class wxSize;
class wxSizer;
class wxString;
class wxWindow;

class Dialog;

class PropertySheetDialog : public Dialog
{
public:
    PropertySheetDialog() { Init(); }

    PropertySheetDialog(wxWindow*       parent,
                        wxWindowID      id,
                        const wxString  &title,
                        const wxPoint   &pos  = wxDefaultPosition,
                        const wxSize    &size = wxDefaultSize,
                        long            style = wxDEFAULT_DIALOG_STYLE,
                        const wxString  &name = "gd_propdlg")
    {
        Init();
        Create( parent, id, title, pos, size, style, name );
    }

    virtual ~PropertySheetDialog() {}

    bool Create(wxWindow*       parent,
                wxWindowID      id,
                const wxString  &title,
                const wxPoint   &pos  = wxDefaultPosition,
                const wxSize    &size = wxDefaultSize,
                long            style = wxDEFAULT_DIALOG_STYLE,
                const wxString  &name = "gd_propdlg");

    void                    SetBookCtrl(wxBookCtrlBase* book);
    wxBookCtrlBase          *GetBookCtrl() const;

    virtual wxWindow        *GetContentWindow() const;

    void                    SetInnerSize(wxSizer* sizer);
    wxSizer                 *GetInnerSizer() const;

    void                    SetSheetStyle(long sheetStyle);
    long                    GetSheetStyle() const;

    void                    SetSheetOuterBorder(int border);
    int                     GetSheetOuterBorder() const;

    void                    SetSheetInnerBorder(int border);
    int                     GetSheetInnerBorder() const;

    virtual void            CreateButtons( int flags = wxOK | wxCANCEL );
    virtual void            LayoutDialog( int centreFlags = wxBOTH );
    virtual wxBookCtrlBase *CreateBookCtrl();
    virtual void            AddBookCtrl( wxSizer *sizer );

    void OnActivate ( wxActivateEvent   &event );
    void OnIdle     ( wxIdleEvent       &event );

private:
    void Init();

    wxBookCtrlBase  *m_bookCtrl;
    wxSizer         *m_innerSizer;
    long            m_sheetStyle;
    int             m_sheetOuterBorder;
    int             m_sheetInnerBorder;
    int             m_selectedPage;

    DECLARE_DYNAMIC_CLASS( PropertySheetDialog )
    DECLARE_EVENT_TABLE()
};

#endif // wxUSE_BOOKCTRL

#endif //__WXGUIDESIGNER_GUI_TOPLEVEL_PROPDLG_H__
