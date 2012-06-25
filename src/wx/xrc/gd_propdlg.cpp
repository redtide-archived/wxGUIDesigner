///////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/gd_propdlg.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/24
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_BOOKCTRL

#include "wx/xrc/gd_propdlg.h"

#ifndef WX_PRECOMP
    #include <wx/log.h>
    #include <wx/sizer.h>
    #include <wx/frame.h>
#endif

#include <wx/bookctrl.h>
#include <wx/panel.h>
#include <wx/propdlg.h>
#include <wx/imaglist.h>

#include "wxguidesigner/gui/toplevel/toplevel.h"
#include "wxguidesigner/gui/toplevel/dialog.h"
#include "wxguidesigner/gui/toplevel/propdlg.h"

IMPLEMENT_DYNAMIC_CLASS(PropertySheetDialogXmlHandler, wxXmlResourceHandler)

PropertySheetDialogXmlHandler::PropertySheetDialogXmlHandler()
                     :wxXmlResourceHandler(),
                      m_isInside(false),
                      m_dialog(NULL)
{
    XRC_ADD_STYLE(wxSTAY_ON_TOP);
    XRC_ADD_STYLE(wxCAPTION);
    XRC_ADD_STYLE(wxDEFAULT_DIALOG_STYLE);
    XRC_ADD_STYLE(wxSYSTEM_MENU);
    XRC_ADD_STYLE(wxRESIZE_BORDER);
    XRC_ADD_STYLE(wxCLOSE_BOX);
    XRC_ADD_STYLE(wxDIALOG_NO_PARENT);

    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
    XRC_ADD_STYLE(wxDIALOG_EX_METAL);
    XRC_ADD_STYLE(wxMAXIMIZE_BOX);
    XRC_ADD_STYLE(wxMINIMIZE_BOX);
    XRC_ADD_STYLE(wxFRAME_SHAPED);
    XRC_ADD_STYLE(wxDIALOG_EX_CONTEXTHELP);

    AddWindowStyles();
}

wxObject *PropertySheetDialogXmlHandler::DoCreateResource()
{
    if (m_class == wxT("propertysheetpage"))
    {
        wxXmlNode *n = GetParamNode(wxT("object"));

        if (!n) n = GetParamNode(wxT("object_ref"));

        if (n)
        {
            wxBookCtrlBase *bookctrl = m_dialog->GetBookCtrl();
            bool old_ins = m_isInside;
            m_isInside = false;
            wxObject *item = CreateResFromNode(n, bookctrl, NULL);
            m_isInside = old_ins;
            wxWindow *wnd = wxDynamicCast(item, wxWindow);

            if (wnd)
            {
                bookctrl->AddPage(wnd, GetText(wxT("label")), GetBool(wxT("selected")));
                if (HasParam(wxT("bitmap")))
                {
                    wxBitmap bmp = GetBitmap(wxT("bitmap"), wxART_OTHER);
                    wxImageList *imgList = bookctrl->GetImageList();
                    if (imgList == NULL)
                    {
                        imgList = new wxImageList(bmp.GetWidth(), bmp.GetHeight());
                        bookctrl->AssignImageList(imgList);
                    }
                    int imgIndex = imgList->Add(bmp);
                    bookctrl->SetPageImage(bookctrl->GetPageCount()-1, imgIndex);
                }
            }
            else
            {
                ReportError(n, "propertysheetpage child must be a window");
            }
            return wnd;
        }
        else
        {
            ReportError("propertysheetpage must have a window child");
            return NULL;
        }
    }
    else
    {
        XRC_MAKE_INSTANCE(dlg, PropertySheetDialog)

        dlg->Create(m_parentAsWindow,
                   GetID(),
                   GetText(wxT("title")),
                   GetPosition(),
                   GetSize(),
                   GetStyle(),
                   GetName());

        if (HasParam(wxT("icon")))
            dlg->SetIcons(GetIconBundle(wxT("icon"), wxART_FRAME_ICON));

        SetupWindow(dlg);

        PropertySheetDialog *old_par = m_dialog;
        m_dialog = dlg;
        bool old_ins = m_isInside;
        m_isInside = true;
        CreateChildren(m_dialog, true/*only this handler*/);
        m_isInside = old_ins;
        m_dialog = old_par;

        if (GetBool(wxT("centered"), false)) dlg->Centre();
        wxString buttons = GetText(wxT("buttons"));
        if (!buttons.IsEmpty())
        {
            int flags = 0;
            if (buttons.Find(wxT("wxOK"))         != wxNOT_FOUND) flags |= wxOK;
            if (buttons.Find(wxT("wxCANCEL"))     != wxNOT_FOUND) flags |= wxCANCEL;
            if (buttons.Find(wxT("wxYES"))        != wxNOT_FOUND) flags |= wxYES;
            if (buttons.Find(wxT("wxNO"))         != wxNOT_FOUND) flags |= wxNO;
            if (buttons.Find(wxT("wxHELP"))       != wxNOT_FOUND) flags |= wxHELP;
            if (buttons.Find(wxT("wxNO_DEFAULT")) != wxNOT_FOUND) flags |= wxNO_DEFAULT;
            dlg->CreateButtons(flags);
        }

        return dlg;
    }
}

bool PropertySheetDialogXmlHandler::CanHandle(wxXmlNode *node)
{
    return ((!m_isInside && IsOfClass(node, wxT("wxPropertySheetDialog"))) ||
            (m_isInside && IsOfClass(node, wxT("propertysheetpage"))));
}

#endif // wxUSE_XRC && wxUSE_BOOKCTRL
