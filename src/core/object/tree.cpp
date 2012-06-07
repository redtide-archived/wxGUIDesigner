///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/tree.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/object/tree.h"
#include "core/utils.h"
#include "core/xrc/object.h"

#include <wx/bitmap.h>
#include <wx/gdicmn.h>
#include <wx/event.h>
#include <wx/filename.h>
#include <wx/font.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>

#include <wx/log.h>

using namespace std;
//*****************************************************************************
// PropertyNode Class
//*****************************************************************************

PropertyNode::PropertyNode( PropertyInfo info )
            : m_info( info ), m_value( info->GetDefaultValue() )
{
}

PropertyNode::~PropertyNode()
{
    if( !m_value.IsNull() )
        m_value.MakeNull();

    m_children.clear();
}

wxString PropertyNode::GetName() const
{
    return m_info->GetName();
}

wxString PropertyNode::GetLabel() const
{
    return m_info->GetLabel();
}

wxString PropertyNode::GetDescription() const
{
    return m_info->GetDescription();
}

PropertyType PropertyNode::GetType() const
{
    return m_info->GetType();
}

PropertyInfo PropertyNode::GetInfo() const
{
    return m_info;
}

size_t PropertyNode::GetChildCount()
{
    return m_children.size();
}

void PropertyNode::AddChild( Property prop )
{
    m_children.push_back( prop );
}

void PropertyNode::AddAttribute( const wxString &name, const wxString &value )
{
    Attribute attr = std::make_pair( name, value );

    m_attributes.push_back( attr );
}

Attribute PropertyNode::GetAttribute( size_t index ) const
{
    if( index < m_attributes.size() )
        return m_attributes.at( index );

    return Attribute();
}

wxString PropertyNode::GetAttributeName( size_t index )  const
{
    if( index < m_attributes.size() )
        return m_attributes.at( index ).first;

    return wxEmptyString;
}

wxString PropertyNode::GetAttributeValue( size_t index )  const
{
    if( index < m_attributes.size() )
        return m_attributes.at( index ).second;

    return wxEmptyString;
}

size_t PropertyNode::GetAttributeCount()
{
    return m_attributes.size();
}
// ----------------------------------------------------------------------------
// Getters
// ----------------------------------------------------------------------------
wxArrayString PropertyNode::GetAsArrayString() const
{
    if( m_value.CheckType< wxArrayString >() )
        return m_value.As< wxArrayString >();

    return wxArrayString();
}

wxBitmap PropertyNode::GetAsBitmap() const
{
    if( m_value.CheckType< wxBitmap >() )
        return m_value.As< wxBitmap >();

    return wxBitmap();
}

bool PropertyNode::GetAsBool() const
{
    if( m_value.CheckType< bool >() )
        return m_value.As< bool >();

    return false;
}

Colour PropertyNode::GetAsColour() const
{
    if( m_value.CheckType< wxString >() )
    {
        wxString strCol = m_value.As< wxString >();
        if( strCol.StartsWith("wxSYS_COLOUR_") )
        {
            int sysColIdx = wxGDConv::GetSystemColourIndex( strCol );
            if( sysColIdx != wxNOT_FOUND )
            {
                Colour sysCol = { wxGDConv::GetSystemColourIndex( strCol ) };
                return sysCol;
            }
        }
    }
    else if( m_value.CheckType< Colour >() )
    {
        return m_value.As< Colour >();
    }

    return Colour();
}

double PropertyNode::GetAsDouble() const
{
    if( m_value.CheckType< double >() )
        return m_value.As< double >();

    return 0.0;
}

wxFont PropertyNode::GetAsFont() const
{
    if( m_value.CheckType< wxString >() )
    {
        wxString strFont = m_value.As< wxString >();
        if( strFont.StartsWith("wxSYS_") && strFont.EndsWith("_FONT") )
        {
            return wxGDConv::GetSystemFont( strFont );
        }
    }
    else if( m_value.CheckType< wxFont >() )
    {
        return m_value.As< wxFont >();
    }
    return wxNullFont;
}

