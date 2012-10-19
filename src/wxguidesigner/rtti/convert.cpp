///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/utils.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <cstring>
#include <ios>
#include <sstream>

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/tokenzr.h>

#include <wx/log.h>

#include "wxguidesigner/fontcontainer.h"
#include "wxguidesigner/rtti.h"
#include "wxguidesigner/rtti/flags.h"
#include "wxguidesigner/rtti/convert.h"
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
RTTI::ClassType RTTI::Convert::StringToClassType( const wxString &value )
{
    if     ( value == "abstract"  ) return Abstract;
    else if( value == "container" ) return Container;
    else if( value == "custom"    ) return Custom;
    else if( value == "item"      ) return Item;
    else if( value == "layout"    ) return Layout;
    else if( value == "root"      ) return Root;
    else if( value == "sizer"     ) return Sizer;
    else if( value == "toplevel"  ) return TopLevel;
    else if( value == "window"    ) return Window;
    else if( value == "widget"    ) return Widget;

    return 0;
}
/*
int RTTI::Convert::StringToHex( const wxString &text )
{
    std::stringstream s(text.ToStdString());
    int ret;
    s >> std::hex >> ret;
    return ret;
}
*/
int RTTI::Convert::StringToFlag( const wxString &value )
{
    int result = 0;

    wxStringTokenizer tokenizer( value, "|" );
    while( tokenizer.HasMoreTokens() )
    {
        wxString flag = tokenizer.GetNextToken();
        flag.Trim( true );
        flag.Trim( false );

        result |= RTTI::Flags::Get()->Get( flag );
    }

    return result;
}

int RTTI::Convert::StringToInteger( const wxString &value )
{
    if( !value.empty() && value.IsNumber() )
        return wxAtoi( value );

    return 0;
}

bool RTTI::Convert::StringToBool( const wxString &value )
{
    if( value.Lower() == "true" || (StringToInteger( value ) > 0) )
        return true;

    return false;
}

double RTTI::Convert::StringToFloat( const wxString &value )
{
    double result;
    value.ToDouble( &result );
    return result;
}

wxArrayString RTTI::Convert::StringToArrayString( const wxString &value )
{
    return wxStringTokenize( value, "\n" );
}

wxFontContainer RTTI::Convert::StringToFont( const wxString &value )
{
    wxFontContainer font;

    wxStringTokenizer tokenizer( value, "," );

    // Order: size,family,style,weight,underlined,face and encoding
    //        (int,int,int,int,int,string) as in 3.0 ctor
    if( tokenizer.HasMoreTokens() )
        font.SetPointSize( wxAtoi( tokenizer.GetNextToken() ) );

    if( tokenizer.HasMoreTokens() )
        font.SetFamily( wxAtoi( tokenizer.GetNextToken() ) );

    if( tokenizer.HasMoreTokens() )
        font.SetStyle( wxAtoi( tokenizer.GetNextToken() ) );

    if( tokenizer.HasMoreTokens() )
        font.SetWeight( wxAtoi( tokenizer.GetNextToken() ) );

    if( tokenizer.HasMoreTokens() )
        font.SetUnderlined( wxAtoi( tokenizer.GetNextToken() ) != 0 );

    if( tokenizer.HasMoreTokens() )
    {
        wxString faceName = tokenizer.GetNextToken();
        faceName.Trim( true );
        faceName.Trim( false );
        font.SetFaceName( faceName );
    }

    if( tokenizer.HasMoreTokens() )
    {
        int encoding = wxAtoi( tokenizer.GetNextToken() );
        font.SetEncoding( encoding );
    }

    return font;
}

