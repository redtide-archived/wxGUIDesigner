///////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/gd_dialog.cpp
// Purpose:     XML resource handler for Dialog (fake toplevel for wxGD)
//              (from the original wx/xrc/xh_dlgcpp)
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/25
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xrc/gd_dialog.h"

#ifndef WX_PRECOMP
    #include <wx/dialog.h>
    #include <wx/frame.h>
    #include <wx/intl.h>
    #include <wx/log.h>
    #include <wx/panel.h>
#endif

#include "wxguidesigner/gui/toplevel/toplevel.h"
#include "wxguidesigner/gui/toplevel/dialog.h"

IMPLEMENT_DYNAMIC_CLASS(DialogXmlHandler, wxXmlResourceHandler)

DialogXmlHandler::DialogXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxSTAY_ON_TOP);
    XRC_ADD_STYLE(wxCAPTION);
    XRC_ADD_STYLE(wxDEFAULT_DIALOG_STYLE);
    XRC_ADD_STYLE(wxSYSTEM_MENU);
    XRC_ADD_STYLE(wxRESIZE_BORDER);
    XRC_ADD_STYLE(wxCLOSE_BOX);
    XRC_ADD_STYLE(wxDIALOG_NO_PARENT);
    XRC_ADD_STYLE(wxMINIMIZE); // == wxICONIZE
    XRC_ADD_STYLE(wxMAXIMIZE);

    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
    XRC_ADD_STYLE(wxDIALOG_EX_METAL);
    XRC_ADD_STYLE(wxMAXIMIZE_BOX);
    XRC_ADD_STYLE(wxMINIMIZE_BOX);
    XRC_ADD_STYLE(wxFRAME_SHAPED);
    XRC_ADD_STYLE(wxDIALOG_EX_CONTEXTHELP);

#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxDIALOG_MODAL);
    XRC_ADD_STYLE(wxTHICK_FRAME);
    XRC_ADD_STYLE(wxRESIZE_BOX);
    XRC_ADD_STYLE(wxDIALOG_MODELESS);
    XRC_ADD_STYLE(wxNO_3D);
#endif // WXWIN_COMPATIBILITY_2_6

    AddWindowStyles();
}

wxObject *DialogXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(dlg, Dialog);

    dlg->Create(m_parentAsWindow,
                GetID(),
                GetText(wxT("title")),
                wxDefaultPosition, wxDefaultSize,
                GetStyle(wxT("style"), wxDEFAULT_DIALOG_STYLE),
                GetName());

    if (HasParam(wxT("size")))
        dlg->SetClientSize(GetSize(wxT("size"), dlg));
    if (HasParam(wxT("pos")))
        dlg->Move(GetPosition());
    if (HasParam(wxT("icon")))
        dlg->SetIcons(GetIconBundle(wxT("icon"), wxART_FRAME_ICON));

    SetupWindow(dlg);

    CreateChildren(dlg);

    if (GetBool(wxT("centered"), false))
        dlg->Centre();

    return dlg;
}

bool DialogXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxDialog"));
}
