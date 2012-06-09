///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/codegenerator/language.h
// Purpose:     Code generator library
// Author:      Jefferson González
// Modified by: 
// Created:     2012/05/17
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_CODEGENERATOR_LANGUAGE_H__
#define __WXGUIDESIGNER_CODEGENERATOR_LANGUAGE_H__

#include <string>

struct Language
{
    std::string Name;
    std::string FileExtension;
    std::string Icon;
};

#endif //__WXGUIDESIGNER_CODEGENERATOR_LANGUAGE_H__
