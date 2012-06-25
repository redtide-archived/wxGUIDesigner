///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/designer.h
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

class wxDC;
class wxPanel;
class wxMouseEvent;
class wxPaintEvent;
//=============================================================================
// wxGDResizingPanel
//=============================================================================
class wxGDResizingPanel : public wxPanel
{
public:
    wxGDResizingPanel( wxWindow *parent, wxWindowID id  = wxID_ANY,
                        const wxPoint &position = wxPoint( 9,9 ),
                        const wxSize &size = wxDefaultSize,
                        long style = wxFULL_REPAINT_ON_RESIZE );
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

    int m_curX, m_curY, m_difX, m_difY;
    int m_resizeBorder;
    wxSize m_minSize;
    wxSize m_baseMinSize;
};

#endif //__WXGUIDESIGNER_GUI_DESIGNER_H__
