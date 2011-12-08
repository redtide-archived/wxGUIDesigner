///////////////////////////////////////////////////////////////////////////////
// Name:        core/defs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_DEFS_H__
#define __WXGDCORE_DEFS_H__

#if defined(WXMAKINGLIB)
  #define DLLIMPEXP_CORE
#elif defined(MAKINGDLL_CORE)
  #define DLLIMPEXP_CORE WXEXPORT
#elif defined(WXUSINGDLL)
  #define DLLIMPEXP_CORE WXIMPORT
#else // not making nor using DLL
  #define DLLIMPEXP_CORE
#endif

#include <wx/filefn.h>
#include <wx/stdpaths.h>

#define wxGD_XRC_DIR wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') + \
                        wxFILE_SEP_PATH + "wxguidesigner" + wxFILE_SEP_PATH

#define wxGD_XRS                wxGD_XRC_DIR + "wxguidesigner.xrs"
#define wxGD_ABOUT              wxGD_XRS + "#zip:about.xrc"
#define wxGD_DESIGNER           wxGD_XRS + "#zip:designer.xrc"
#define wxGD_EDITOR             wxGD_XRS + "#zip:editor.xrc"
#define wxGD_AUIFRAME           wxGD_XRS + "#zip:mainframe.xrc"
#define wxGD_MAINMENU           wxGD_XRS + "#zip:mainmenu.xrc"
#define wxGD_OBJECT_INSPECTOR   wxGD_XRS + "#zip:objinsp.xrc"
#define wxGD_OBJECT_TREE        wxGD_XRS + "#zip:objtree.xrc"
#define wxGD_OBJECT_PALETTE     wxGD_XRS + "#zip:palette.xrc"
#define wxGD_TOOLBAR            wxGD_XRS + "#zip:toolbar.xrc"
#define wxGD_IMAGES             wxGD_XRS + "#zip:images.xrc"

#endif //__WXGDCORE_DEFS_H__
