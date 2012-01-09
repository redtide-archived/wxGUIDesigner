///////////////////////////////////////////////////////////////////////////////
// Name:        core/utils.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/utils.h"

#include <cstring>
#include <ios>
#include <sstream>

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>

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

const wxColour wxGDConv::GetSystemColour( const wxString &name )
{
    if ( !name.empty() )
    {
        #define SYSCLR(clr) \
            if ( name == #clr ) return wxSystemSettings::GetColour( clr );
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
