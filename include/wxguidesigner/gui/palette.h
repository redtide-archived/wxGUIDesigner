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

class wxEvent;
class wxNotebook;
class wxAuiToolBar;
class wxWindow;
class wxString;
class wxBitmap;
class wxCommandEvent;

class wxGDHandler;

class wxGDToolPalette : public wxNotebook
{
public:
    wxGDToolPalette( wxGDHandler *handler, wxWindow* parent );
    ~wxGDToolPalette();

    wxAuiToolBar *AddGroup( const wxString &label, const wxBitmap &bitmap );

private:
    void OnToolClicked( wxCommandEvent &event );

    wxGDHandler *m_handler;
};

#endif //__WXGUIDESIGNER_GUI_PALETTE_H__
