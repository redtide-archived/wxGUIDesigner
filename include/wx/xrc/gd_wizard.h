/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/gd_wizard.h
// Purpose:     XML resource handler for Frame (fake toplevel for wxGD)
//              (from the original wx/xrc/xh_wizrd.h)
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/05
// Revision:    $Hash$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef __WX_XRC_GD_WIZARD_H__
#define __WX_XRC_GD_WIZARD_H__

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_WIZARDDLG

class Wizard;
class WizardPageSimple;

class WizardXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS( WizardXmlHandler )

public:
    WizardXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle( wxXmlNode *node );

private:
    Wizard           *m_wizard;
    WizardPageSimple *m_lastSimplePage;
};

#endif // wxUSE_XRC && wxUSE_WIZARDDLG

#endif //__WX_XRC_GD_WIZARD_H__
