///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/xrc/types.h
// Purpose:     XRC Object types
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/14
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_XRC_TYPES_H__
#define __WXGUIDESIGNER_XRC_TYPES_H__

class wxXmlNode;
class wxXmlDocument;

typedef shared_ptr< wxXmlNode > XRCObject;
typedef vector< XRCObject >     XRCObjects;

typedef wxXmlDocument           XRCDocument;
typedef vector< wxXmlDocument > XRCDocuments;

#endif //__WXGUIDESIGNER_XRC_TYPES_H__
