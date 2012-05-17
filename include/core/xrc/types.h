///////////////////////////////////////////////////////////////////////////////
// Name:        core/xrc/types.h
// Purpose:     XRC Object types
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/14
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __CORE_XRC_TYPES_H__
#define __CORE_XRC_TYPES_H__

class wxXmlNode;
class wxXmlDocument;

typedef shared_ptr< wxXmlNode > XRCObject;
typedef vector< XRCObject >     XRCObjects;

typedef wxXmlDocument           XRCDocument;
typedef vector< wxXmlDocument > XRCDocuments;

#endif //__CORE_XRC_TYPES_H__
