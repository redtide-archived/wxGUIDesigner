///////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/gd_wizard.cpp
// Purpose:     XML resource handler for Frame (fake toplevel for wxGD)
//              (from the original wx/xrc/xh_wizrd.cpp)
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/05
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_WIZARDDLG

#include "wx/xrc/gd_wizard.h"

#ifndef WX_PRECOMP
    #include <wx/log.h>
#endif

#include <wx/wizard.h>

#include "wxguidesigner/gui/toplevel/toplevel.h"
#include "wxguidesigner/gui/toplevel/dialog.h"
#include "wxguidesigner/gui/toplevel/wizard.h"

IMPLEMENT_DYNAMIC_CLASS(WizardXmlHandler, wxXmlResourceHandler)

WizardXmlHandler::WizardXmlHandler() : wxXmlResourceHandler()
{
    m_wizard = NULL;
    m_lastSimplePage = NULL;
    XRC_ADD_STYLE(wxWIZARD_EX_HELPBUTTON);
    AddWindowStyles();
}

wxObject *WizardXmlHandler::DoCreateResource()
{
    if (m_class == wxT("wxWizard"))
    {
        XRC_MAKE_INSTANCE(wiz, Wizard)

        long style = GetStyle(wxT("exstyle"), 0);
        if (style != 0)
            wiz->SetExtraStyle(style);
        wiz->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("title")),
                    GetBitmap(),
                    GetPosition());
        SetupWindow(wiz);

        Wizard *old = m_wizard;
        m_wizard = wiz;
        m_lastSimplePage = NULL;
        CreateChildren(wiz, true /*this handler only*/);
        m_wizard = old;
        return wiz;
    }
    else
    {
        WizardPage *page;

        if (m_class == wxT("wxWizardPageSimple"))
        {
            XRC_MAKE_INSTANCE(p, WizardPageSimple)
            p->Create(m_wizard, NULL, NULL, GetBitmap());
            if (m_lastSimplePage)
                WizardPageSimple::Chain(m_lastSimplePage, p);
            page = p;
            m_lastSimplePage = p;
        }
        else // if (m_class == wxT("wxWizardPage"))
        {
            if ( !m_instance )
            {
                ReportError("wxWizardPage is abstract class and must be subclassed");
                return NULL;
            }

            page = wxStaticCast( m_instance, WizardPage );
            page->Create( m_wizard, GetBitmap() );
        }

        page->SetName(GetName());
        page->SetId(GetID());

        SetupWindow(page);
        CreateChildren(page);

        m_wizard->AddPage(page);

        return page;
    }
}

bool WizardXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass( node, wxT("wxWizard") )                   ||
                    ( m_wizard != NULL                          &&
                    ( IsOfClass( node, wxT("wxWizardPage") )    ||
                      IsOfClass( node, wxT("wxWizardPageSimple") ) ) );
}

#endif // wxUSE_XRC && wxUSE_WIZARDDLG
