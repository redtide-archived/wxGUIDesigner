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

bool wxXRCSerializer::Load( RTTITree tree, const wxString &path )
{
    wxXmlDocument doc;
    if( !tree || !doc.Load( path ) )
        return false;

    if( doc.GetRoot()->GetName() != "resource" )
        return false;

    Object    parent     = Object();                     // root
    Object    child      = Object();                     // frame
    wxXmlNode *childNode = doc.GetRoot()->GetChildren(); // frame

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

bool wxXRCSerializer::Save( RTTITree tree, const wxString &path, int indent )
{
    wxFileName fileName = path;
    if( !tree || !fileName.DirExists() )
        return false;

    wxXmlDocument xmlDocument = Serialize( tree );

    return xmlDocument.Save( path, indent );
}

wxXmlDocument wxXRCSerializer::Serialize( RTTITree tree )
{
    wxXmlDocument xmlDocument;

    int xrcVerSel;
    wxString xrcVer = "2.5.3.1";
    wxConfigBase::Get()->Read( "locale/selected", &xrcVerSel, 1 );

    if( xrcVerSel == 0 )
        xrcVer = "2.3.0.1";

    wxXmlNode *rootNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "resource" );
    rootNode->AddAttribute( "xmlns", "http://www.wxwidgets.org/wxxrc" );
    rootNode->AddAttribute( "version", xrcVer );

    xmlDocument.SetRoot( rootNode );

    if( !tree )
        return xmlDocument;

    Object root = tree->GetRootObject();

    if( !root )
        return xmlDocument;

    // Serialize toplevel objects (no events / properties for root node)
    SerializeChildren( root->GetChildren(), rootNode );

    return xmlDocument;
}

void wxXRCSerializer::SerializeObject( Object object, wxXmlNode *parent )
{
    if( !object || !parent )
        return;

    wxString   objType = object->IsReference() ? "object_ref" : "object";
    wxXmlNode *objNode = new wxXmlNode( parent, wxXML_ELEMENT_NODE, objType );

    objNode->AddAttribute( "class", object->GetClassName() );
    objNode->AddAttribute( "name",  object->GetName() );

    if( object->IsExpanded() )
        objNode->AddAttribute( "expanded", "1" );

    SerializeProperties ( object->GetProperties(),  objNode );
    SerializeEvents     ( object->GetEvents(),      objNode );
    SerializeChildren   ( object->GetChildren(),    objNode );
}

void wxXRCSerializer::SerializeChildren( Objects children, wxXmlNode *parent )
{
    for( Objects::const_iterator it = children.begin(); it != children.end(); ++it )
    {
        Object object = *it;
        SerializeObject( object, parent );
    }
}

