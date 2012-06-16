///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/auidockart.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/06/03
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_AUIDOCKART_H__
#define __WXGUIDESIGNER_GUI_AUIDOCKART_H__

#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>

class wxGDAUIDockArt : public wxAuiDefaultDockArt
{
public:
    wxGDAUIDockArt();
    virtual ~wxGDAUIDockArt();

    void DrawCaption( wxDC& dc, wxWindow *window, const wxString &text,
                        const wxRect &rect, wxAuiPaneInfo &pane );
private:
    bool IsDark( const wxColour &colour ) const;
};

#endif //__WXGUIDESIGNER_GUI_AUIDOCKART_H__
