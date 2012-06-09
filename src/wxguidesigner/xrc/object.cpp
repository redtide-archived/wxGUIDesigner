///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/xrc/manager.cpp
// Purpose:     wxXRCManager implementation
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/14
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/xrc/object.h"

XRCObject::XRCObject( XRCObject *parent, IObject *object, bool isRef )
         : wxXmlNode( (wxXmlNode *)parent, wxXML_ELEMENT_NODE,
                        isRef ? "object_ref" : "object" ),
m_object( object )
{
    
}

XRCObject::~XRCObject()
{
}

