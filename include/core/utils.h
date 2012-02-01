///////////////////////////////////////////////////////////////////////////////
// Name:        core/utils.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_UTILS_H__
#define __CORE_UTILS_H__

#include "core/defs.h"

#include <wx/stdpaths.h>
#include <wx/window.h>

class wxColour;
class wxString;

inline wxWindow *wxFindWindowById( long winid, const wxWindow *parent = NULL )
{
    return wxWindow::FindWindowById( winid, parent );
}

inline wxString GetDataBasePath()
{
    return wxStandardPaths::Get().GetResourcesDir() + wxFILE_SEP_PATH + "db";
}

namespace wxGDConv
{
    const int       StringToHex( const wxString &text );
    const wxString  HexToString( int value );
    const ClassType ClassTypeFromString( const wxString &value );
    const int       IntFromString( const wxString &value );
    const wxColour  GetSystemColour( const wxString &name );
};

#endif //__CORE_UTILS_H__
