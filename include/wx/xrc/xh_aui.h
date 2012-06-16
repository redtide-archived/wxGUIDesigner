/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_aui.h
// Purpose:     XRC resource handler for wxAUI
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011-09-18
// Revision:    $Hash$
// Copyleft:    (ɔ) 2011 wxWidgets Team
// Licence:     GNU General Public License Version 3
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_AUI_H_
#define _WX_XH_AUI_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_AUI

class  WXDLLIMPEXP_FWD_AUI wxAuiManager;
#include "wx/dynarray.h"

WX_DEFINE_USER_EXPORTED_ARRAY_PTR(wxAuiManager *, wxAuiManagerPtrArray,
                                                class WXDLLIMPEXP_AUI);

class WXDLLIMPEXP_AUI wxAuiXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxAuiXmlHandler);

public:
    wxAuiXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

    // Returns the wxAuiManager for the specified window
    wxAuiManager *GetAuiManager(wxWindow *managed);

private:
    // Used to UnInit() the wxAuiManager before destroying its managed window
    void OnManagedWindowClose(wxWindowDestroyEvent &event);

    wxAuiManagerPtrArray m_managers; // wxAuiManagers created in this handler
    wxAuiManager *m_manager; // Current wxAuiManager
    wxWindow *m_window; // Current managed wxWindow
    bool m_isInside; // Are we handling a wxAuiManager or panes inside it?
};

#endif //wxUSE_XRC && wxUSE_AUI

#endif //_WX_XH_AUI_H_

