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
//=============================================================================
// EventNode Class
//=============================================================================
wxGD::RTTI::EventNode::EventNode( EventInfo eventInfo )
:
m_eventInfo( eventInfo )
{
    for( size_t i = 0; i < m_eventInfo->GetTypeCount(); i++ )
        m_handlers.push_back(wxEmptyString);
}

wxGD::RTTI::EventNode::~EventNode()
{
}

wxString wxGD::RTTI::EventNode::GetName() const
{
    return m_eventInfo->GetName();
}

wxString wxGD::RTTI::EventNode::GetDescription() const
{
    return m_eventInfo->GetDescription();
}

wxString wxGD::RTTI::EventNode::GetHandlerName( size_t typeIndex ) const
{
    if( typeIndex < m_handlers.size() )
        return m_handlers.at( typeIndex );

    return wxEmptyString;
}

wxString wxGD::RTTI::EventNode::GetTypeName( size_t typeIndex  ) const
{
    if( typeIndex < m_eventInfo->GetTypeCount() )
        return m_eventInfo->GetTypeName( typeIndex );

    return wxEmptyString;
}

wxString wxGD::RTTI::EventNode::GetTypeDescription( size_t typeIndex  ) const
{
    if( typeIndex < m_eventInfo->GetTypeCount() )
        return m_eventInfo->GetTypeDescription( typeIndex );

    return wxEmptyString;
}

size_t wxGD::RTTI::EventNode::GetTypeCount() const
{
    return m_eventInfo->GetTypeCount();
}

wxGD::RTTI::Handlers
wxGD::RTTI::EventNode::GetHandlers() const
{
    return m_handlers;
}

bool wxGD::RTTI::EventNode::HasHandlers()
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

void wxGD::RTTI::EventNode::SetHandlerName( size_t typeIndex,
                                            const wxString &funcName )
{
    if( typeIndex < m_eventInfo->GetTypeCount() )
    {
        Handlers::iterator it = m_handlers.begin() + typeIndex;
        m_handlers.erase( it );
        m_handlers.insert( it, funcName );
    }
}

