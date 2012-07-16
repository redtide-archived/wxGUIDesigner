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

#include <cstring>
#include <ios>
#include <sstream>

#include <wx/colour.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/log.h>

wxColour wxGDConv::GetSystemColour( const wxString &name )
{
    if( !name.empty() )
    {
        #define SYSCLR(clr) \
            if( name == #clr ) return wxSystemSettings::GetColour( clr );
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

wxInt32 wxGDConv::GetSystemColourIndex( const wxString &name )
{
    if( !name.empty() )
    {
        #define SYSCLRIDX( clr ) \
            if( name == #clr ) return clr;
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

    return 0;
}

wxFont wxGDConv::GetSystemFont( const wxString &name )
{
    if( !name.empty() )
    {
        #define SYSFONT(font) \
            if( name == #font ) return wxSystemSettings::GetFont( font );
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
/*
Colour wxGDConv::StringToColourInfo( const wxString &value )
{
    // System colour
    if( value.StartsWith("wxSYS_COLOUR_") )
    {
        wxInt32 colType = GetSystemColourIndex( value );
        Colour col = { colType, wxColour() };
        return col;
    }

    // Custom colour
    wxStringTokenizer tokenizer( value, "," );
    bool ok; unsigned long r = 0, g = 0, b = 0;

    ok = tokenizer.GetNextToken().ToULong( &r );
    ok = ok && tokenizer.GetNextToken().ToULong( &g );
    ok = ok && tokenizer.GetNextToken().ToULong( &b );

    wxColour colour = wxNullColour;

    if( ok )
        colour = wxColour( r, g, b );

    Colour col = { 0xFFFFFF, colour };
    return col;
}
*/

int wxGDConv::StringToHex( const wxString &text )
{
    std::stringstream s(text.ToStdString());
    int ret;
    s >> std::hex >> ret;
    return ret;
}

int wxGDConv::StringToInt( const wxString &value )
{
    if( !value.empty() )
        return wxAtoi( value );

    return 0;
}

wxFontContainer wxGDConv::StringToFont( const wxString &value )
{
    wxFontContainer font; // = wxSystemSettings::GetFont( wxSYS_SYSTEM_FONT );

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

wxString wxGDConv::AnyToString( const wxAny &any )
{
    if( any.CheckType< wxString >() )
    {
        return any.As< wxString >();
    }
    else if( any.CheckType< wxArrayString >() )
    {
        return wxEmptyString; // TODO?
    }
    else if( any.CheckType< wxBitmap >() )
    {
        return wxEmptyString; // TODO?
    }
    else if( any.CheckType< bool >() )
    {
        return BoolToString( any.As< bool >() );
    }
    else if( any.CheckType< Colour >() )
    {
        Colour colour = any.As< Colour >();
        return ColourToString( colour.colour, colour.type );
    }
    else if( any.CheckType< double >() )
    {
        return FloatToString( any.As< double >() );
    }
    else if( any.CheckType< wxFont >() )
    {
        return FontToString( any.As< wxFont >() );
    }
    else if( any.CheckType< int >() )
    {
        return IntToString( any.As< int >() );
    }
    else if( any.CheckType< wxPoint >() )
    {
        return PointToString( any.As< wxPoint >() );
    }
    else if( any.CheckType< wxSize >() )
    {
        return SizeToString( any.As< wxSize >() );
    }

    return wxEmptyString;
}

wxString wxGDConv::BoolToString( bool value )
{
    return( value ? "1" : "0" );
}

wxString wxGDConv::SystemColourToString( wxInt32 index )
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

    return wxEmptyString;
}

wxString wxGDConv::ColourToString( const wxColour &colour, wxInt32 type )
{
    if( type != 0xFFFFFF ) // wxPG_COLOUR_CUSTOM
    {
        return SystemColourToString( type );
    }
    else
    {
        return( wxString::Format( "rgb(%i,%i,%i)", colour.Red(), colour.Green(), colour.Blue() ) );
    }

    return wxEmptyString;
}

wxString wxGDConv::FloatToString( double value )
{
    return wxString::Format( "%d", value );
}

wxString wxGDConv::FontToString( const wxFontContainer &font )
{
    // Order: size,family,style,weight,underlined,face and encoding
    //        (int,int,int,int,int,string) as in 3.0 ctor
    return wxString::Format   ( wxT("%i,%i,%i,%i,%i,%s,%i"), font.GetPointSize(),
                                font.GetFamily(), font.GetStyle(),
                                font.GetWeight(), font.GetUnderlined() ? 1 : 0,
                                font.GetFaceName(), font.GetEncoding() );
}

wxString wxGDConv::IntToString( int value )
{
    return wxString::Format( "%i", value );
}

wxString wxGDConv::PointToString( const wxPoint &point )
{
    return wxString::Format( "%i,%i", point.x, point.y );
}

wxString wxGDConv::SizeToString( const wxSize &size )
{
    return wxString::Format( "%i,%i", size.GetWidth(), size.GetHeight() );
}
