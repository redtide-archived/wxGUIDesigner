///////////////////////////////////////////////////////////////////////////////
// Name:        codegenerator.h
// Purpose:     Code generator library
// Author:      Jefferson González
// Modified by: 
// Created:     2012/05/17
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CODEGENERATOR_H__
#define __CODEGENERATOR_H__

#include "cgdllimpexp.h"

#include <vector>
#include "language.h"

class DLLIMPEXP_CODEGEN CodeGenerator
{
public:
    static CodeGenerator *Get();
    static void Free();

    void AddLanguage();
    void ScanLanguages();

private:
    CodeGenerator();
    ~CodeGenerator();

    static CodeGenerator    *ms_instance;

    std::vector< Language > m_languages;
};

#endif //__CODEGENERATOR_H__