void wxGD::RTTI::EventNode::SetHandlerName( const wxString &typeName,
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
wxGD::RTTI::PropertyNode::PropertyNode( wxGD::RTTI::PropertyInfo propertyInfo )
:
m_propertyInfo  ( propertyInfo ),
m_value         ( propertyInfo->GetDefaultValue() )
{
}

wxGD::RTTI::PropertyNode::~PropertyNode()
{
    m_children.clear();
}

wxString wxGD::RTTI::PropertyNode::GetName() const
{
    return m_propertyInfo->GetName();
}

wxString wxGD::RTTI::PropertyNode::GetLabel() const
{
    return m_propertyInfo->GetLabel();
}

wxString wxGD::RTTI::PropertyNode::GetDescription() const
{
    return m_propertyInfo->GetDescription();
}

wxGD::RTTI::PropertyType
wxGD::RTTI::PropertyNode::GetType() const
{
    return m_propertyInfo->GetType();
}

wxGD::RTTI::PropertyInfo
wxGD::RTTI::PropertyNode::GetInfo() const
{
    return m_propertyInfo;
}

bool wxGD::RTTI::PropertyNode::IsCategory() const
{
    return( m_propertyInfo->GetType() == PROPERTY_TYPE_CATEGORY );
}
//-----------------------------------------------------------------------------
// Children
//-----------------------------------------------------------------------------
void wxGD::RTTI::PropertyNode::AddChild( Property property )
{
    if( !property )
        return;

    m_children.push_back( property );

    PropertyInfo propertyInfo = property->GetInfo();

    for( size_t i = 0; i < propertyInfo->GetChildCount(); i++ )
    {
        PropertyInfo  childInfo = propertyInfo->GetChild(i);
        Property      child( new PropertyNode( childInfo ) );

        property->AddChild( child );
    }
}

wxGD::RTTI::Property
wxGD::RTTI::PropertyNode::GetChild( size_t index  ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return Property();
}

wxGD::RTTI::Properties
wxGD::RTTI::PropertyNode::GetChildren() const
{
    return m_children;
}

size_t wxGD::RTTI::PropertyNode::GetChildCount() const
{
    return m_children.size();
}
//-----------------------------------------------------------------------------
// Attributes
//-----------------------------------------------------------------------------
void wxGD::RTTI::PropertyNode::AddAttribute(const wxString &name,
                                            const wxString &value)
{
    Attribute attr( name, value );

    m_attributes.push_back( attr );
}

wxGD::RTTI::Attribute
wxGD::RTTI::PropertyNode::GetAttribute( size_t index ) const
{
    if( index < m_attributes.size() )
        return m_attributes.at( index );

    return Attribute();
}

wxString wxGD::RTTI::PropertyNode::GetAttributeName( size_t index )  const
{
    if( index < m_attributes.size() )
        return m_attributes.at( index ).first;

    return wxEmptyString;
}

wxString wxGD::RTTI::PropertyNode::GetAttributeValue( size_t index )  const
{
    if( index < m_attributes.size() )
        return m_attributes.at( index ).second;

    return wxEmptyString;
}

size_t wxGD::RTTI::PropertyNode::GetAttributeCount() const
{
    return m_attributes.size();
}
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
wxArrayString wxGD::RTTI::PropertyNode::GetAsArrayString() const
{
    return wxGD::Convert::StringToArrayString( m_value );
}

bool wxGD::RTTI::PropertyNode::GetAsBool() const
{
    return wxGD::Convert::StringToBool( m_value );
}

int wxGD::RTTI::PropertyNode::GetAsSystemColour() const
{
    // System or Default Colour
    wxColour colour = wxColour( m_value );
    if( colour == wxNullColour )
        return wxGD::Convert::StringToSystemColourIndex( m_value );

    // Custom Colour
    return 0xFFFFFF;
}

wxColour wxGD::RTTI::PropertyNode::GetAsColour() const
{
    // Default Colour
    if( m_value.empty() )
        return wxNullColour;

    // System Colour or Default Colour
    wxColour colour = wxGD::Convert::StringToSystemColour( m_value );
    if( colour != wxNullColour )
        return colour;

    // Custom Colour
    return wxColour( m_value );
}

double wxGD::RTTI::PropertyNode::GetAsDouble() const
{
    return wxGD::Convert::StringToFloat( m_value );
}

wxFontContainer wxGD::RTTI::PropertyNode::GetAsFont() const
{
    return wxGD::Convert::StringToFont( m_value );
}

int wxGD::RTTI::PropertyNode::GetAsInteger() const
{
    return wxGD::Convert::StringToInteger( m_value );
}

wxPoint wxGD::RTTI::PropertyNode::GetAsPoint() const
{
    return wxGD::Convert::StringToPoint( m_value );
}

wxSize wxGD::RTTI::PropertyNode::GetAsSize() const
{
    return wxGD::Convert::StringToSize( m_value );
}

wxString wxGD::RTTI::PropertyNode::GetAsString() const
{
    return m_value;
}

int wxGD::RTTI::PropertyNode::GetAsStyle() const
{
    return wxGD::Convert::StringToFlag( m_value );
}

wxString wxGD::RTTI::PropertyNode::GetAsText() const
{
    return GetAsString(); // TODO
}

wxString wxGD::RTTI::PropertyNode::GetAsURL() const
{
    return GetAsString(); // TODO
}
//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------
void wxGD::RTTI::PropertyNode::SetValue( const wxString &value )
{
    m_value = value;
}

void wxGD::RTTI::PropertyNode::SetValue( int value )
{
    m_value = wxGD::Convert::IntegerToString(value);
}

void wxGD::RTTI::PropertyNode::SetValue( bool value )
{
    m_value = wxGD::Convert::BoolToString(value);
}

void wxGD::RTTI::PropertyNode::SetValue( double value )
{
    m_value = wxGD::Convert::FloatToString(value);
}

void wxGD::RTTI::PropertyNode::SetValue( const wxArrayString &value )
{
    m_value = wxGD::Convert::ArrayStringToString(value);
}

void wxGD::RTTI::PropertyNode::SetValue( const wxColour &colour, int type )
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
            m_value = wxGD::Convert::ColourToString( colour, type );
            break;
        }
        default:        // System Colour
        {
            m_value = wxGD::Convert::SystemColourToString( type );
        }
    }
}

void wxGD::RTTI::PropertyNode::SetValue( const wxFontContainer &value )
{
    m_value = wxGD::Convert::FontToString(value);
}

