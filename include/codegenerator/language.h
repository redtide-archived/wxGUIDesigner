///////////////////////////////////////////////////////////////////////////////
// Name:        codegenerator/codegenerator.h
// Purpose:     Code generator library
// Author:      Jefferson González
// Modified by: 
// Created:     2012/05/17
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef WXGD_LANGUAGE_H
#define WXGD_LANGUAGE_H

#include <string>

struct Language
{
    std::string Name;
    std::string FileExtension;
    std::string Icon;
};

#endif // WXGD_LANGUAGE_H
