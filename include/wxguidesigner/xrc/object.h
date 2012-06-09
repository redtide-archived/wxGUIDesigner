///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/xrc/object.h
// Purpose:     XRCObject
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/14
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_XRC_OBJECT_H__
#define __WXGUIDESIGNER_XRC_OBJECT_H__

#include <wxguidesigner/interfaces/iobject.h>

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

#endif //__WXGUIDESIGNER_XRC_OBJECT_H__
