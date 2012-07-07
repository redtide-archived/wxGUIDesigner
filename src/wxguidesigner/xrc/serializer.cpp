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
#include <wx/filename.h>
#include <wx/xml/xml.h>

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
    if( !tree || !tree->GetRootObject() )
        return false;

    wxFileName fileName = path;
    if( !fileName.DirExists() )
        return false;

    wxXmlDocument doc;

    wxXmlNode *rootNode = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "resource" );
    rootNode->AddAttribute( "xmlns", "http://www.wxwidgets.org/wxxrc" );
    rootNode->AddAttribute( "version", "2.5.3.0" );

    SerializeChildren( tree->GetRootObject()->GetChildren(), rootNode );

    doc.SetRoot( rootNode );

    return doc.Save( path, indent );
}

void wxXRCSerializer::Serialize( RTTITree tree, wxXmlNode *rootNode )
{
    if( !tree || !rootNode )
        return;

    Object root = tree->GetRootObject();

    if( !root )
        return;

    SerializeChildren( root->GetChildren(), rootNode );
}

void wxXRCSerializer::SerializeObject( Object object, wxXmlNode *parent )
{
    if( !object || !parent )
        return;

    wxString objType = object->IsReference() ? "object_ref" : "object";
    wxString objName = object->GetName();

    wxXmlNode *objNode = new wxXmlNode( parent, wxXML_ELEMENT_NODE, objType );
    objNode->AddAttribute( "class", object->GetClassName() );
    objNode->AddAttribute( "name", object->GetName() );
    objNode->AddAttribute( "expanded", object->IsExpanded() ? "1" : "0" );

    SerializeProperties ( object->GetProperties(),  objNode );
    SerializeEvents     ( object->GetEvents(),      objNode );
    SerializeChildren   ( object->GetChildren(),    objNode );
}

void wxXRCSerializer::SerializeChildren( Objects children, wxXmlNode *parent )
{
    for( Objects::iterator it = children.begin(); it != children.end(); ++it )
    {
        Object object = *it;
        SerializeObject( object, parent );
    }
}

void wxXRCSerializer::SerializeProperties( Properties props, wxXmlNode *parent )
{
    for( Properties::iterator it = props.begin(); it != props.end(); ++it )
    {
        Property prop = *it;
        wxString name = prop->GetName();

        // In XRC the "name" property is an attribute, skip it:
        // it is set in Serialize()
        if( name == "name" )
            continue;

        if( prop->IsCategory() )
        {
            SerializeProperties( prop->GetChildren(), parent );

            continue;
        }

        wxString value  = prop->GetAsString();
        size_t   count  = prop->GetAttributeCount();
        bool hasValue   = !value.empty();

        wxXmlNode *propNode = NULL;

        if( hasValue || count )
            propNode = new wxXmlNode( parent, wxXML_ELEMENT_NODE, name );
        else
            continue;

        if( hasValue )
        {
            new wxXmlNode( propNode, wxXML_TEXT_NODE, "", value );
        }

        for( size_t i = 0; i < count; i++ )
        {
            wxString attrName = prop->GetAttributeName(i);
            wxString attrVal  = prop->GetAttributeValue(i);
//          wxLogDebug("%s %s", attrName, attrVal);
            propNode->AddAttribute( attrName, attrVal );
        }
    }
}

void wxXRCSerializer::SerializeEvents( Events events, wxXmlNode *parent )
{
    for( Events::iterator it = events.begin(); it != events.end(); ++it )
    {
        Event event = (*it);

        if( event && event->HasHandlers() )
        {
            wxXmlNode *eventNode =
            new wxXmlNode( parent, wxXML_ELEMENT_NODE, "event" );
            eventNode->AddAttribute( "class", event->GetName() );

            for( size_t i = 0; i < event->GetTypeCount(); i++ )
            {
                wxString fName = event->GetHandlerName(i);
                if( !fName.empty() )
                {
                    wxXmlNode *tNode =
                    new wxXmlNode( eventNode, wxXML_ELEMENT_NODE, "type" );
                    tNode->AddAttribute( "name", event->GetTypeName(i) );

                    wxXmlNode *fNode =
                    new wxXmlNode( tNode, wxXML_ELEMENT_NODE, "function" );
                    new wxXmlNode( fNode, wxXML_TEXT_NODE, "", fName );
                }
            }
        }
    }
}

Object wxXRCSerializer::CreateObject  ( RTTITree tree, Object parent,
                                        wxXmlNode *childNode, bool isReference )
{
    Object child = Object();

    if( childNode && (childNode->GetName() == "object") )
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

                    Property prop  = child->GetProperty("name");
                    if( prop )
                        prop->SetValue( subNode->GetAttribute("name") );
                }
                else if( nodeName == "object_ref" )
                {
                    // Get all object references recursively
                    // TODO: insert_at and ref attributes
                    CreateObject( tree, child, subNode, true );

                    Property prop    = child->GetProperty("name");
                    wxString refName = subNode->GetAttribute("ref");

                    if( prop )
                    {
                        prop->SetValue( subNode->GetAttribute("name") );
                        prop->AddAttribute( "ref", refName );
                    }
                }
                // Get all object's properties, excluding 'name' one:
                // we use it as property, but XRC objects use it as attribute
                else if( (nodeName != "event") && (nodeName != "name") )
                {
                    Property prop  = child->GetProperty( nodeName );
                    wxString value = subNode->GetNodeContent();

                    if( prop )
                    {
                        if( !value.empty() )
                        {
//                          wxLogDebug( "Setting property %s to %s", nodeName, value );
                            prop->SetValue( value );
                        }

                        wxXmlAttribute *attr = subNode->GetAttributes();
                        while( attr )
                        {
                            wxString attrName = attr->GetName();
//                          wxLogDebug( "Adding attribute %s: %s", attrName, attr->GetValue() );
                            prop->AddAttribute( attrName, attr->GetValue() );
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
