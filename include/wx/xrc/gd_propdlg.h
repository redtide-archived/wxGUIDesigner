///////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/gd_propdlg.h
// Purpose:     XML resource handler for PropertySheetDialog
//              (from the original wx/xrc/xh_propdlg.h)
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/24
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WX_XRC_GD_PROPDLG_H__
#define __WX_XRC_GD_PROPDLG_H__

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC

class PropertySheetDialog;

class PropertySheetDialogXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(PropertySheetDialogXmlHandler)

public:
    PropertySheetDialogXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    PropertySheetDialog *m_dialog;
};

#endif // wxUSE_XRC

#endif //__WX_XRC_GD_PROPDLG_H__
