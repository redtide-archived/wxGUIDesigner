///////////////////////////////////////////////////////////////////////////////
// Name:        core/defs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_DEFS_H__
#define __WXGDCORE_DEFS_H__

#if defined(WXMAKINGLIB)
  #define DLLIMPEXP_CORE
#elif defined(WXMAKINGDLL)
  #define DLLIMPEXP_CORE WXEXPORT
#elif defined(WXUSINGDLL)
  #define DLLIMPEXP_CORE WXIMPORT
#else // not making nor using DLL
  #define DLLIMPEXP_CORE
#endif

#include <wx/filefn.h>
#include <wx/stdpaths.h>

#define wxGD_XRC_DIR wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') + \
                        wxFILE_SEP_PATH + "wxguidesigner" + \
                        wxFILE_SEP_PATH + "xrc" + wxFILE_SEP_PATH

#define wxGD_ABOUT              wxGD_XRC_DIR + "gui/about.xrc"
//#define wxGD_DESIGNER           wxGD_XRC_DIR + "gui/designer.xrc"
#define wxGD_EDITOR             wxGD_XRC_DIR + "gui/editor.xrc"
#define wxGD_AUIFRAME           wxGD_XRC_DIR + "gui/mainframe.xrc"
#define wxGD_MAINMENU           wxGD_XRC_DIR + "gui/mainmenu.xrc"
#define wxGD_OBJECT_INSPECTOR   wxGD_XRC_DIR + "gui/objinsp.xrc"
#define wxGD_OBJECT_TREE        wxGD_XRC_DIR + "gui/objtree.xrc"
#define wxGD_OBJECT_PALETTE     wxGD_XRC_DIR + "gui/palette.xrc"
#define wxGD_TOOLBAR            wxGD_XRC_DIR + "gui/toolbar.xrc"
#define wxGD_LOGO               wxGD_XRC_DIR + "gui/images/logo.png"

#define wxGD_PROP_WINDOW        wxGD_XRC_DIR + "classes/window.xrc"
#define wxGD_PROP_TOPLEVEL      wxGD_XRC_DIR + "classes/toplevel.xrc"

#endif //__WXGDCORE_DEFS_H__
