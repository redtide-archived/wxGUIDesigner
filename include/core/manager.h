///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/manager.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/13
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_MANAGER_H__
#define __CORE_MANAGER_H__

#include "core/dllimpexp.h"

class wxFrame;
class wxWindow;

class DLLIMPEXP_CORE wxGUIDesigner
{
public:
    static wxGUIDesigner *Get();
    static void Free();

    wxFrame *GetMainFrame( wxWindow *parent );
    void NewProject();

private:
    wxGUIDesigner();
    ~wxGUIDesigner();

    static wxGUIDesigner *ms_instance;
};

#endif //__CORE_MANAGER_H__
