///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/editbook/designer.h
// Purpose:
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/06/01
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_DESIGNER_H__
#define __WXGUIDESIGNER_GUI_DESIGNER_H__

#include <wx/scrolwin.h>

class wxDC;
class wxPanel;
class wxMouseEvent;
class wxPaintEvent;
class wxStaticBitmap;
class wxStaticText;
class wxXmlNode;

class wxGlossyButton;

namespace wxGD
{
namespace Editor
{
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
                const wxString  &name     = "titlebar");

    void SetTitleLabel( const wxString &label );

private:
    void Init();
    void OnPaint( wxPaintEvent &event );

    wxStaticText    *m_titleLbl;
    wxStaticBitmap  *m_titleBmp;
    wxGlossyButton  *m_titleBmpMin;
    wxGlossyButton  *m_titleBmpMax;
    wxGlossyButton  *m_titleBmpClose;

    DECLARE_DYNAMIC_CLASS( TitleBar )
};
//=============================================================================
// Resizer
//=============================================================================
class Designer;

class Resizer : public wxPanel
{
public:
    Resizer( Designer *parent );
    ~Resizer();

private:
    void OnMouseMotion  ( wxMouseEvent &event );
    void OnLeftDown     ( wxMouseEvent &event );
    void OnLeftUp       ( wxMouseEvent &event );

    enum
    {
        NONE,
        RIGHT,
        BOTTOM,
        RIGHTBOTTOM
    }   m_sizing;

    int         m_curX, m_curY, m_difX, m_difY;
    int         m_resizeBorder;
    wxSize      m_minSize;
    wxSize      m_baseMinSize;
    Designer    *m_editor;
};
//=============================================================================
// wxGD::Editor::Designer: Scrolled window, the main visual editor page
//=============================================================================
class Designer : public wxScrolled< wxPanel >
{
public:
    Designer( wxWindow *parent );
    ~Designer();

    void Cleanup();
    void ShowTitleBar( bool show = true );

    void UpdateDesigner( const wxString &topClassName, const wxString &topName );
    void UpdateVirtualSize();

private:

    wxPanel     *m_border;
    wxPanel     *m_client;
    wxPanel     *m_designer;
    Resizer     *m_resizer;
    TitleBar    *m_title;
};

}; // namespace Editor
}; // namespace wxGD

#endif //__WXGUIDESIGNER_GUI_DESIGNER_H__
