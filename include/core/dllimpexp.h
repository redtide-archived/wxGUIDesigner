/*
    Name:        core/dllimpexp.h
    Purpose:     
    Author:      Andrea Zanellato
    Modified by: 
    Created:     2011/11/20
    Revision:    $Hash$
    Licence:     wxWindows licence
*/

#ifndef __WXGDCORE_DLLIMPEXP_H__
#define __WXGDCORE_DLLIMPEXP_H__

#include <wx/dlimpexp.h>

#if defined(WXMAKINGLIB)
  #define DLLIMPEXP_CORE
#elif defined(MAKINGDLL_CORE)
  #define DLLIMPEXP_CORE WXEXPORT
#elif defined(WXUSINGDLL)
  #define DLLIMPEXP_CORE WXIMPORT
#else /* not making nor using DLL */
  #define DLLIMPEXP_CORE
#endif

#endif /*__WXGDCORE_DLLIMPEXP_H__*/
