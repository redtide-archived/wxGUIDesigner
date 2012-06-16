/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/gd_frame.h
// Purpose:     XML resource handler for Frame (fake toplevel for wxGD)
//              (from the original wx/xrc/xh_frame.h)
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/05
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
/////////////////////////////////////////////////////////////////////////////
#ifndef __WX_XRC_GD_FRAME_H__
#define __WX_XRC_GD_FRAME_H__

#include "wx/xrc/xmlres.h"

class FrameXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(FrameXmlHandler)

public:
    FrameXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif //__WX_XRC_GD_FRAME_H__
