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
    bool        Load                  ( RTTITree tree, const wxString &filePath );
    bool        Save                  ( RTTITree tree, const wxString &filePath );

    Object      CreateObject          ( RTTITree tree, Object parent,
                                        wxXmlNode *parentNode,
                                        bool isReference = false );

    bool        Serialize             ( RTTITree tree, wxXmlNode *rootNode );

    wxXmlNode   *SerializeObject      ( Object object,    wxXmlNode *parent );
    wxXmlNode   *SerializeChildren    ( Objects children, wxXmlNode *parent );
    wxXmlNode   *SerializeProperties  ( Properties props, wxXmlNode *parent );
};

#endif //__WXGUIDESIGNER_XRC_SERIALIZER_H__
