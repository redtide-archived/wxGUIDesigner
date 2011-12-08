///////////////////////////////////////////////////////////////////////////////
// Name:        sdk/defs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/04
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDPLUGIN_DEFS_H__
#define __WXGDPLUGIN_DEFS_H__

#if defined(WXMAKINGLIB)
  #define DLLIMPEXP_PLUGIN
#elif defined(MAKINGDLL_PLUGIN)
  #define DLLIMPEXP_PLUGIN WXEXPORT
#elif defined(WXUSINGDLL)
  #define DLLIMPEXP_PLUGIN WXIMPORT
#else // not making nor using DLL
  #define DLLIMPEXP_PLUGIN
#endif

#include <wx/filefn.h>
#include <wx/stdpaths.h>

#ifdef __WXMSW__
    #define wxGD_PLUGIN_DIR \
    wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') + wxFILE_SEP_PATH + \
                                    "plugins" + wxFILE_SEP_PATH
#else
    #define wxGD_PLUGIN_DIR \
    wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') + wxFILE_SEP_PATH + \
                                    "wxguidesigner/plugins" + wxFILE_SEP_PATH
#endif

#endif //__WXGDPLUGIN_DEFS_H__
