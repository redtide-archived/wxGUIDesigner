///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/tree.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/any.h>
#include <wx/sstream.h>
#include <wx/bitmap.h>
#include <wx/gdicmn.h>
#include <wx/event.h>
#include <wx/filename.h>
#include <wx/font.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>

#include "wxguidesigner/fontcontainer.h"
#include "wxguidesigner/utils.h"

#include "wxguidesigner/rtti/flags.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/tree.h"


using namespace std;
//=============================================================================
// EventNode Class
//=============================================================================
EventNode::EventNode( EventInfo eventInfo )
:
m_eventInfo( eventInfo )
{
    for( size_t i = 0; i < m_eventInfo->GetTypeCount(); i++ )
        m_handlers.push_back(wxEmptyString);
}

EventNode::~EventNode()
{
}

wxString EventNode::GetName() const
{
    return m_eventInfo->GetName();
}

wxString EventNode::GetDescription() const
{
    return m_eventInfo->GetDescription();
}

wxString EventNode::GetHandlerName( size_t typeIndex ) const
{
    if( typeIndex < m_handlers.size() )
        return m_handlers.at( typeIndex );

    return wxEmptyString;
}

wxString EventNode::GetTypeName( size_t typeIndex  ) const
{
    if( typeIndex < m_eventInfo->GetTypeCount() )
        return m_eventInfo->GetTypeName( typeIndex );

    return wxEmptyString;
}

wxString EventNode::GetTypeDescription( size_t typeIndex  ) const
{
    if( typeIndex < m_eventInfo->GetTypeCount() )
        return m_eventInfo->GetTypeDescription( typeIndex );

    return wxEmptyString;
}

size_t EventNode::GetTypeCount() const
{
    return m_eventInfo->GetTypeCount();
}

Handlers EventNode::GetHandlers() const
{
    return m_handlers;
}

bool EventNode::HasHandlers()
{
    bool hasHandlers = false;

    for( size_t i = 0; i < m_handlers.size(); i++ )
    {
        if( m_handlers.at(i) != wxEmptyString )
        {
            hasHandlers = true;
            break;
        }
    }

    return hasHandlers;
}

void EventNode::SetHandlerName( size_t typeIndex, const wxString &funcName )
{
    if( typeIndex < m_eventInfo->GetTypeCount() )
    {
        Handlers::iterator it = m_handlers.begin() + typeIndex;
        m_handlers.erase( it );
        m_handlers.insert( it, funcName );
    }
}

void EventNode::SetHandlerName( const wxString &typeName,
                                const wxString &funcName )
{
    for( size_t i = 0; i < m_eventInfo->GetTypeCount(); i++ )
    {
        if( m_eventInfo->GetTypeName(i) == typeName )
        {
            Handlers::iterator it = m_handlers.begin() + i;
            m_handlers.erase( it );
            m_handlers.insert( it, funcName );
        }
    }
}
//=============================================================================
// PropertyNode Class
//=============================================================================
PropertyNode::PropertyNode( PropertyInfo propertyInfo )
:
m_propertyInfo  ( propertyInfo ),
m_value         ( propertyInfo->GetDefaultValue() )
{
}

PropertyNode::~PropertyNode()
{
    m_children.clear();
}

wxString PropertyNode::GetName() const
{
    return m_propertyInfo->GetName();
}

wxString PropertyNode::GetLabel() const
{
    return m_propertyInfo->GetLabel();
}

wxString PropertyNode::GetDescription() const
{
    return m_propertyInfo->GetDescription();
}

PropertyType PropertyNode::GetType() const
{
    return m_propertyInfo->GetType();
}

PropertyInfo PropertyNode::GetInfo() const
{
    return m_propertyInfo;
}

bool PropertyNode::IsCategory() const
{
    return( m_propertyInfo->GetType() == PROPERTY_TYPE_CATEGORY );
}
//-----------------------------------------------------------------------------
// Children
//-----------------------------------------------------------------------------
void PropertyNode::AddChild( Property property )
{
    if( !property )
        return;

    m_children.push_back( property );

    PropertyInfo propertyInfo = property->GetInfo();

    for( size_t i = 0; i < propertyInfo->GetChildCount(); i++ )
    {
        PropertyInfo childInfo = propertyInfo->GetChild(i);
        Property child( new PropertyNode( childInfo ) );

        property->AddChild( child );
    }
}

