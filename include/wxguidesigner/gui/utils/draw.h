///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/utils/draw.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/06/21
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_UTILS_H__
#define __WXGUIDESIGNER_GUI_UTILS_H__

class wxBitmap;
class wxColour;
class wxDC;
class wxRect;
class wxString;

namespace wxGD
{
namespace Draw
{
    const bool IsDark( const wxColour &colour );

    void GlossyGradient( wxDC &dc, wxRect &rect, wxColour &topStart,
                         wxColour &bottomStart,  wxColour &bottomEnd,
                         wxColour &colour, bool hover = false );

    void Bitmap( wxDC &dc, const wxBitmap &bitmap, const wxRect &rect,
                const wxString &text );
};
};

#endif //__WXGUIDESIGNER_UTILS_H__
