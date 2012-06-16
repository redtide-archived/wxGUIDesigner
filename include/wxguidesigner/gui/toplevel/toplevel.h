///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/toplevel.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/01
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_TOPLEVEL_TOPLEVEL_H__
#define __WXGUIDESIGNER_GUI_TOPLEVEL_TOPLEVEL_H__

#include <wx/toplevel.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/iconbndl.h>

class TopLevelWindow : public wxPanel
{
public:
    TopLevelWindow() { Init(); }
    TopLevelWindow(wxWindow *parent,
                     wxWindowID id,
                     const wxString& title,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxDEFAULT_FRAME_STYLE,
                     const wxString& name = "toplevelwindow")
    {
        Init();

        Create( parent, id, title, pos, size, style, name );
    }

    virtual ~TopLevelWindow();

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = "toplevelwindow" );

    wxIcon GetIcon() const;
    void SetIcon( const wxIcon &icon );
    virtual void SetIcons( const wxIconBundle& icons );

private:
    void Init() {}

    wxString        m_title;
    wxIconBundle    m_icons;
};

#endif //__WXGUIDESIGNER_GUI_TOPLEVEL_TOPLEVEL_H__