int PropertyNode::GetAsInt() const
{
    if( m_value.CheckType< wxString >() )
    {
        return wxGDConv::IntFromString( m_value.As< wxString >() );
    }
    else if( m_value.CheckType< int >() )
    {
        return m_value.As< int >();
    }
    return 0;
}

wxPoint PropertyNode::GetAsPoint() const
{
    if( m_value.CheckType< wxPoint >() )
        return m_value.As< wxPoint >();

    return wxDefaultPosition;
}

wxSize PropertyNode::GetAsSize() const
{
    if( m_value.CheckType< wxSize >() )
        return m_value.As< wxSize >();

    return wxDefaultSize;
}

wxString PropertyNode::GetAsString() const
{
    if( m_value.CheckType< wxString >() )
        return m_value.As< wxString >();

    return wxEmptyString;
}

int PropertyNode::GetAsStyle() const
{
    if( m_value.CheckType< int >() )
        return m_value.As< int >();

    return 0;
}

wxString PropertyNode::GetAsText() const
{
    return GetAsString(); // TODO
}

wxString PropertyNode::GetAsURL() const
{
    return GetAsString(); // TODO
}

Property PropertyNode::GetChild( size_t index  ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return Property();
}

void PropertyNode::SetValue( const wxAny &value )
{
    m_value = value;
}
//*****************************************************************************
// ObjectNode Class
//*****************************************************************************

ObjectNode::ObjectNode( ClassInfo classInfo, Object parent,
                        bool expanded, bool isReference )
:
m_info( classInfo ),
m_parent( parent ),
m_expanded( expanded ),
m_isRef( isReference )
{
}

ObjectNode::~ObjectNode()
{
    m_props.clear();
    m_events.clear();
    m_children.clear();
}

wxString ObjectNode::GetClassName() const
{
    return m_info->GetName();
}

wxString ObjectNode::GetDescription() const
{
    return m_info->GetDescription();
}

wxString ObjectNode::GetName()
{
    Property prop = GetProperty("name");
    if( prop.get() )
        return prop->GetAsString();

    return wxEmptyString;
}

ClassInfo ObjectNode::GetClassInfo() const
{
    return m_info;
}

bool ObjectNode::IsRoot()
{
    return m_info->GetType() == CLASS_TYPE_ROOT;
}

bool ObjectNode::IsExpanded()
{
    return m_expanded;
}

void ObjectNode::Collapse()
{
    m_expanded = false;
}

void ObjectNode::Expand()
{
    m_expanded = true;
}

void ObjectNode::AddEvent( Event event )
{
    m_events.push_back( event );
}

size_t ObjectNode::GetEventCount()
{
    return m_events.size();
}

Object ObjectNode::GetChild( size_t index )
{
    if( index < m_children.size() )
        return m_children.at( index );

    return Object();
}

void ObjectNode::AddChild( Object child )
{
    m_children.push_back( child );
}

Object ObjectNode::GetParent()
{
    return m_parent;
}

size_t ObjectNode::GetChildCount()
{
    return m_children.size();
}
//-----------------------------------------------------------------------------
// Events
//-----------------------------------------------------------------------------

Event ObjectNode::GetEvent( size_t index )
{
    if( index < m_events.size() )
        return m_events.at( index );

    return Event();
}

Event ObjectNode::GetEvent( const wxString &name )
{
    for( Events::iterator it = m_events.begin(); it != m_events.end(); ++it )
    {
        if( (*it)->GetName() == name )
            return *it;
    }

    return Event();
}
//-----------------------------------------------------------------------------
// Properties
//-----------------------------------------------------------------------------

