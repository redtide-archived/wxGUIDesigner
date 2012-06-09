///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/utils.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/20
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_UTILS_H__
#define __WXGUIDESIGNER_UTILS_H__

#include "wxguidesigner/defs.h"

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
#ifdef __WXMSW__
    return wxStandardPaths::Get().GetDataDir() + wxFILE_SEP_PATH + "db";
#else
    return wxStandardPaths::Get().GetResourcesDir().BeforeLast( wxFILE_SEP_PATH ) +
                                                wxFILE_SEP_PATH + "wxguidesigner" +
                                                wxFILE_SEP_PATH + "db";
#endif
}

inline wxString GetResourcePath()
{
#ifdef __WXMSW__
    return wxStandardPaths::Get().GetDataDir() + wxFILE_SEP_PATH;
#else
    return wxStandardPaths::Get().GetResourcesDir().BeforeLast( wxFILE_SEP_PATH ) +
                            wxFILE_SEP_PATH + "wxguidesigner" + wxFILE_SEP_PATH;
#endif
}

namespace wxGDConv
{
    const wxString  HexToString         ( int value );
    const int       StringToHex         ( const wxString &text );
    const ClassType ClassTypeFromString ( const wxString &value );
    const int       IntFromString       ( const wxString &value );
    const bool      IsDark              ( const wxColour &colour );
    const wxColour  GetSystemColour     ( const wxString &name );
    const wxInt32   GetSystemColourIndex( const wxString &name );
    const wxFont    GetSystemFont       ( const wxString &name );
    const Colour    StringToColourInfo  ( const wxString &value );
};

#endif //__WXGUIDESIGNER_UTILS_H__
