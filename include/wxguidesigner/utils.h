///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/utils.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_UTILS_H__
#define __WXGUIDESIGNER_UTILS_H__

#include <wx/stdpaths.h>
#include <wx/settings.h>
#include <wx/window.h>

class wxColour;
class wxFontContainer;
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
    wxColour        GetSystemColour     ( const wxString &name );
    wxInt32         GetSystemColourIndex( const wxString &name );
    wxFont          GetSystemFont       ( const wxString &name );

    int             StringToHex         ( const wxString &value );
    int             StringToInt         ( const wxString &value );
    wxFontContainer StringToFont        ( const wxString &value );
//  Colour          StringToColourInfo  ( const wxString &value );

    wxString        AnyToString         ( const wxAny &any );
    wxString        BoolToString        ( bool value );
    wxString        SystemColourToString( wxInt32 index );
    wxString        ColourToString      ( const wxColour &colour, wxInt32 type );
    wxString        FloatToString       ( double value );
    wxString        FontToString        ( const wxFontContainer &font );
    wxString        IntToString         ( int value );
    wxString        SizeToString        ( const wxSize &size );
    wxString        PointToString       ( const wxPoint &point );
};

#endif //__WXGUIDESIGNER_UTILS_H__
