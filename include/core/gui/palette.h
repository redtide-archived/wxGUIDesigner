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

class PaletteHandler
{
public:
    PaletteHandler( wxNotebook *owner );

    wxToolGroup *AddGroup( const wxString &label, const wxBitmap &bitmap );

private:
    void OnToolClicked( wxCommandEvent &event );

    wxVector< wxToolGroup * >  m_toolGroups;
    wxNotebook                  *m_palette;

    friend class GUIManager;
};

#endif //__CORE_PALETTE_H__
