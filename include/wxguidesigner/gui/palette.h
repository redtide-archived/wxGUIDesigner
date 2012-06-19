///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/palette.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/23
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PALETTE_H__
#define __WXGUIDESIGNER_GUI_PALETTE_H__

#if defined(__WXMAC__) || defined( wxGD_USE_AUIBAR )
    #include <wx/aui/auibar.h>
    typedef wxAuiToolBar wxToolGroup;
#else
    #include <wx/toolbar.h>
    typedef wxToolBar wxToolGroup;
#endif

class wxEvent;
class wxNotebook;

class wxGDToolPalette : public wxNotebook
{
public:
    wxGDToolPalette( wxGDHandler *handler, wxWindow* parent );
    ~wxGDToolPalette();

    wxToolGroup *AddGroup( const wxString &label, const wxBitmap &bitmap );

private:
    void        OnToolClicked( wxCommandEvent &event );

    wxGDHandler *m_handler;
};

#endif //__WXGUIDESIGNER_GUI_PALETTE_H__
