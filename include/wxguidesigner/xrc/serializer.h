///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/xrc/serializer.h
// Purpose:     XRC File Parser
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/11
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_XRC_SERIALIZER_H__
#define __WXGUIDESIGNER_XRC_SERIALIZER_H__

namespace wxGD
{
namespace XRCSerializer
{
    bool Load( RTTI::Tree tree, const wxString &path );
    bool Save( RTTI::Tree tree, const wxString &path, int indent = 4 );

    wxXmlNode *Serialize  ( RTTI::Tree tree );

    void SerializeObject  ( RTTI::Object object,        wxXmlNode *objectNode );
    void SerializeProperty( RTTI::Property property,    wxXmlNode *objectNode );
    void SerializeEvent   ( RTTI::EventProperty event,  wxXmlNode *objectNode );

    RTTI::Object CreateObject ( RTTI::Tree tree, RTTI::Object parent,
                                wxXmlNode *parentNode, bool isReference = false );
};
};

#endif //__WXGUIDESIGNER_XRC_SERIALIZER_H__
