///////////////////////////////////////////////////////////////////////////////
// Name:        core/utils.h
// Purpose:     interface of Object
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    Alias to wxWindow::FindWindowById()
*/
inline wxWindow *wxFindWindowById( long winid, const wxWindow *parent = NULL );

/**
    @namespace wxGDConv

    Support utilities

    @library{core}
*/
namespace wxGDConv
{
/**
    Convert the given string (like "0x04000000") to int
*/
    const int StringToHex( const wxString &text );

//  const wxString  HexToString( int value );
/**
    Gets a system colour from the given string (like "wxSYS_COLOUR_BTNFACE")
*/
    const wxColour GetSystemColour( const wxString &name );
};
