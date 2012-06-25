///////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/gd_dlg.h
// Purpose:     XML resource handler for Dialog (fake toplevel for wxGD)
//              (from the original wx/xrc/xh_dialog.h)
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/25
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WX_XRC_GD_DIALOG_H__
#define __WX_XRC_GD_DIALOG_H__

#include <wx/xrc/xmlres.h>

class DialogXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS( DialogXmlHandler )

public:
    DialogXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle( wxXmlNode *node );
};

#endif //__WX_XRC_GD_DIALOG_H__
