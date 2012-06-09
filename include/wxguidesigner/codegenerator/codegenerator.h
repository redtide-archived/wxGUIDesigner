///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/codegenerator/codegenerator.h
// Purpose:     Code generator library
// Author:      Jefferson González
// Modified by: 
// Created:     2012/05/17
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_CODEGENERATOR_CODEGENERATOR_H__
#define __WXGUIDESIGNER_CODEGENERATOR_CODEGENERATOR_H__

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

#endif //__WXGUIDESIGNER_CODEGENERATOR_CODEGENERATOR_H__
