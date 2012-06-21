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
#include "wxguidesigner/utils.h"

#include <cstring>
#include <ios>
#include <sstream>

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/tokenzr.h>

const int wxGDConv::StringToHex( const wxString &text )
{
    std::stringstream s(text.ToStdString());
    int ret;
    s >> std::hex >> ret;
    return ret;
}

const wxString wxGDConv::HexToString( int value )
{
    return wxString::Format( "%s", value );
}

const int wxGDConv::IntFromString( const wxString &value )
{
    if( !value.empty() )
        return wxAtoi( value );

    return 0;
}

const wxColour wxGDConv::GetSystemColour( const wxString &name )
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

const wxInt32 wxGDConv::GetSystemColourIndex( const wxString &name )
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

const wxFont wxGDConv::GetSystemFont( const wxString &name )
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

const Colour wxGDConv::StringToColourInfo( const wxString &value )
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

    Colour col = { ColourCustom, colour };
    return col;
}
