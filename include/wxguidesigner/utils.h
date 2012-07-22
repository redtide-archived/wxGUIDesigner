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

namespace wxGD
{
    namespace Convert
    {
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
//      int             StringToHex                 ( const wxString &value );
        int             StringToFlag                ( const wxString &value );
        int             StringToInteger             ( const wxString &value );
        int             StringToSystemColourIndex   ( const wxString &value );
        bool            StringToBool                ( const wxString &value );
        double          StringToFloat               ( const wxString &value );
        wxArrayString   StringToArrayString         ( const wxString &value );
        wxColour        StringToColour              ( const wxString &value );
        wxColour        StringToSystemColour        ( const wxString &value );
        wxFontContainer StringToFont                ( const wxString &value );
        wxFont          StringToSystemFont          ( const wxString &value );
        wxPoint         StringToPoint               ( const wxString &value );
        wxSize          StringToSize                ( const wxString &value );
//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------
        wxString        IntegerToString       ( int     value );
//      wxString        FlagToString          ( int     value );
        wxString        BoolToString          ( bool    value );
        wxString        FloatToString         ( double  value );
        wxString        SystemColourToString  ( int     index );
        wxString        ColourToString        ( const wxColour &colour,
                                                int colourType );
        wxString        ArrayStringToString   ( const wxArrayString     &value );
        wxString        FontToString          ( const wxFontContainer   &value );
        wxString        PointToString         ( const wxPoint           &value );
        wxString        SizeToString          ( const wxSize            &value );
    };
};

#endif //__WXGUIDESIGNER_UTILS_H__
