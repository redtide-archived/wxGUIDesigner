/*
    Name:        codegenerator/dllimpexp.h
    Purpose:     
    Author:      Andrea Zanellato
    Modified by: 
    Created:     2012/05/18
    Revision:    $Hash$
    Licence:     wxWindows licence
*/

#ifndef __CODEGENERATOR_DLLIMPEXP_H__
#define __CODEGENERATOR_DLLIMPEXP_H__

#include <wx/dlimpexp.h>

#if defined(WXMAKINGLIB)
  #define DLLIMPEXP_CODEGEN
#elif defined(MAKINGDLL_CODEGEN)
  #define DLLIMPEXP_CODEGEN WXEXPORT
#elif defined(WXUSINGDLL)
  #define DLLIMPEXP_CODEGEN WXIMPORT
#else /* not making nor using DLL */
  #define DLLIMPEXP_CODEGEN
#endif

#endif /*__CODEGENERATOR_DLLIMPEXP_H__*/
