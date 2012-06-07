///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/palette.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/23
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_PALETTE_H__
#define __CORE_PALETTE_H__

#include <wx/vector.h>

#if defined(__WXMAC__) || defined( USE_AUIBAR )
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
    void OnToolClicked( wxCommandEvent &event );

    wxGDHandler              *m_handler;
    wxVector< wxToolGroup * > m_toolGroups;
};

#endif //__CORE_PALETTE_H__
