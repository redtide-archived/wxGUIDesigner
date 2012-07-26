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

class wxBoxSizer;
class wxIconBundle;
class wxPanel;
class wxPoint;
class wxSize;
class wxString;
class wxWindow;

class TopLevelWindow : public wxPanel
{
public:
    TopLevelWindow();
    TopLevelWindow( wxWindow        *parent,
                    wxWindowID      id,
                    const wxString  &title,
                    const wxPoint   &pos   = wxDefaultPosition,
                    const wxSize    &size  = wxDefaultSize,
                    long            style  = wxDEFAULT_FRAME_STYLE,
                    const wxString  &name  = "toplevelwindow" );

    virtual ~TopLevelWindow();

    bool Create(wxWindow        *parent,
                wxWindowID      id,
                const wxString  &title,
                const wxPoint   &pos   = wxDefaultPosition,
                const wxSize    &size  = wxDefaultSize,
                long            style  = wxDEFAULT_FRAME_STYLE,
                const wxString  &name  = "toplevelwindow" );

    wxIcon GetIcon() const;
    void SetIcon( const wxIcon &icon );
    virtual void SetIcons( const wxIconBundle &icons );

private:
    void Init();

    wxString        m_title;
    wxIconBundle    m_icons;

private:
    DECLARE_DYNAMIC_CLASS( TopLevelWindow )
};

#endif //__WXGUIDESIGNER_GUI_TOPLEVEL_TOPLEVEL_H__
