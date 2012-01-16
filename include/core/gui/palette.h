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

#ifndef __WXGDCORE_PALETTE_H__
#define __WXGDCORE_PALETTE_H__

#include <plugin/plugin.h>
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

class PaletteHandler : public IPluginHandler
{
    friend class GUIManager;

public:
    PaletteHandler( wxNotebook *owner );

    wxToolGroup *AddGroup( const wxString &label, const wxBitmap &bitmap );

    void OnToolClicked( wxCommandEvent &event );

private:
    void OnPluginLoaded( Plugin *plugin );

    wxVector< wxToolGroup * >  m_toolGroups;
    wxNotebook                  *m_palette;
};

#endif //__WXGDCORE_PALETTE_H__
