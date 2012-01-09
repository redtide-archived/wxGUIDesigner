///////////////////////////////////////////////////////////////////////////////
// Name:        core/utils.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_UTILS_H__
#define __WXGDCORE_UTILS_H__

#include <wx/window.h>

class wxColour;
class wxString;

inline wxWindow *wxFindWindowById( long winid, const wxWindow *parent = NULL )
{
    return wxWindow::FindWindowById( winid, parent );
}

namespace wxGDConv
{
    const int       StringToHex( const wxString &text );
    const wxString  HexToString( int value );
    const wxColour  GetSystemColour( const wxString &name );
};

#endif //__WXGDCORE_UTILS_H__
