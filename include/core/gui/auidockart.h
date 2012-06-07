///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/auidockart.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/06/03
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGD_GUI_AUIDOCKART_H__
#define __WXGD_GUI_AUIDOCKART_H__

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

#endif //__WXGD_GUI_AUIDOCKART_H__
