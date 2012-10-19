///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/convert.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_RTTI_CONVERT_H__
#define __WXGUIDESIGNER_RTTI_CONVERT_H__

class wxColour;
class wxFontContainer;
class wxString;

namespace RTTI
{
namespace Convert
{
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
    ClassType       StringToClassType           ( const wxString &value );
//  int             StringToHex                 ( const wxString &value );
    int             StringToFlag                ( const wxString &value );
    int             StringToInteger             ( const wxString &value );
    int             StringToSystemColourIndex   ( const wxString &value );
    bool            StringToBool                ( const wxString &value );
    double          StringToFloat               ( const wxString &value );
    wxArrayString   StringToArrayString         ( const wxString &value );
    wxColour        StringToSystemColour        ( const wxString &value );
    wxFontContainer StringToFont                ( const wxString &value );
    wxFont          StringToSystemFont          ( const wxString &value );
    wxPoint         StringToPoint               ( const wxString &value );
    wxSize          StringToSize                ( const wxString &value );
//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------
    wxString        IntegerToString       ( int     value );
//  wxString        FlagToString          ( int     value );
    wxString        BoolToString          ( bool    value );
    wxString        FloatToString         ( double  value );
    wxString        SystemColourToString  ( int     index );
    wxString        ColourToString        ( const wxColour &colour,
                                            int colourType );
    wxString        ArrayStringToString   ( const wxArrayString     &value );
    wxString        FontToString          ( const wxFontContainer   &value );
    wxString        PointToString         ( const wxPoint           &value );
    wxString        SizeToString          ( const wxSize            &value );

}; // namespace Convert
}; // namespace RTTI

#endif //__WXGUIDESIGNER_RTTI_CONVERT_H__