void ObjectNode::AddProperty( Property prop )
{
    // TODO: need to be recursive?
    if( prop.get() )
    {
        m_props.push_back( prop );

        for( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
        {
            PropertyInfo childInfo = prop->GetInfo()->GetChild( i );
            Property     child( new PropertyNode( childInfo ) );

            child->SetValue( childInfo->GetDefaultValue() );
            prop->AddChild( child );
        }
    }
}

Property ObjectNode::GetProperty( size_t index )
{
    if( index < m_props.size() )
        return m_props.at( index );

    return Property();
}

Property ObjectNode::GetChildProperty( Property parent, const wxString &name )
{
    if( parent.get() )
    {
        for( size_t i = 0; i < parent->GetChildCount(); i++ )
        {
            Property prop = parent->GetChild( i );
            if( prop.get() && prop->GetName() == name )
                return prop;
        }
    }
    return Property();
}

Property ObjectNode::GetProperty( const wxString &name )
{
    for( Properties::iterator it = m_props.begin(); it != m_props.end(); ++it )
    {
        if( (*it)->GetName() == name )
            return *it;

        Property prop = GetChildProperty( *it, name );
        if( prop.get() && prop->GetName() == name )
            return prop;
    }

    return Property();
}

size_t ObjectNode::GetPropertyCount()
{
    return m_props.size();
}

bool ObjectNode::PropertyExists( const wxString &name )
{
    return m_info->PropertyInfoExists( name );
}
//-----------------------------------------------------------------------------
// Inherited classes
//-----------------------------------------------------------------------------

void ObjectNode::AddBaseInfo( ClassInfo info )
{
    m_baseinfos.push_back( info );
}

wxString ObjectNode::GetBaseName( size_t index )
{
    if( m_info.get() )
        return m_info->GetBaseName( index );

    return wxEmptyString;
}

ClassInfo ObjectNode::GetBaseInfo( size_t index )
{
    return ClassInfoDB::Get()->GetClassInfo( GetBaseName( index ) );
}

size_t ObjectNode::GetBaseCount()
{
    return m_info->GetBaseCount();
}

wxXmlNode *ObjectNode::Serialize( wxXmlNode *parent )
{
    wxString objType = m_isRef ? "object_ref" : "object";
    wxString objName = GetName();

    wxXmlNode *objNode = new wxXmlNode( parent, wxXML_ELEMENT_NODE, objType );
    objNode->AddAttribute( "class", GetClassName() );
    objNode->AddAttribute( "name", GetName() );
    objNode->AddAttribute( "expanded", m_expanded ? "1" : "0" );

    wxXmlNode *propNode = SerializeProperties( m_props, objNode );
    if( propNode )
        objNode->AddChild( propNode );

    for( Events::iterator it = m_events.begin(); it != m_events.end(); ++it )
    {
        Event event = *it;

        if( event->HasFunctions() )
        {
            wxXmlNode *eventNode =
            new wxXmlNode( objNode, wxXML_ELEMENT_NODE, "event" );
            eventNode->AddAttribute( "class", event->GetName() );

            for( size_t i = 0; i < event->GetTypeCount(); i++ )
            {
                wxString fName = event->GetFunctionName(i);
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

    SerializeChildren( m_children, objNode );

    return objNode;
}

wxXmlNode *ObjectNode::SerializeChildren( Objects children, wxXmlNode *parent )
{
    wxXmlNode *objNode = NULL;

    for( Objects::iterator it = children.begin(); it != children.end(); ++it )
    {
        Object obj = *it;
        objNode = obj->Serialize( parent );
    }

    return objNode;
}

wxXmlNode *ObjectNode::SerializeProperties( Properties props, wxXmlNode *parent )
{
    wxXmlNode *propNode = NULL;

    for( Properties::iterator it = props.begin(); it != props.end(); ++it )
    {
        propNode      = NULL;
        Property prop = *it;
        wxString name = prop->GetName();

        // In XRC the "name" property is an attribute, skip it:
        // it is set in Serialize()
        if( name == "name" )
            continue;

        if( prop->IsCategory() )
        {
            propNode = SerializeProperties( prop->GetChildren(), parent );

            if( propNode )
                parent->AddChild( propNode );

            continue;
        }

        wxString value  = prop->GetAsString();
        size_t   count  = prop->GetAttributeCount();
        bool hasValue   = !value.empty();

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
            wxLogDebug("%s %s", attrName, attrVal);
            propNode->AddAttribute( attrName, attrVal );
        }
    }

    return propNode;
}
//*****************************************************************************
// ObjectTree Singleton Class
//*****************************************************************************

ObjectTree::ObjectTree()
:
m_root
(
    new ObjectNode
    (
        ClassInfoDB::Get()->GetClassInfo("Project"),
        Object()
    )
)
{
    m_sel = m_root;
}

ObjectTree::~ObjectTree()
{
    m_sel  = Object();
    m_root = Object();
}

size_t ObjectTree::GetChildInfoCount( Object parent, ClassInfo info )
{
    size_t count = 0;

    for( size_t i = 0; i < parent->GetChildCount(); i++ )
    {
        if( parent->GetChild( i )->GetClassInfo() == info )
            count++;
    }
    return count;
}

Object ObjectTree::CreateObject( const wxString &className, Object parent )
{
//  - Get the requested object class information
//  - Count how many children are allowed for the current object
//  - and how many are already created

//  This function is not called for the special root object.

    ClassInfo info = ClassInfoDB::Get()->GetClassInfo( className );
    if( !info.get() )
    {
        wxLogError(_("Unknown object type") );
        return Object();
    }

    if( !parent.get() )
        parent = m_sel;

    bool   allow    = false;
    size_t childCount = GetChildInfoCount( parent, info );

    // Try all parents
    while( parent.get() )
    {
        allow = parent->GetClassInfo()->IsChildInfoOk( className, childCount );

        if( allow )
        {
            m_sel = parent;
            break;
        }

        parent = parent->GetParent();
    }

    if( !allow )
    {
        if( childCount > 0 )
        {
            wxLogError
            (
                "%s allows only %i %s instances as its child(ren)",
                m_sel->GetClassName(), childCount, className
            );
        }
        else
        {
            wxLogError
            (
                "%s isn't an allowed child type for class %s",
                className, m_sel->GetClassName()
            );
        }

        return Object();
    }

    // Create the object
    Object object( new ObjectNode( info, m_sel ) );
    m_sel->AddChild( object );

    // Add properties
    for( size_t i = 0; i < info->GetPropertyInfoCount(); i++ )
    {
        Property prop( new PropertyNode( info->GetPropertyInfo( i ) ) );
        object->AddProperty( prop );
    }

    // Add events
    for( size_t i = 0; i < info->GetEventInfoCount(); i++ )
    {
        Event evt( new EventNode( info->GetEventInfo( i ) ) );
        object->AddEvent( evt );
    }

    // Add inherited class informations
    for( size_t i = 0; i < info->GetBaseCount(); i++ )
    {
        wxString     baseName = info->GetBaseName( i );
        ClassInfo    baseInfo = ClassInfoDB::Get()->GetClassInfo( baseName );

        if( baseInfo.get() )
        {
            PropertyInfo catInfo( new PropertyInfoNode
            ( PROPERTY_TYPE_CATEGORY, baseName, baseName ) );

            Property category( new PropertyNode( catInfo ) );
            object->AddProperty( category );

            for( size_t n = 0; n < baseInfo->GetPropertyInfoCount(); n++ )
            {
                // Is it possible that the inherit class have a property
                // with same name, skip it (overrided method)
                PropertyInfo info = baseInfo->GetPropertyInfo( n );
                if( !object->PropertyExists( info->GetName() ) )
                {
                    Property prop( new PropertyNode( info ) );
                    category->AddChild( prop );
                }
            }

            for( size_t n = 0; n < baseInfo->GetEventInfoCount(); n++ )
            {
                Event evt( new EventNode( baseInfo->GetEventInfo( n ) ) );
                object->AddEvent( evt );
            }
        }
    }

    return object;
}

void ObjectTree::SelectObject( Object object, bool withEvent )
{
    wxASSERT( object != m_sel );

    m_sel = object;

    // Forward the event to other handlers if needed.
//  if( withEvent )
//      SendEvent( object, EVT_OBJECT_SELECTED );
}

bool ObjectTree::Load( const wxString &filePath )
{
    wxXmlDocument doc;
    if( !doc.Load( filePath ) )
        return false;

    if( doc.GetRoot()->GetName() != "resource" )
        return false;

    Object      parentObject = Object();                     // root
    Object      childObject  = Object();                     // frame
    wxXmlNode   *childNode   = doc.GetRoot()->GetChildren(); // frame

    while( childNode )
    {
        wxString childName = childNode->GetName();
        if( childName == "object" )
        {
            childObject = DoCreateObject( parentObject, childNode );
        }
        else if( childName == "object_ref")
        {
            wxString objectRef  = childNode->GetAttribute("ref");
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

Object ObjectTree::DoCreateObject( Object parentObject, wxXmlNode *childNode,
                                                            bool isReference )
{
    Object childObject = Object();

    if( childNode && (childNode->GetName() == "object") )
    {
        // Set 'name' and 'class' attributes
        wxString className  = childNode->GetAttribute("class");
        wxString subclsName = childNode->GetAttribute("subclass"); // TODO

        // Create the object
        childObject = CreateObject( className, parentObject );

        if( childObject.get() )
        {
            // Get all object's property values
            wxXmlNode *subNode = childNode->GetChildren();
            while( subNode )
            {
                wxString nodeName = subNode->GetName();
                if( nodeName == "object" )
                {
                    // Get all object's children recursively
                    DoCreateObject( childObject, subNode );

                    Property prop  = childObject->GetProperty("name");
                    if( prop.get() )
                        prop->SetValue( subNode->GetAttribute("name") );
                }
                else if( nodeName == "object_ref" )
                {
                    // Get all object references recursively
                    // TODO: insert_at and ref attributes
                    DoCreateObject( childObject, subNode, true );

                    Property prop    = childObject->GetProperty("name");
                    wxString refName = subNode->GetAttribute("ref");

                    if( prop.get() )
                    {
                        prop->SetValue( subNode->GetAttribute("name") );
                        prop->AddAttribute( "ref", refName );
                    }
                }
                // Get all object's properties, excluding 'name' one:
                // we use it as property, but XRC objects use it as attribute
                else if( (nodeName != "event") && (nodeName != "name") )
                {
                    Property prop  = childObject->GetProperty( nodeName );
                    wxString value = subNode->GetNodeContent();

                    if( prop.get() )
                    {
                        if( !value.empty() )
                        {
                            wxLogDebug( "Setting property %s to %s", nodeName, value );
                            prop->SetValue( value );
                        }

                        wxXmlAttribute *attr = subNode->GetAttributes();
                        while( attr )
                        {
                            wxString attrName = attr->GetName();
                            wxLogDebug( "Adding attribute %s: %s", attrName, attr->GetValue() );
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
                    wxLogDebug( "Setting %s %s", nodeName, subNode->GetAttribute("class") );
                }

                subNode = subNode->GetNext();
            }
        }
    }

    return childObject;
}

bool ObjectTree::Serialize( const wxString &filePath )
{
    if( !m_root.get() )
        return false;

    wxFileName fileName = filePath;
    if( !fileName.DirExists() )
        return false;

    wxXmlDocument doc;

    wxXmlNode *root = new wxXmlNode( NULL, wxXML_ELEMENT_NODE, "resource" );
    root->AddAttribute( "xmlns", "http://www.wxwidgets.org/wxxrc" );
    root->AddAttribute( "version", "2.5.3.0" );

    m_root->SerializeChildren( m_root->GetChildren(), root );

    doc.SetRoot( root );

    return doc.Save( filePath, 4 );
}
