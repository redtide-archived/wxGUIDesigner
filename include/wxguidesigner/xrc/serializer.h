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

namespace wxXRCSerializer
{
    bool Load( RTTITree tree, const wxString &path );
    bool Save( RTTITree tree, const wxString &path, int indent = 4 );

    wxXmlNode *Serialize( RTTITree tree );

    void    SerializeObject     ( Object    object,     wxXmlNode *objectNode );
    void    SerializeProperty   ( Property  property,   wxXmlNode *objectNode );
    void    SerializeEvent      ( Event     event,      wxXmlNode *objectNode );

    Object  CreateObject( RTTITree tree, Object parent, wxXmlNode *parentNode,
                            bool isReference = false );
};

#endif //__WXGUIDESIGNER_XRC_SERIALIZER_H__
