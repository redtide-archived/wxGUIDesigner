///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/toplevel/frame.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/05
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_TOPLEVEL_FRAME_H__
#define __WXGUIDESIGNER_GUI_TOPLEVEL_FRAME_H__

#include "wxguidesigner/gui/toplevel/toplevel.h"

class wxBitmap;
class wxBoxSizer;
class wxButton;
class wxCommandEvent;
//class wxPanel;
class wxStaticBitmap;
//class TopLevelWindow;

class Frame : public TopLevelWindow 
{
public:
    Frame() {}
    Frame(  wxWindow        *parent,
            wxWindowID      id,
            const wxString  &title,
            const wxPoint   &pos    = wxDefaultPosition,
            const wxSize    &size   = wxDefaultSize,
            long            style   = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString  &name   = "gd_frame" )
    {
        Create( parent, id, title, pos, size, style, name );
    }
    ~Frame() {}

    bool Create(wxWindow        *parent,
                wxWindowID      id,
                const wxString  &title,
                const wxPoint   &pos    = wxDefaultPosition,
                const wxSize    &size   = wxDefaultSize,
                long            style   = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString  &name   = "gd_frame" );
};

#endif //__WXGUIDESIGNER_GUI_TOPLEVEL_FRAME_H__
