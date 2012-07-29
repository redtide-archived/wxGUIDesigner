///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/utils/draw.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/21
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/colour.h>

#include "wxguidesigner/gui/utils/draw.h"

const bool wxGD::Draw::IsDark( const wxColour &colour )
{
    int average = ( colour.Red() + colour.Green() + colour.Blue() ) / 3;
    if ( average < 128 )
        return true;

    return false;
}
