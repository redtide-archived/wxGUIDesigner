///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/xrc/serializer.cpp
// Purpose:     XRC File Parser
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/13
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/fontmap.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>

#include "wxguidesigner/fontcontainer.h"
#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/xrc/serializer.h"

bool wxGD::XRCSerializer::Load( RTTI::Tree tree, const wxString &path )
{
    wxXmlDocument doc;
    if( !tree || !doc.Load( path ) )
        return false;

    if( doc.GetRoot()->GetName() != "resource" )
        return false;

    RTTI::Object    parent     = RTTI::Object();                // root
    RTTI::Object    child      = RTTI::Object();                // frame
    wxXmlNode       *childNode = doc.GetRoot()->GetChildren();  // frame

    while( childNode )
    {
        wxString childName = childNode->GetName();
        if( childName == "object" )
        {
            child = CreateObject( tree, parent, childNode );
        }
        else if( childName == "object_ref")
        {
            wxString objectRef = childNode->GetAttribute("ref");
            wxString objectID  = childNode->GetAttribute("name");
        }
        else if( childName == "event")
        {
            wxString className = childNode->GetAttribute("class");
        }
        else // property
        {
            
        }

        childNode = childNode->GetNext();
    }

    return true;
}

bool wxGD::XRCSerializer::Save( RTTI::Tree tree, const wxString &path, int indent )
{
    wxFileName fileName = path;
    if( !tree || !fileName.DirExists() )
        return false;

    wxXmlDocument xmlDocument;
    xmlDocument.SetRoot( Serialize( tree ) );

    return xmlDocument.Save( path, indent );
}

wxXmlNode *wxGD::XRCSerializer::Serialize( RTTI::Tree tree )
{
    if( !tree )
        return NULL;

    RTTI::Object root = tree->GetRootObject();

    if( !root )
        return NULL;
/*
    int xrcVerSel;
    wxString xrcVer = "2.5.3.1";
    wxConfigBase::Get()->Read( "locale/selected", &xrcVerSel, 1 );

    if( xrcVerSel == 0 )
        xrcVer = "2.3.0.1";
*/
    wxXmlNode *rootNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "resource" );
/*
    rootNode->AddAttribute( "xmlns", "http://www.wxwidgets.org/wxxrc" );
    rootNode->AddAttribute( "version", xrcVer );
*/
    // Serialize toplevel objects (no events / properties for root node)
    RTTI::Objects objects = root->GetChildren();
    for( RTTI::Objects::const_iterator it = objects.begin();
                                        it != objects.end(); ++it )
    {
        SerializeObject( *it, rootNode );
    }

    return rootNode;
}

void wxGD::XRCSerializer::SerializeObject( RTTI::Object object, wxXmlNode *parent )
{
    if( !object || !parent )
        return;

    wxString   objectType = object->IsReference() ? "object_ref" : "object";
    wxXmlNode *objectNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, objectType );

    objectNode->AddAttribute( "class", object->GetClassName() );
    objectNode->AddAttribute( "name",  object->GetName() );

    parent->AddChild( objectNode );

    if( object->IsExpanded() )
        objectNode->AddAttribute( "expanded", "1" );

    RTTI::Properties properties = object->GetProperties();
    for( RTTI::Properties::const_iterator it = properties.begin();
                                            it != properties.end(); ++it )
    {
        SerializeProperty( *it, objectNode );
    }

    RTTI::EventProperties events = object->GetEvents();
    for( RTTI::EventProperties::const_iterator it = events.begin();
                                                it != events.end(); ++it )
    {
        SerializeEvent( *it, objectNode );
    }

    RTTI::Objects objects = object->GetChildren();
    for( RTTI::Objects::const_iterator it = objects.begin();
                                        it != objects.end(); ++it )
    {
        SerializeObject( *it, objectNode );
    }
}