void wxGD::RTTI::PropertyNode::SetValue( const wxPoint &value )
{
    m_value = wxGD::Convert::PointToString(value);
}

void wxGD::RTTI::PropertyNode::SetValue( const wxSize &value )
{
    m_value = wxGD::Convert::SizeToString(value);
}
//=============================================================================
// ObjectNode Class
//=============================================================================
wxGD::RTTI::ObjectNode::ObjectNode( ClassInfo classInfo, Object parent,
                                    bool expanded, bool isReference )
:
m_classInfo ( classInfo ),
m_parent    ( parent ),
m_expanded  ( expanded ),
m_isRef     ( isReference )
{
}

wxGD::RTTI::ObjectNode::~ObjectNode()
{
    m_properties.clear();
    m_events.clear();
    m_children.clear();
}

wxString wxGD::RTTI::ObjectNode::GetName() const
{
    Property property = GetProperty("name");
    if( property )
        return property->GetAsString();

    return wxEmptyString;
}

wxString wxGD::RTTI::ObjectNode::GetDescription() const
{
    return m_classInfo->GetDescription();
}

bool wxGD::RTTI::ObjectNode::IsReference() const
{
    return m_isRef;
}

bool wxGD::RTTI::ObjectNode::IsRoot() const
{
    return( m_classInfo->GetType() == CLASS_TYPE_ROOT );
}

bool wxGD::RTTI::ObjectNode::IsTopLevel() const
{
    return( m_classInfo->IsTypeOf( CLASS_TYPE_TOPLEVEL ) );
}
//-----------------------------------------------------------------------------
// State into the tree
//-----------------------------------------------------------------------------
bool wxGD::RTTI::ObjectNode::IsExpanded() const
{
    return m_expanded;
}

void wxGD::RTTI::ObjectNode::Collapse()
{
    m_expanded = false;
}

void wxGD::RTTI::ObjectNode::Expand()
{
    m_expanded = true;
}
//-----------------------------------------------------------------------------
// ClassInfos
//-----------------------------------------------------------------------------
wxGD::RTTI::ClassInfo
wxGD::RTTI::ObjectNode::GetClassInfo() const
{
    return m_classInfo;
}

wxString wxGD::RTTI::ObjectNode::GetClassName() const
{
    return m_classInfo->GetName();
}
//-----------------------------------------------------------------------------
// Events
//-----------------------------------------------------------------------------
void wxGD::RTTI::ObjectNode::AddEvent( EventProperty event )
{
    if( !event )
        return;

    m_events.push_back( event );
}

wxGD::RTTI::EventProperty
wxGD::RTTI::ObjectNode::GetEvent( size_t index ) const
{
    if( index < m_events.size() )
        return m_events.at( index );

    return EventProperty();
}

wxGD::RTTI::EventProperty
wxGD::RTTI::ObjectNode::GetEvent( const wxString &name ) const
{
    for( EventProperties::const_iterator it = m_events.begin();
                                        it != m_events.end(); ++it )
    {
        if( (*it)->GetName() == name )
            return *it;
    }

    return EventProperty();
}

wxGD::RTTI::EventProperties
wxGD::RTTI::ObjectNode::GetEvents() const
{
    return m_events;
}

size_t wxGD::RTTI::ObjectNode::GetEventCount() const
{
    return m_events.size();
}
//-----------------------------------------------------------------------------
// Properties
//-----------------------------------------------------------------------------
void wxGD::RTTI::ObjectNode::AddProperty( Property property )
{
    if( !property )
        return;

    m_properties.push_back( property );

    PropertyInfo propertyInfo = property->GetInfo();

    for( size_t i = 0; i < propertyInfo->GetChildCount(); i++ )
    {
        PropertyInfo  childInfo = propertyInfo->GetChild(i);
        wxString      value     = childInfo->GetDefaultValue();
        Property      child( new PropertyNode( childInfo ) );

        child->SetValue( value );
        property->AddChild( child );
    }
}

wxGD::RTTI::Property
wxGD::RTTI::ObjectNode::GetProperty( size_t index ) const
{
    if( index < m_properties.size() )
        return m_properties.at( index );

    return Property();
}

wxGD::RTTI::Property
wxGD::RTTI::ObjectNode::GetProperty( const wxString &name ) const
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

