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
#include "wxguidesigner/fontcontainer.h"
#include "wxguidesigner/utils.h"
#include "wxguidesigner/rtti.h"
#include "wxguidesigner/rtti/flags.h"

#include <cstring>
#include <ios>
#include <sstream>

#include <wx/colour.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/log.h>
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
/*
int wxGD::Convert::StringToHex( const wxString &text )
{
    std::stringstream s(text.ToStdString());
    int ret;
    s >> std::hex >> ret;
    return ret;
}
*/
int wxGD::Convert::StringToFlag( const wxString &value )
{
    int result = 0;

    wxStringTokenizer tokenizer( value, "|" );
    while( tokenizer.HasMoreTokens() )
    {
        wxString token = tokenizer.GetNextToken();
        token.Trim( true );
        token.Trim( false );

        result |= wxFlagsManager::Get()->GetFlag( token );
    }

    return result;
}

int wxGD::Convert::StringToInteger( const wxString &value )
{
    if( !value.empty() && value.IsNumber() )
        return wxAtoi( value );

    return 0;
}

bool wxGD::Convert::StringToBool( const wxString &value )
{
    if( value.Lower() == "true" || (StringToInteger( value ) > 0) )
        return true;

    return false;
}

double wxGD::Convert::StringToFloat( const wxString &value )
{
    double result;
    value.ToDouble( &result );
    return result;
}

wxArrayString wxGD::Convert::StringToArrayString( const wxString &value )
{
    return wxStringTokenize( value, "\n" );
}

wxFontContainer wxGD::Convert::StringToFont( const wxString &value )
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

wxFont wxGD::Convert::StringToSystemFont( const wxString &value )
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

wxColour wxGD::Convert::StringToColour( const wxString &value )
{
    /* System colour
    if( value.StartsWith("wxSYS_COLOUR_") )
        return StringToSystemColour( value );
*/
    // Custom colour: "r,g,b"
    wxStringTokenizer tokenizer( value, "," );
    bool ok; unsigned long r = 0, g = 0, b = 0;

    ok = tokenizer.GetNextToken().ToULong( &r );
    ok = ok && tokenizer.GetNextToken().ToULong( &g );
    ok = ok && tokenizer.GetNextToken().ToULong( &b );

    if( ok )
        return wxColour( r, g, b );

    return wxNullColour;
}

wxColour wxGD::Convert::StringToSystemColour( const wxString &value )
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

int wxGD::Convert::StringToSystemColourIndex( const wxString &value )
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

wxPoint wxGD::Convert::StringToPoint( const wxString &value )
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

wxSize wxGD::Convert::StringToSize( const wxString &value )
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
wxString wxGD::Convert::IntegerToString( int value )
{
    return wxString::Format( "%i", value );
}
/*
wxString wxGD::Convert::FlagToString( int value )
{
    return wxFlagsManager::Get()->GetFlag( value );
}
*/
wxString wxGD::Convert::BoolToString( bool value )
{
    return( value ? "1" : "0" );
}

wxString wxGD::Convert::FloatToString( double value )
{
    return wxString::Format( "%d", value );
}

wxString wxGD::Convert::SystemColourToString( wxInt32 index )
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

wxString wxGD::Convert::ColourToString( const wxColour &colour, int type )
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

wxString wxGD::Convert::ArrayStringToString( const wxArrayString &value )
{
    wxString result = wxEmptyString;

    for( size_t i = 0; i < value.GetCount(); i++ )
        result += value.Item(i) + "\n";

    return result;
}

wxString wxGD::Convert::FontToString( const wxFontContainer &font )
{
    // Order: size,family,style,weight,underlined,face and encoding
    //        (int,int,int,int,int,string) as in 3.0 ctor
    return wxString::Format   ( wxT("%i,%i,%i,%i,%i,%s,%i"), font.GetPointSize(),
                                font.GetFamily(), font.GetStyle(),
                                font.GetWeight(), font.GetUnderlined() ? 1 : 0,
                                font.GetFaceName(), font.GetEncoding() );
}

wxString wxGD::Convert::PointToString( const wxPoint &point )
{
    return wxString::Format( "%i,%i", point.x, point.y );
}

wxString wxGD::Convert::SizeToString( const wxSize &size )
{
    return wxString::Format( "%i,%i", size.GetWidth(), size.GetHeight() );
}
