///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/palette.h
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

#include <wx/bitmap.h>

#ifdef USE_AUIBAR
    #include <wx/aui/auibar.h>
#else
    #include <wx/toolbar.h>
#endif

#include <wx/event.h>
#include <wx/notebook.h>
#include <wx/vector.h>

#include <plugin.h>

class PaletteHandler
{
public:
    PaletteHandler( wxNotebook *owner );

#ifndef USE_AUIBAR
    wxToolBar *
#else
    wxAuiToolBar *
#endif
         AddGroup( const wxString &label, const wxBitmap &bitmap );

    void OnPluginLoaded( PluginEvent &event );
    void OnToolClicked( wxCommandEvent &event );

protected:
#ifdef USE_AUIBAR
    wxVector< wxAuiToolBar * >  m_toolbars;
#else
    wxVector< wxToolBar * >     m_toolbars;
#endif
    wxNotebook                  *m_palette;
};

#endif //__WXGDCORE_PALETTE_H__