///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/flags.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/02/02
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_RTTI_FLAGS_H__
#define __WXGUIDESIGNER_RTTI_FLAGS_H__

#include "wxguidesigner/dllimpexp.h"

#include <wx/arrstr.h>
#include <wx/dynarray.h>

class wxArrayString;

class DLLIMPEXP_WXGUIDESIGNER wxFlagsManager
{
public:
    static wxFlagsManager *Get();
    static void Free();

    void AddFlag( const wxString &name, int value );
    int  GetFlag( const wxString &name );

private:
    wxFlagsManager();
    ~wxFlagsManager();

    wxFlagsManager( const wxFlagsManager & );
    wxFlagsManager& operator=( wxFlagsManager const & );

    void Init();

    wxArrayString m_flagNames;
    wxArrayInt    m_flagValues;

    static wxFlagsManager *ms_instance;
};

#define wxFLAG_ADD( style ) AddFlag( #style, style )

#endif //__WXGUIDESIGNER_RTTI_FLAGS_H__