void wxXRCSerializer::SerializeProperties( Properties props, wxXmlNode *parent )
{
    for( Properties::const_iterator it = props.begin(); it != props.end(); ++it )
    {
        Property property = *it;
        wxString     name = property->GetName();
        PropertyType type = property->GetType();

        // In XRC the "name" property is an attribute, skip it:
        // it is set in Serialize()
        if( name == "name" )
        {
            continue;
        }
        else if( type == PROPERTY_TYPE_BITMAP )
        {
            wxString value = property->GetAsString();
            if( value.empty() )
                continue;

            wxArrayString attributes = wxStringTokenize( value, ";" );
            size_t count = attributes.GetCount();
            if( count < 2 )
                continue;

            int bitmapType = wxAtoi( attributes.Item(0) );

            if( bitmapType == 1 ) // wxArtProvider
            {
                wxXmlNode *bitmapNode =
                new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );

                if( count > 1 )
                {
                    wxString artId  = attributes.Item(1);
                    bitmapNode->AddAttribute("stock_id", artId );
                }

                if( count > 2 )
                {
                    wxString client = attributes.Item(2);
                    bitmapNode->AddAttribute("stock_client", client );
                }

                if( count > 3 )
                {
                    wxString size   = attributes.Item(3);
                    bitmapNode->AddAttribute("size", size ); // TODO
                }

            }
            else if( bitmapType == 2 ) // File
            {
                value = attributes.Item(1);
                wxXmlNode *bitmapNode =
                new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );
                new wxXmlNode( bitmapNode, wxXML_TEXT_NODE, "", value );
            }

            continue;
        }
        else if( type == PROPERTY_TYPE_BOOL )
        {
            bool value = property->GetAsBool();
            if( !value )
                continue;

            wxXmlNode *boolNode =
            new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );
            new wxXmlNode( boolNode, wxXML_TEXT_NODE, "", "1" );

            continue;
        }
        else if( type == PROPERTY_TYPE_CATEGORY )
        {
            SerializeProperties( property->GetChildren(), parent );
            continue;
        }
        else if( type == PROPERTY_TYPE_COLOUR )
        {
            wxString value = property->GetAsString();
            if( value.empty() )
                continue;
            else if( !value.Contains("wxSYS_COLOUR_") )
                value.Prepend("rgb(").Append(")");

            wxXmlNode *colourNode =
            new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );
            new wxXmlNode( colourNode, wxXML_TEXT_NODE, "", value );

            continue;
        }
        else if( type == PROPERTY_TYPE_FONT )
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
                continue;

            wxXmlNode *fontNode =
            new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );

            // Font face
            if( !face.empty() && (face != _("Default")) )
            {
                wxXmlNode *faceNode =
                new wxXmlNode( fontNode, wxXML_ELEMENT_NODE, "face" );
                new wxXmlNode( faceNode, wxXML_TEXT_NODE, "", face );
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

                wxXmlNode *styleNode =
                new wxXmlNode( fontNode, wxXML_ELEMENT_NODE, "style" );
                new wxXmlNode( styleNode, wxXML_TEXT_NODE, "", value );
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

                wxXmlNode *weightNode =
                new wxXmlNode( fontNode, wxXML_ELEMENT_NODE, "weight" );
                new wxXmlNode( weightNode, wxXML_TEXT_NODE, "", value );
            }

            // Font size
            if( size > 0 )
            {
                wxString value = wxString::Format( "%i", size );
                wxXmlNode *sizeNode =
                new wxXmlNode( fontNode, wxXML_ELEMENT_NODE, "size" );
                new wxXmlNode( sizeNode, wxXML_TEXT_NODE, "", value );
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

                wxXmlNode *familyNode =
                new wxXmlNode( fontNode, wxXML_ELEMENT_NODE, "family" );
                new wxXmlNode( familyNode, wxXML_TEXT_NODE, "", value );
            }

            // Font underlined
            if( underlined )
            {
                wxXmlNode *underlinedNode =
                new wxXmlNode( fontNode, wxXML_ELEMENT_NODE, "underlined" );
                new wxXmlNode( underlinedNode, wxXML_TEXT_NODE, "", "1" );
            }

            if( encoding != wxFONTENCODING_DEFAULT )
            {
                wxString value =
                wxFontMapper::GetEncodingName( (wxFontEncoding)encoding );
                wxXmlNode *encodingNode =
                new wxXmlNode( fontNode, wxXML_ELEMENT_NODE, "encoding" );
                new wxXmlNode( encodingNode, wxXML_TEXT_NODE, "", value );
            }

            continue;
        }
        else if( (type == PROPERTY_TYPE_POINT) || (type == PROPERTY_TYPE_SIZE) )
        {
            wxString value = property->GetAsString();
            if( value.empty() || value == "-1,-1" )
                continue;

            wxXmlNode *node =
            new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );
            new wxXmlNode( node, wxXML_TEXT_NODE, "", value );

            continue;
        }

        wxString value    = property->GetAsString();
        size_t   count    = property->GetAttributeCount();
        bool     hasValue = !value.empty();

        wxXmlNode *propNode = NULL;

        if( hasValue || count )
            propNode = new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );
        else
            continue;

        if( hasValue )
            new wxXmlNode( propNode, wxXML_TEXT_NODE, "", value );

        for( size_t i = 0; i < count; i++ )
        {
            wxString attrName = property->GetAttributeName(i);
            wxString attrVal  = property->GetAttributeValue(i);
//          wxLogDebug("%s %s", attrName, attrVal);
            propNode->AddAttribute( attrName, attrVal );
        }
    }
}

void wxXRCSerializer::SerializeEvents( Events events, wxXmlNode *parent )
{
    for( Events::const_iterator it = events.begin(); it != events.end(); ++it )
    {
        Event event = (*it);

        if( event && event->HasHandlers() )
        {
            wxXmlNode *eventNode =
            new wxXmlNode( parent, wxXML_ELEMENT_NODE, "event" );
            eventNode->AddAttribute( "class", event->GetName() );

            for( size_t i = 0; i < event->GetTypeCount(); i++ )
            {
                wxString handler = event->GetHandlerName(i);

                if( !handler.empty() )
                {
                    wxXmlNode *typeNode =
                    new wxXmlNode( eventNode, wxXML_ELEMENT_NODE, "type" );
                    new wxXmlNode( typeNode,  wxXML_TEXT_NODE, "", handler );

                    typeNode->AddAttribute( "name", event->GetTypeName(i) );
                }
            }
        }
    }
}

Object wxXRCSerializer::CreateObject  ( RTTITree tree, Object parent,
                                        wxXmlNode *childNode, bool isReference )
{
    Object child = Object();

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

                    Property property  = child->GetProperty("name");
                    if( property )
                        property->SetValue( subNode->GetAttribute("name") );
                }
                else if( nodeName == "object_ref" )
                {
                    // Get all object references recursively
                    // TODO: insert_at and ref attributes
                    CreateObject( tree, child, subNode, true );

                    Property property    = child->GetProperty("name");
                    wxString refName = subNode->GetAttribute("ref");

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
                    Property property  = child->GetProperty( nodeName );
                    wxString value = subNode->GetNodeContent();

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
