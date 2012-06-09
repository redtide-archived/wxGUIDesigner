/*
    Name:        wxguidesigner/dllimpexp.h
    Purpose:     
    Author:      Andrea Zanellato
    Modified by: 
    Created:     2011/11/20
    Revision:    $Hash$
    Copyright:   (c) Andrea Zanellato
    Licence:     wxWindows licence
*/
#ifndef __WXGUIDESIGNER_DLLIMPEXP_H__
#define __WXGUIDESIGNER_DLLIMPEXP_H__

#include <wx/dlimpexp.h>

#if defined(WXMAKINGLIB)
  #define DLLIMPEXP_WXGUIDESIGNER
#elif defined(MAKINGDLL_WXGUIDESIGNER)
  #define DLLIMPEXP_WXGUIDESIGNER WXEXPORT
#elif defined(WXUSINGDLL)
  #define DLLIMPEXP_WXGUIDESIGNER WXIMPORT
#else /* not making nor using DLL */
  #define DLLIMPEXP_WXGUIDESIGNER
#endif

#endif /*__WXGUIDESIGNER_DLLIMPEXP_H__*/
