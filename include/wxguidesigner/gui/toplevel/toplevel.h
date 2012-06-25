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

#include <wx/iconbndl.h>
#include <wx/toplevel.h>

class wxBoxSizer;
class wxGlossyButton;
class wxIconBundle;
class wxPaintEvent;
class wxPanel;
class wxPoint;
class wxSize;
class wxStaticBitmap;
class wxStaticText;
class wxString;
class wxWindow;
//=============================================================================
// TitleBar
//=============================================================================
class TitleBar : public wxPanel
{
public:
    TitleBar();
    TitleBar  ( wxWindow        *parent,
                wxWindowID      id        = wxID_ANY,
                const wxPoint   &position = wxDefaultPosition,
                const wxSize    &size     = wxDefaultSize,
                long            style     = 0,
                const wxString  &name     = "titlebar" );
    ~TitleBar();

    bool Create(wxWindow        *parent,
                wxWindowID      id        = wxID_ANY,
                const wxPoint   &position = wxDefaultPosition,
                const wxSize    &size     = wxDefaultSize,
                long            style     = 0,
                const wxString  &name     = "titlebar" );

    void SetTitleLabel( const wxString &label );

private:
    void Init();
    void OnPaint( wxPaintEvent &event );

    wxStaticText        *m_titleLbl;
    wxStaticBitmap      *m_titleBmp;
    wxGlossyButton      *m_titleBmpMin;
    wxGlossyButton      *m_titleBmpMax;
    wxGlossyButton      *m_titleBmpClose;

    DECLARE_DYNAMIC_CLASS( TitleBar )
};
//=============================================================================
// TopLevelWindow
//=============================================================================
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

    wxBoxSizer *GetClientSizer() const { return m_clientSizer; }

    wxIcon GetIcon() const;
    void SetIcon( const wxIcon &icon );
    virtual void SetIcons( const wxIconBundle &icons );

private:
    void Init();

    wxString        m_title;
    wxIconBundle    m_icons;

    TitleBar        *m_titleBar;
    wxBoxSizer      *m_clientSizer;

private:
    DECLARE_DYNAMIC_CLASS( TopLevelWindow )
};

#endif //__WXGUIDESIGNER_GUI_TOPLEVEL_TOPLEVEL_H__