Property PropertyNode::GetChild( size_t index  ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return Property();
}

Properties PropertyNode::GetChildren() const
{
    return m_children;
}

size_t PropertyNode::GetChildCount() const
{
    return m_children.size();
}
//-----------------------------------------------------------------------------
// Attributes
//-----------------------------------------------------------------------------
void PropertyNode::AddAttribute( const wxString &name, const wxString &value )
{
    Attribute attr = make_pair( name, value );

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

size_t PropertyNode::GetAttributeCount() const
{
    return m_attributes.size();
}
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
using namespace wxGD::Convert;

wxArrayString PropertyNode::GetAsArrayString() const
{
    return StringToArrayString( m_value );
}

bool PropertyNode::GetAsBool() const
{
    return StringToBool( m_value );
}

int PropertyNode::GetAsSystemColour() const
{
    // System or Default Colour
    wxColour colour = StringToColour( m_value );
    if( colour == wxNullColour )
        return StringToSystemColourIndex( m_value );

    // Custom Colour
    return 0xFFFFFF;
}

wxColour PropertyNode::GetAsColour() const
{
    // Default Colour
    if( m_value.empty() )
        return wxNullColour;

    // System Colour or Default Colour
    wxColour colour = StringToSystemColour( m_value );
    if( colour != wxNullColour )
        return colour;

    // Custom Colour
    return StringToColour( m_value );
}

double PropertyNode::GetAsDouble() const
{
    return StringToFloat( m_value );
}

wxFontContainer PropertyNode::GetAsFont() const
{
    return StringToFont( m_value );
}

int PropertyNode::GetAsInteger() const
{
    return StringToInteger( m_value );
}

wxPoint PropertyNode::GetAsPoint() const
{
    return StringToPoint( m_value );
}

wxSize PropertyNode::GetAsSize() const
{
    return StringToSize( m_value );
}

wxString PropertyNode::GetAsString() const
{
    return m_value;
}

int PropertyNode::GetAsStyle() const
{
    return StringToFlag( m_value );
}

wxString PropertyNode::GetAsText() const
{
    return GetAsString(); // TODO
}

wxString PropertyNode::GetAsURL() const
{
    return GetAsString(); // TODO
}
//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------
void PropertyNode::SetValue( const wxString &value )
{
    m_value = value;
}

void PropertyNode::SetValue( int value )
{
    m_value = IntegerToString(value);
}

void PropertyNode::SetValue( bool value )
{
    m_value = BoolToString(value);
}

void PropertyNode::SetValue( double value )
{
    m_value = FloatToString(value);
}

void PropertyNode::SetValue( const wxArrayString &value )
{
    m_value = ArrayStringToString(value);
}

void PropertyNode::SetValue( const wxColour &colour, int type )
{
    switch( type )
    {
        case 0:         // Default
        {
            m_value = wxEmptyString;
            break;
        }
        case 0xFFFFFF:  // Custom Colour
        {
            m_value = ColourToString( colour, type );
            break;
        }
        default:        // System Colour
        {
            m_value = SystemColourToString( type );
        }
    }
}

void PropertyNode::SetValue( const wxFontContainer &value )
{
    m_value = FontToString(value);
}

void PropertyNode::SetValue( const wxPoint &value )
{
    m_value = PointToString(value);
}

void PropertyNode::SetValue( const wxSize &value )
{
    m_value = SizeToString(value);
}
//=============================================================================
// ObjectNode Class
//=============================================================================
ObjectNode::ObjectNode( ClassInfo classInfo, Object parent,
                        bool expanded, bool isReference )
:
m_classInfo ( classInfo ),
m_parent    ( parent ),
m_expanded  ( expanded ),
m_isRef     ( isReference )
{
}

ObjectNode::~ObjectNode()
{
    m_properties.clear();
    m_events.clear();
    m_children.clear();
}

wxString ObjectNode::GetName() const
{
    Property prop = GetProperty("name");
    if( prop )
        return prop->GetAsString();

    return wxEmptyString;
}

wxString ObjectNode::GetDescription() const
{
    return m_classInfo->GetDescription();
}

bool ObjectNode::IsReference() const
{
    return m_isRef;
}

bool ObjectNode::IsRoot() const
{
    return( m_classInfo->GetType() == CLASS_TYPE_ROOT );
}

bool ObjectNode::IsTopLevel() const
{
    return( m_classInfo->IsTypeOf( CLASS_TYPE_TOPLEVEL ) );
}
//-----------------------------------------------------------------------------
// State into the tree
//-----------------------------------------------------------------------------
bool ObjectNode::IsExpanded() const
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
//-----------------------------------------------------------------------------
// ClassInfos
//-----------------------------------------------------------------------------
ClassInfo ObjectNode::GetClassInfo() const
{
    return m_classInfo;
}

wxString ObjectNode::GetClassName() const
{
    return m_classInfo->GetName();
}
//-----------------------------------------------------------------------------
// Events
//-----------------------------------------------------------------------------
void ObjectNode::AddEvent( Event event )
{
    if( !event )
        return;

    m_events.push_back( event );
}

Event ObjectNode::GetEvent( size_t index ) const
{
    if( index < m_events.size() )
        return m_events.at( index );

    return Event();
}

Event ObjectNode::GetEvent( const wxString &name ) const
{
    for( Events::const_iterator it = m_events.begin();
                                it != m_events.end(); ++it )
    {
        if( (*it)->GetName() == name )
            return *it;
    }

    return Event();
}

Events ObjectNode::GetEvents() const
{
    return m_events;
}

size_t ObjectNode::GetEventCount() const
{
    return m_events.size();
}
//-----------------------------------------------------------------------------
// Properties
//-----------------------------------------------------------------------------
void ObjectNode::AddProperty( Property property )
{
    if( !property )
        return;

    m_properties.push_back( property );

    PropertyInfo propertyInfo = property->GetInfo();

    for( size_t i = 0; i < propertyInfo->GetChildCount(); i++ )
    {
        PropertyInfo childInfo = propertyInfo->GetChild(i);
        wxString     value     = childInfo->GetDefaultValue();
        Property     child( new PropertyNode( childInfo ) );

        child->SetValue( value );
        property->AddChild( child );
    }
}

Property ObjectNode::GetProperty( size_t index ) const
{
    if( index < m_properties.size() )
        return m_properties.at( index );

    return Property();
}

Property ObjectNode::GetProperty( const wxString &name ) const
{
    for( Properties::const_iterator it = m_properties.begin();
                                    it != m_properties.end(); ++it )
    {
        if( (*it)->GetName() == name )
            return *it;

        Property prop = GetChildProperty( *it, name );
        if( prop && prop->GetName() == name )
            return prop;
    }

    return Property();
}

Property ObjectNode::GetChildProperty( Property parent,
                                       const wxString &name ) const
{
    if( parent )
    {
        for( size_t i = 0; i < parent->GetChildCount(); i++ )
        {
            Property prop = parent->GetChild(i);
            if( prop && prop->GetName() == name )
                return prop;
        }
    }

    return Property();
}

Properties ObjectNode::GetProperties() const
{
    return m_properties;
}

size_t ObjectNode::GetPropertyCount() const
{
    return m_properties.size();
}
//-----------------------------------------------------------------------------
// Parent / children objects
//-----------------------------------------------------------------------------
void ObjectNode::AddChild( Object child )
{
    m_children.push_back( child );
}

Object ObjectNode::GetChild( size_t index ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return Object();
}

Object ObjectNode::GetParent() const
{
    return m_parent;
}

Objects ObjectNode::GetChildren() const
{
    return m_children;
}

size_t ObjectNode::GetChildCount() const
{
    return m_children.size();
}
//=============================================================================
// ObjectTree
//=============================================================================
ObjectTree::ObjectTree()
{
    wxFlagsManager::Get();
    ClassInfo classInfo = ClassInfoDB::Get()->GetClassInfo("Project");

    m_root      = Object( new ObjectNode( classInfo, Object() ) );
    m_selected  = m_root;
}

ObjectTree::~ObjectTree()
{
    m_selected  =
    m_root      = Object();

    ClassInfoDB::Free();
    wxFlagsManager::Free();
}

Object ObjectTree::CreateObject( const wxString &className, Object parent )
{
//  - Get the requested object class information
//  - Count how many children are allowed for the current object
//  - and how many are already created

//  This function is not called for the special root object.

    ClassInfo classInfo = ClassInfoDB::Get()->GetClassInfo( className );
    if( !classInfo )
    {
        wxLogError(_("Unknown object type") + " " + className );
        return Object();
    }

    if( !parent )
        parent = m_selected;

    bool allow = false;

    // Try all parents
    while( parent )
    {
        allow = IsChildOk( parent, classInfo );

        if( allow )
        {
            m_selected = parent;
            break;
        }

        parent = parent->GetParent();
    }

    if( !allow )
        return Object();

    // Check item object
    if( classInfo->IsTypeOf( CLASS_TYPE_ITEM ) )
    {
        
    }

    // Create the object
    Object object( new ObjectNode( classInfo, m_selected ) );
    m_selected->AddChild( object );

    // Add properties
    for( size_t i = 0; i < classInfo->GetPropertyInfoCount(); i++ )
    {
        PropertyInfo propertyInfo = classInfo->GetPropertyInfo(i);
        Property property( new PropertyNode( propertyInfo ) );
        object->AddProperty( property );
    }

    // Add events
    for( size_t i = 0; i < classInfo->GetEventInfoCount(); i++ )
    {
        EventInfo eventInfo = classInfo->GetEventInfo(i);
        Event event( new EventNode( eventInfo ) );
        object->AddEvent( event );
    }

    return object;
}

size_t ObjectTree::GetSiblingsCount( Object parent, ClassInfo classInfo ) const
{
    size_t count = 0;

    // Enumerate all children at same depth
    for( size_t i = 0; i < parent->GetChildCount(); i++ )
    {
        Object    sibling     = parent->GetChild(i);
        ClassInfo siblingInfo = sibling->GetClassInfo();
        wxString  siblingName = siblingInfo->GetName();

        // Compare sibling class
        if( classInfo->IsA( siblingName ) )
        {
            count++;
        }
        else
        {
            // Compare sibling inherited classes
            for( size_t j = 0; j < siblingInfo->GetBaseCount(); j++ )
            {
                wxString siblingBaseName = siblingInfo->GetBaseName(j);

                if( classInfo->IsA( siblingBaseName ) )
                {
                    count++;
                    break;
                }
            }
        }
    }

    return count;
}

bool ObjectTree::IsChildOk( Object parentObject, ClassInfo classInfo ) const
{
    if( !parentObject || !classInfo )
        return false;

    ClassInfo parentInfo = parentObject->GetClassInfo();
    wxString  parentName = parentInfo->GetName();
    wxString  className  = classInfo->GetName();

    // Partial check at database level
    size_t childCount = GetSiblingsCount( parentObject, classInfo );
    bool isOk = parentInfo->IsChildInfoOk( className, childCount );

    if( isOk )
    {
        // Check for optional children, return false if any
        for( size_t i = 0; i < parentObject->GetChildCount(); i++ )
        {
            Object child = parentObject->GetChild(i);
            classInfo = child->GetClassInfo();

            for( size_t j = 0; j < parentInfo->GetChildInfoCount(); j++ )
            {
                ChildInfo childInfo = parentInfo->GetChildInfo(j);
                wxString  childName = childInfo->GetName();

                if( classInfo->IsA( childName ) &&
                    childInfo->IsOption() )
                {
                    wxLogError( "%s has already 1 %s as mutually exclusive child.",
                                parentName, childName );
                    return false;
                }
            }
        }
    }
    else
    {
        if( childCount > 0 )
            wxLogError( "%s allows only %i %s instance(s) as its child(ren)",
                        parentName, childCount, className );
        else
            wxLogError( "%s isn't an allowed child type for class %s",
                        className, parentName );
    }

    return isOk;
}

Object ObjectTree::GetRootObject() const
{
    return m_root;
}

Object ObjectTree::GetSelectedObject() const
{
    return m_selected;
}

Object ObjectTree::GetTopLevelObject( Object object ) const
{
    if( !object )
        return Object();

    Object parent = object;
    while( parent )
    {
        if( parent->IsTopLevel() )
            return parent;

        parent = parent->GetParent();
    }

    return parent;
}

void ObjectTree::SelectObject( Object object )
{
    wxASSERT( object != m_selected );

    m_selected = object;
}
