///////////////////////////////////////////////////////////////////////////////
// Name:        core/xrc/object.h
// Purpose:     XRCObject
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/14
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_XRC_MANAGER_H__
#define __CORE_XRC_MANAGER_H__

#include <interfaces/iobject.h>

#include <wx/xml/xml.h>

static const wxString wxXRCNameSpaceURI = "http://www.wxwidgets.org/wxxrc";

class XRCObject : public wxXmlNode
{
public:
    XRCObject( XRCObject *parent, IObject *object, bool isRef = false );
    virtual ~XRCObject();

private:
    IObject *m_object;
};

#endif //__CORE_XRC_MANAGER_H__