wxFont RTTI::Convert::StringToSystemFont( const wxString &value )
{
    if( !value.empty() )
    {
        #define SYSFONT(font) \
            if( value == #font ) return wxSystemSettings::GetFont( font );
        SYSFONT( wxSYS_OEM_FIXED_FONT )
        SYSFONT( wxSYS_ANSI_FIXED_FONT )
        SYSFONT( wxSYS_ANSI_VAR_FONT )
        SYSFONT( wxSYS_SYSTEM_FONT )
        SYSFONT( wxSYS_DEVICE_DEFAULT_FONT )
        SYSFONT( wxSYS_DEFAULT_GUI_FONT )
        #undef SYSFONT
    }

    return wxNullFont;
}

wxColour RTTI::Convert::StringToSystemColour( const wxString &value )
{
    if( !value.empty() )
    {
        #define SYSCLR(clr) \
            if( value == #clr ) return wxSystemSettings::GetColour( clr );
        SYSCLR( wxSYS_COLOUR_SCROLLBAR )
        SYSCLR( wxSYS_COLOUR_BACKGROUND )
        SYSCLR( wxSYS_COLOUR_DESKTOP )
        SYSCLR( wxSYS_COLOUR_ACTIVECAPTION )
        SYSCLR( wxSYS_COLOUR_INACTIVECAPTION )
        SYSCLR( wxSYS_COLOUR_MENU )
        SYSCLR( wxSYS_COLOUR_WINDOW )
        SYSCLR( wxSYS_COLOUR_WINDOWFRAME )
        SYSCLR( wxSYS_COLOUR_MENUTEXT )
        SYSCLR( wxSYS_COLOUR_WINDOWTEXT )
        SYSCLR( wxSYS_COLOUR_CAPTIONTEXT )
        SYSCLR( wxSYS_COLOUR_ACTIVEBORDER )
        SYSCLR( wxSYS_COLOUR_INACTIVEBORDER )
        SYSCLR( wxSYS_COLOUR_APPWORKSPACE )
        SYSCLR( wxSYS_COLOUR_HIGHLIGHT )
        SYSCLR( wxSYS_COLOUR_HIGHLIGHTTEXT )
        SYSCLR( wxSYS_COLOUR_BTNFACE )
        SYSCLR( wxSYS_COLOUR_3DFACE )
        SYSCLR( wxSYS_COLOUR_BTNSHADOW )
        SYSCLR( wxSYS_COLOUR_3DSHADOW )
        SYSCLR( wxSYS_COLOUR_GRAYTEXT )
        SYSCLR( wxSYS_COLOUR_BTNTEXT )
        SYSCLR( wxSYS_COLOUR_INACTIVECAPTIONTEXT )
        SYSCLR( wxSYS_COLOUR_BTNHIGHLIGHT )
        SYSCLR( wxSYS_COLOUR_BTNHILIGHT )
        SYSCLR( wxSYS_COLOUR_3DHIGHLIGHT )
        SYSCLR( wxSYS_COLOUR_3DHILIGHT )
        SYSCLR( wxSYS_COLOUR_3DDKSHADOW )
        SYSCLR( wxSYS_COLOUR_3DLIGHT )
        SYSCLR( wxSYS_COLOUR_INFOTEXT )
        SYSCLR( wxSYS_COLOUR_INFOBK )
        SYSCLR( wxSYS_COLOUR_LISTBOX )
        SYSCLR( wxSYS_COLOUR_HOTLIGHT )
        SYSCLR( wxSYS_COLOUR_GRADIENTACTIVECAPTION )
        SYSCLR( wxSYS_COLOUR_GRADIENTINACTIVECAPTION )
        SYSCLR( wxSYS_COLOUR_MENUHILIGHT )
        SYSCLR( wxSYS_COLOUR_MENUBAR )
        #undef SYSCLR
    }

    return wxNullColour;
}

int RTTI::Convert::StringToSystemColourIndex( const wxString &value )
{
    if( !value.empty() )
    {
        #define SYSCLRIDX( clr ) \
            if( value == #clr ) return clr;
        SYSCLRIDX( wxSYS_COLOUR_SCROLLBAR )
        SYSCLRIDX( wxSYS_COLOUR_BACKGROUND )
        SYSCLRIDX( wxSYS_COLOUR_DESKTOP )
        SYSCLRIDX( wxSYS_COLOUR_ACTIVECAPTION )
        SYSCLRIDX( wxSYS_COLOUR_INACTIVECAPTION )
        SYSCLRIDX( wxSYS_COLOUR_MENU )
        SYSCLRIDX( wxSYS_COLOUR_WINDOW )
        SYSCLRIDX( wxSYS_COLOUR_WINDOWFRAME )
        SYSCLRIDX( wxSYS_COLOUR_MENUTEXT )
        SYSCLRIDX( wxSYS_COLOUR_WINDOWTEXT )
        SYSCLRIDX( wxSYS_COLOUR_CAPTIONTEXT )
        SYSCLRIDX( wxSYS_COLOUR_ACTIVEBORDER )
        SYSCLRIDX( wxSYS_COLOUR_INACTIVEBORDER )
        SYSCLRIDX( wxSYS_COLOUR_APPWORKSPACE )
        SYSCLRIDX( wxSYS_COLOUR_HIGHLIGHT )
        SYSCLRIDX( wxSYS_COLOUR_HIGHLIGHTTEXT )
        SYSCLRIDX( wxSYS_COLOUR_BTNFACE )
        SYSCLRIDX( wxSYS_COLOUR_3DFACE )
        SYSCLRIDX( wxSYS_COLOUR_BTNSHADOW )
        SYSCLRIDX( wxSYS_COLOUR_3DSHADOW )
        SYSCLRIDX( wxSYS_COLOUR_GRAYTEXT )
        SYSCLRIDX( wxSYS_COLOUR_BTNTEXT )
        SYSCLRIDX( wxSYS_COLOUR_INACTIVECAPTIONTEXT )
        SYSCLRIDX( wxSYS_COLOUR_BTNHIGHLIGHT )
        SYSCLRIDX( wxSYS_COLOUR_BTNHILIGHT )
        SYSCLRIDX( wxSYS_COLOUR_3DHIGHLIGHT )
        SYSCLRIDX( wxSYS_COLOUR_3DHILIGHT )
        SYSCLRIDX( wxSYS_COLOUR_3DDKSHADOW )
        SYSCLRIDX( wxSYS_COLOUR_3DLIGHT )
        SYSCLRIDX( wxSYS_COLOUR_INFOTEXT )
        SYSCLRIDX( wxSYS_COLOUR_INFOBK )
        SYSCLRIDX( wxSYS_COLOUR_LISTBOX )
        SYSCLRIDX( wxSYS_COLOUR_HOTLIGHT )
        SYSCLRIDX( wxSYS_COLOUR_GRADIENTACTIVECAPTION )
        SYSCLRIDX( wxSYS_COLOUR_GRADIENTINACTIVECAPTION )
        SYSCLRIDX( wxSYS_COLOUR_MENUHILIGHT )
        SYSCLRIDX( wxSYS_COLOUR_MENUBAR )
        #undef SYSCLRIDX
    }

    return 0; // "Default"
}

wxPoint RTTI::Convert::StringToPoint( const wxString &value )
{
    wxArrayString tokens = wxStringTokenize( value, "," );
    if( tokens.GetCount() == 2 )
    {
        wxString sX = tokens.Item(0);
        wxString sY = tokens.Item(1);

        if( sX.IsNumber() && sY.IsNumber() )
        {
            int x = wxAtoi(sX);
            int y = wxAtoi(sY);
            return wxPoint( x,y );
        }
    }

    return wxDefaultPosition;
}

wxSize RTTI::Convert::StringToSize( const wxString &value )
{
    wxArrayString tokens = wxStringTokenize( value, "," );
    if( tokens.GetCount() == 2 )
    {
        wxString sX = tokens.Item(0);
        wxString sY = tokens.Item(1);

        if( sX.IsNumber() && sY.IsNumber() )
        {
            int x = wxAtoi(sX);
            int y = wxAtoi(sY);
            return wxSize( x,y );
        }
    }

    return wxDefaultSize;
}
//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------
wxString RTTI::Convert::IntegerToString( int value )
{
    return wxString::Format( "%i", value );
}
/*
wxString RTTI::Convert::FlagToString( int value )
{
    return RTTI::Flags::Get()->Get( value );
}
*/
wxString RTTI::Convert::BoolToString( bool value )
{
    return( value ? "1" : "0" );
}

wxString RTTI::Convert::FloatToString( double value )
{
    return wxString::Format( "%d", value );
}

wxString RTTI::Convert::SystemColourToString( wxInt32 index )
{
    switch( index )
    {
        #define SYSCLRTOSTR(idx) \
            case idx: \
                return #idx;
        SYSCLRTOSTR( wxSYS_COLOUR_SCROLLBAR )
        SYSCLRTOSTR( wxSYS_COLOUR_BACKGROUND )
//      SYSCLRTOSTR( wxSYS_COLOUR_DESKTOP )
        SYSCLRTOSTR( wxSYS_COLOUR_ACTIVECAPTION )
        SYSCLRTOSTR( wxSYS_COLOUR_INACTIVECAPTION )
        SYSCLRTOSTR( wxSYS_COLOUR_MENU )
        SYSCLRTOSTR( wxSYS_COLOUR_WINDOW )
        SYSCLRTOSTR( wxSYS_COLOUR_WINDOWFRAME )
        SYSCLRTOSTR( wxSYS_COLOUR_MENUTEXT )
        SYSCLRTOSTR( wxSYS_COLOUR_WINDOWTEXT )
        SYSCLRTOSTR( wxSYS_COLOUR_CAPTIONTEXT )
        SYSCLRTOSTR( wxSYS_COLOUR_ACTIVEBORDER )
        SYSCLRTOSTR( wxSYS_COLOUR_INACTIVEBORDER )
        SYSCLRTOSTR( wxSYS_COLOUR_APPWORKSPACE )
        SYSCLRTOSTR( wxSYS_COLOUR_HIGHLIGHT )
        SYSCLRTOSTR( wxSYS_COLOUR_HIGHLIGHTTEXT )
        SYSCLRTOSTR( wxSYS_COLOUR_BTNFACE )
//      SYSCLRTOSTR( wxSYS_COLOUR_3DFACE )
        SYSCLRTOSTR( wxSYS_COLOUR_BTNSHADOW )
//      SYSCLRTOSTR( wxSYS_COLOUR_3DSHADOW )
        SYSCLRTOSTR( wxSYS_COLOUR_GRAYTEXT )
        SYSCLRTOSTR( wxSYS_COLOUR_BTNTEXT )
        SYSCLRTOSTR( wxSYS_COLOUR_INACTIVECAPTIONTEXT )
//      SYSCLRTOSTR( wxSYS_COLOUR_BTNHIGHLIGHT )
//      SYSCLRTOSTR( wxSYS_COLOUR_BTNHILIGHT )
        SYSCLRTOSTR( wxSYS_COLOUR_3DHIGHLIGHT )
//      SYSCLRTOSTR( wxSYS_COLOUR_3DHILIGHT )
        SYSCLRTOSTR( wxSYS_COLOUR_3DDKSHADOW )
        SYSCLRTOSTR( wxSYS_COLOUR_3DLIGHT )
        SYSCLRTOSTR( wxSYS_COLOUR_INFOTEXT )
        SYSCLRTOSTR( wxSYS_COLOUR_INFOBK )
        SYSCLRTOSTR( wxSYS_COLOUR_LISTBOX )
        SYSCLRTOSTR( wxSYS_COLOUR_HOTLIGHT )
        SYSCLRTOSTR( wxSYS_COLOUR_GRADIENTACTIVECAPTION )
        SYSCLRTOSTR( wxSYS_COLOUR_GRADIENTINACTIVECAPTION )
        SYSCLRTOSTR( wxSYS_COLOUR_MENUHILIGHT )
        SYSCLRTOSTR( wxSYS_COLOUR_MENUBAR )
        #undef SYSCLRTOSTR
    }

    return "0"; // "Default"
}

wxString RTTI::Convert::ColourToString( const wxColour &colour, int type )
{
    if( (type != 0xFFFFFF) && (type != 0) ) // !wxPG_COLOUR_CUSTOM || !"Default"
    {
        return SystemColourToString( type );
    }
    else
    {
        return( wxString::Format("%i,%i,%i", colour.Red(), colour.Green(), colour.Blue()) );
    }

    return wxEmptyString;
}

wxString RTTI::Convert::ArrayStringToString( const wxArrayString &value )
{
    wxString result = wxEmptyString;

    for( size_t i = 0; i < value.GetCount(); i++ )
        result += value.Item(i) + "\n";

    return result;
}

wxString RTTI::Convert::FontToString( const wxFontContainer &font )
{
    // Order: size,family,style,weight,underlined,face and encoding
    //        (int,int,int,int,int,string) as in 3.0 ctor
    return wxString::Format   ( wxT("%i,%i,%i,%i,%i,%s,%i"), font.GetPointSize(),
                                font.GetFamily(), font.GetStyle(),
                                font.GetWeight(), font.GetUnderlined() ? 1 : 0,
                                font.GetFaceName(), font.GetEncoding() );
}

wxString RTTI::Convert::PointToString( const wxPoint &point )
{
    return wxString::Format( "%i,%i", point.x, point.y );
}

wxString RTTI::Convert::SizeToString( const wxSize &size )
{
    return wxString::Format( "%i,%i", size.GetWidth(), size.GetHeight() );
}
