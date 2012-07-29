///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/aui/tbarart.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/06/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_AUI_TBARART_H__
#define __WXGUIDESIGNER_GUI_AUI_TBARART_H__

class wxAuiDefaultToolBarArt;
class wxDC;
class wxWindow;
class wxRect;

namespace wxGD
{
class AUIToolBarArt : public wxAuiDefaultToolBarArt
{
public:
    AUIToolBarArt()          {}
    virtual ~AUIToolBarArt() {}

    virtual void DrawBackground( wxDC &dc, wxWindow *wnd, const wxRect &rect );

    virtual void DrawOverflowButton( wxDC &dc, wxWindow *wnd,
                                     const wxRect &rect, int state );
};
};

#endif //__WXGUIDESIGNER_GUI_AUI_TBARART_H__