wxGD::RTTI::Property
wxGD::RTTI::ObjectNode::GetChildProperty( Property parent,
                                        const wxString &name ) const
{
    if( parent )
    {
        for( size_t i = 0; i < parent->GetChildCount(); i++ )
        {
            Property property = parent->GetChild(i);
            if( property && property->GetName() == name )
                return property;
        }
    }

    return Property();
}

wxGD::RTTI::Properties
wxGD::RTTI::ObjectNode::GetProperties() const
{
    return m_properties;
}

size_t wxGD::RTTI::ObjectNode::GetPropertyCount() const
{
    return m_properties.size();
}
//-----------------------------------------------------------------------------
// Parent / children objects
//-----------------------------------------------------------------------------
void wxGD::RTTI::ObjectNode::AddChild( Object child )
{
    m_children.push_back( child );
}

wxGD::RTTI::Object
wxGD::RTTI::ObjectNode::GetChild( size_t index ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return Object();
}

wxGD::RTTI::Object
wxGD::RTTI::ObjectNode::GetParent() const
{
    return m_parent;
}

wxGD::RTTI::Object
wxGD::RTTI::ObjectNode::GetTopLevelParent()
{
    Object parent = shared_from_this();
    while( parent )
    {
        if( parent->IsTopLevel() )
            return parent;

        parent = parent->GetParent();
    }

    return Object();
}

wxGD::RTTI::Objects
wxGD::RTTI::ObjectNode::GetChildren() const
{
    return m_children;
}

size_t wxGD::RTTI::ObjectNode::GetChildCount() const
{
    return m_children.size();
}
//=============================================================================
// ObjectTree
//=============================================================================
wxGD::RTTI::ObjectTree::ObjectTree()
{
    wxGD::Flags::Get();
    ClassInfo classInfo = ClassTree::Get()->GetClassInfo("Project");

    m_root      = Object( new ObjectNode( classInfo, Object() ) );
    m_selected  = m_root;
}

wxGD::RTTI::ObjectTree::~ObjectTree()
{
    m_selected  =
    m_root      = Object();

    ClassTree::Free();
    wxGD::Flags::Free();
}

wxGD::RTTI::Object
wxGD::RTTI::ObjectTree::CreateObject  ( const wxString &className,
                                        Object parent )
{
//  - Get the requested object class information
//  - Count how many children are allowed for the current object
//  - and how many are already created

//  This function is not called for the special root object.

    ClassInfo classInfo = ClassTree::Get()->GetClassInfo( className );
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
        PropertyInfo  propertyInfo = classInfo->GetPropertyInfo(i);
        Property      property( new PropertyNode( propertyInfo ) );
        object->AddProperty( property );
    }

    // Add events
    for( size_t i = 0; i < classInfo->GetEventInfoCount(); i++ )
    {
        EventInfo     eventInfo = classInfo->GetEventInfo(i);
        EventProperty event( new EventNode( eventInfo ) );
        object->AddEvent( event );
    }

    return object;
}

size_t wxGD::RTTI::ObjectTree::GetSiblingsCount(Object      parent,
                                                ClassInfo   classInfo) const
{
    size_t count = 0;

    // Enumerate all children at same depth
    for( size_t i = 0; i < parent->GetChildCount(); i++ )
    {
        Object      sibling     = parent->GetChild(i);
        ClassInfo   siblingInfo = sibling->GetClassInfo();
        wxString    siblingName = siblingInfo->GetName();

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

bool wxGD::RTTI::ObjectTree::IsChildOk( Object      parentObject,
                                        ClassInfo   classInfo ) const
{
    if( !parentObject || !classInfo )
        return false;

    ClassInfo   parentInfo = parentObject->GetClassInfo();
    wxString    parentName = parentInfo->GetName();
    wxString    className  = classInfo->GetName();

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
                ChildInfo   childInfo = parentInfo->GetChildInfo(j);
                wxString    childName = childInfo->GetName();

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

wxGD::RTTI::Object
wxGD::RTTI::ObjectTree::GetRootObject() const
{
    return m_root;
}

wxGD::RTTI::Object
wxGD::RTTI::ObjectTree::GetSelectedObject() const
{
    return m_selected;
}

void wxGD::RTTI::ObjectTree::SelectObject( Object object )
{
    wxASSERT( object != m_selected );

    m_selected = object;
}