void wxGD::XRCSerializer::SerializeProperty( RTTI::Property property,
                                            wxXmlNode *objectNode )
{
    wxString            name = property->GetName();
    RTTI::PropertyType  type = property->GetType();

    wxXmlNode *propertyNode = NULL;
    wxXmlNode *childNode    = NULL;
    wxXmlNode *valueNode    = NULL;

    // In XRC the "name" property is an attribute, skip it:
    // it is set in Serialize()
    if( name == "name" )
    {
        return;
    }
    else if( type == RTTI::PROPERTY_TYPE_BITMAP )
    {
        wxString value = property->GetAsString();
        if( value.empty() )
            return;

        wxArrayString attributes = wxStringTokenize( value, ";" );
        size_t count = attributes.GetCount();
        if( count < 2 )
            return;

        wxString source = attributes.Item(0);

        if( source == _("wxArtProvider") )
        {
            propertyNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, name );

            if( count > 1 )
            {
                wxString artId = attributes.Item(1);
                propertyNode->AddAttribute("stock_id", artId );
            }

            if( count > 2 )
            {
                wxString client = attributes.Item(2);
                propertyNode->AddAttribute("stock_client", client );
            }

            if( count > 3 )
            {
                wxString size = attributes.Item(3);
                propertyNode->AddAttribute("size", size ); // TODO
            }

            objectNode->AddChild( propertyNode );
        }
        else if( source == _("File") )
        {
            value        = attributes.Item(1);
            propertyNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, name );
            valueNode    = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

            propertyNode->AddChild( valueNode );
            objectNode->AddChild( propertyNode );
        }
    }
    else if( type == RTTI::PROPERTY_TYPE_BOOL )
    {
        bool value = property->GetAsBool();
        if( !value )
            return;

        propertyNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, name );
        valueNode    = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", "1" );

        propertyNode->AddChild( valueNode );
        objectNode->AddChild( propertyNode );
    }
    else if( type == RTTI::PROPERTY_TYPE_CATEGORY )
    {
        RTTI::Properties properties = property->GetChildren();
        for( RTTI::Properties::const_iterator it = properties.begin();
                                            it != properties.end(); ++it )
        {
            SerializeProperty( *it, objectNode );
        }
    }
    else if( type == RTTI::PROPERTY_TYPE_COLOUR )
    {
        wxString value = property->GetAsString();
        if( value.empty() )
            return;

        propertyNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, name );
        valueNode    = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

        propertyNode->AddChild( valueNode );
        objectNode->AddChild( propertyNode );
    }
    else if( type == RTTI::PROPERTY_TYPE_FONT )
    {
        wxFontContainer font = property->GetAsFont();
        int     size        = font.GetPointSize();
        int     family      = font.GetFamily();
        int     style       = font.GetStyle();
        int     weight      = font.GetWeight();
        int     encoding    = font.GetEncoding();
        bool    underlined  = font.GetUnderlined();
        wxString face       = font.GetFaceName();

        if( size < 1 && family == wxFONTFAMILY_DEFAULT &&
            style  == wxFONTSTYLE_NORMAL &&
            weight == wxFONTWEIGHT_NORMAL &&
            encoding == wxFONTENCODING_DEFAULT && underlined == false &&
            ( face.empty() || (face == _("Default")) ) )
            return;

        propertyNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, name );

        // Font face
        if( !face.empty() && (face != _("Default")) )
        {
            childNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "face" );
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", face );

            childNode->AddChild( valueNode );
            propertyNode->AddChild( childNode );
        }

        // Font style
        if( style != wxFONTSTYLE_NORMAL )
        {
            wxString value;
            switch( style )
            {
                case wxFONTSTYLE_ITALIC:
                {
                    value = "italic";
                    break;
                }
                case wxFONTSTYLE_SLANT:
                {
                    value = "slant";
                    break;
                }
                default:
                {
                    value = "normal";
                    break;
                }
            }

            childNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "style" );
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

            childNode->AddChild( valueNode );
            propertyNode->AddChild( childNode );
        }

        // Font weight
        if( weight != wxFONTWEIGHT_NORMAL )
        {
            wxString value;
            switch( weight )
            {
                case wxFONTWEIGHT_LIGHT:
                {
                    value = "light";
                    break;
                }
                case wxFONTWEIGHT_BOLD:
                {
                    value = "bold";
                    break;
                }
                default:
                {
                    value = "normal";
                    break;
                }
            }

            childNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "weight" );
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

            childNode->AddChild( valueNode );
            propertyNode->AddChild( childNode );
        }

        // Font size
        if( size > 0 )
        {
            wxString value = wxString::Format( "%i", size );
            childNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "size" );
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

            childNode->AddChild( valueNode );
            propertyNode->AddChild( childNode );
        }

        // Font family
        if( family != wxFONTFAMILY_DEFAULT )
        {
            wxString value;
            switch( family )
            {
                case wxFONTFAMILY_DECORATIVE:
                {
                    value = "decorative";
                    break;
                }
                case wxFONTFAMILY_ROMAN:
                {
                    value = "roman";
                    break;
                }
                case wxFONTFAMILY_SCRIPT:
                {
                    value = "script";
                    break;
                }
                case wxFONTFAMILY_SWISS:
                {
                    value = "swiss";
                    break;
                }
                case wxFONTFAMILY_MODERN:
                {
                    value = "modern";
                    break;
                }
                case wxFONTFAMILY_TELETYPE:
                {
                    value = "teletype";
                    break;
                }
                default: // wxFONTFAMILY_DEFAULT
                {
                    value = "default";
                    break;
                }
            }

            childNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "family" );
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

            childNode->AddChild( valueNode );
            propertyNode->AddChild( childNode );
        }

        // Font underlined
        if( underlined )
        {
            childNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "underlined" );
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", "1" );

            childNode->AddChild( valueNode );
            propertyNode->AddChild( childNode );
        }

        if( encoding != wxFONTENCODING_DEFAULT )
        {
            wxString value =
            wxFontMapper::GetEncodingName( (wxFontEncoding)encoding );

            childNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "encoding" );
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

            childNode->AddChild( valueNode );
            propertyNode->AddChild( childNode );
        }

        objectNode->AddChild( propertyNode );
    }
    else if((type == RTTI::PROPERTY_TYPE_POINT) ||
            (type == RTTI::PROPERTY_TYPE_SIZE))
    {
        wxString value = property->GetAsString();
        if( value.empty() || value == "-1,-1" )
            return;

        propertyNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, name );
        valueNode    = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

        propertyNode->AddChild( valueNode );
        objectNode->AddChild( propertyNode );
    }
    else
    {
        wxString value    = property->GetAsString();
        size_t   count    = property->GetAttributeCount();
        bool     hasValue = !value.empty();

        if( hasValue || count )
            propertyNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, name );
        else
            return;

        if( hasValue )
            valueNode = new wxXmlNode( NULL, wxXML_TEXT_NODE, "", value );

        if( propertyNode )
        {
            for( size_t i = 0; i < count; i++ )
            {
                wxString attributeName  = property->GetAttributeName(i);
                wxString attributeValue = property->GetAttributeValue(i);

                propertyNode->AddAttribute( attributeName, attributeValue );
            }

            if( valueNode )
                propertyNode->AddChild( valueNode );

            objectNode->AddChild( propertyNode );
        }
    }
}

