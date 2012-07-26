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
//=============================================================================
// wxGDTitleBar
//=============================================================================
class wxGDTitleBar : public wxPanel
{
public:
    wxGDTitleBar();
    wxGDTitleBar  ( wxWindow        *parent,
                    wxWindowID      id        = wxID_ANY,
                    const wxPoint   &position = wxDefaultPosition,
                    const wxSize    &size     = wxDefaultSize,
                    long            style     = 0,
                    const wxString  &name     = "titlebar" );
    ~wxGDTitleBar();

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

    DECLARE_DYNAMIC_CLASS( wxGDTitleBar )
};
//=============================================================================
// wxGDResizingPanel
//=============================================================================
class wxGDEditor;

class wxGDResizingPanel : public wxPanel
{
public:
    wxGDResizingPanel( wxGDEditor *parent );
    ~wxGDResizingPanel();

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
    wxGDEditor  *m_editor;
};
//=============================================================================
// wxGDEditor: Scrolled window, the main visual editor page
//=============================================================================
class wxGDEditor : public wxScrolled< wxPanel >
{
public:
    wxGDEditor( wxWindow *parent );
    ~wxGDEditor();

    void Cleanup();
    void ShowTitleBar( bool show = true );

    void UpdateDesigner( const wxString &topClassName, const wxString &topName );
    void UpdateVirtualSize();

private:

    wxPanel             *m_border;
    wxPanel             *m_client;
    wxPanel             *m_designer;
    wxGDResizingPanel   *m_resizer;
    wxGDTitleBar        *m_title;
};

#endif //__WXGUIDESIGNER_GUI_DESIGNER_H__
