///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/flags.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/02/02
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_RTTI_FLAGS_H__
#define __WXGUIDESIGNER_RTTI_FLAGS_H__

#include <wx/arrstr.h>
#include <wx/dynarray.h>

#include "wxguidesigner/dllimpexp.h"

class wxString;

namespace wxGD
{
class DLLIMPEXP_WXGUIDESIGNER Flags
{
public:
    static Flags *Get();
    static void Free();

    void        Add( const wxString &name, int value );
    int         Get( const wxString &name );
    wxString    Get( size_t index );

private:
    Flags();
    ~Flags();

    Flags( const Flags & );
    Flags& operator=( Flags const & );

    void Init();

    wxArrayString   m_flagNames;
    wxArrayInt      m_flagValues;
    static Flags    *ms_instance;
};
};

#endif //__WXGUIDESIGNER_RTTI_FLAGS_H__