void wxGD::XRCSerializer::SerializeEvent  ( RTTI::EventProperty event,
                                            wxXmlNode           *objectNode )
{
    if( event && event->HasHandlers() )
    {
        wxXmlNode *eventNode =
            new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "event" );

        eventNode->AddAttribute( "class", event->GetName() );
        objectNode->AddChild( eventNode );

        for( size_t i = 0; i < event->GetTypeCount(); i++ )
        {
            wxString handler = event->GetHandlerName(i);

            if( !handler.empty() )
            {
                wxXmlNode *typeNode =
                    new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "type" );

                wxXmlNode *valueNode =
                    new wxXmlNode( NULL,  wxXML_TEXT_NODE, "", handler );

                typeNode->AddAttribute( "name", event->GetTypeName(i) );
                typeNode->AddChild( valueNode );
                eventNode->AddChild( typeNode );
            }
        }
    }
}

wxGD::RTTI::Object wxGD::XRCSerializer::CreateObject  ( RTTI::Tree  tree,
                                                        RTTI::Object parent,
                                                        wxXmlNode   *childNode,
                                                        bool        isReference )
{
    RTTI::Object child = RTTI::Object();

    if( tree && childNode && (childNode->GetName() == "object") )
    {
        // Set 'name' and 'class' attributes
        wxString className  = childNode->GetAttribute("class");
        wxString subclsName = childNode->GetAttribute("subclass"); // TODO

        // Create the object
        child = tree->CreateObject( className, parent );

        if( child )
        {
            // Get all object's property values
            wxXmlNode *subNode = childNode->GetChildren();
            while( subNode )
            {
                wxString nodeName = subNode->GetName();
                if( nodeName == "object" )
                {
                    // Get all object's children recursively
                    CreateObject( tree, child, subNode );

                    RTTI::Property property  = child->GetProperty("name");
                    if( property )
                        property->SetValue( subNode->GetAttribute("name") );
                }
                else if( nodeName == "object_ref" )
                {
                    // Get all object references recursively
                    // TODO: insert_at and ref attributes
                    CreateObject( tree, child, subNode, true );

                    RTTI::Property  property    = child->GetProperty("name");
                    wxString        refName     = subNode->GetAttribute("ref");

                    if( property )
                    {
                        property->SetValue( subNode->GetAttribute("name") );
                        property->AddAttribute( "ref", refName );
                    }
                }
                // Get all object's properties, excluding 'name' one:
                // we use it as property, but XRC objects use it as attribute
                else if( (nodeName != "event") && (nodeName != "name") )
                {
                    RTTI::Property  property    = child->GetProperty( nodeName );
                    wxString        value       = subNode->GetNodeContent();

                    if( property )
                    {
                        if( !value.empty() )
                        {
//                          wxLogDebug( "Setting property %s to %s", nodeName, value );
                            property->SetValue( value );
                        }

                        wxXmlAttribute *attr = subNode->GetAttributes();
                        while( attr )
                        {
                            wxString attrName = attr->GetName();
//                          wxLogDebug( "Adding attribute %s: %s", attrName, attr->GetValue() );
                            property->AddAttribute( attrName, attr->GetValue() );
/* TODO
                            // platform specific
                            if( attrName == "platform" )
                            {
                                
                            }
                            // label
                            if( attrName == "translate" )
                            {
                                
                            }
                            // wxArtProvider bitmap
                            else if( attrName == "stock_id" )
                            {
                                
                            }
                            else if( attrName == "stock_client" )
                            {
                                
                            }
                            // wxCheckListBox
                            else if( attrName == "checked" )
                            {
                                
                            }
                            // wxRadioBox
                            else if( attrName == "tooltip" )
                            {
                                
                            }
                            else if( attrName == "helptext" )
                            {
                                
                            }
                            else if( attrName == "enabled" )
                            {
                                
                            }
                            else if( attrName == "hidden" )
                            {
                                
                            }
*/
                            attr = attr->GetNext();
                        }
                    }
                }
                else
                {
                    // Get all object's events
//                  wxLogDebug( "Setting %s %s", nodeName, subNode->GetAttribute("class") );
                }

                subNode = subNode->GetNext();
            }
        }
    }

    return child;
}
