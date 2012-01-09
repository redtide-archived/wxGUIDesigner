/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_stc.h
// Purpose:     XRC resource handler for wxStyledTextCtrl
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011-09-18
// Revision:    $Hash$
// Copyright:   (c) 2011 wxWidgets Team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_STC_H_
#define _WX_XH_STC_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_STC

class WXDLLIMPEXP_XRC wxStyledTextCtrlXmlHandler : public wxXmlResourceHandler
{
public:
    wxStyledTextCtrlXmlHandler();

    virtual wxObject *DoCreateResource();
    virtual bool CanHandle( wxXmlNode *node );

    wxDECLARE_DYNAMIC_CLASS( wxStyledTextCtrlXmlHandler );
};

#endif //wxUSE_XRC && wxUSE_STC

#endif //_WX_XH_STC_H_
