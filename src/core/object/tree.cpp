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

#include <wx/bitmap.h>
#include <wx/gdicmn.h>
#include <wx/event.h>
#include <wx/font.h>
#include <wx/tokenzr.h>

#include <wx/log.h>
//*****************************************************************************
// EventNode Class
//*****************************************************************************

EventNode::EventNode( EventInfo eventInfo ) : m_info( eventInfo )
{
    m_handlers.Alloc( m_info->GetTypeCount() );

    for ( size_t i = 0; i < m_info->GetTypeCount(); i++ )
        m_handlers.Add( wxEmptyString );
}

EventNode::~EventNode()
{
}

wxString EventNode::GetHandlerName( size_t index ) const
{
    if ( index < m_handlers.GetCount() )
        return m_handlers.Item( index );

    return wxEmptyString;
}

bool EventNode::SetHandlerName( size_t index, const wxString &name )
{
    if ( index < m_handlers.GetCount() )
    {
        m_handlers[ index ] = name;
        return true;
    }

    return false;
}

bool EventNode::SetHandlerName( const wxString &type, const wxString &name )
{
    for ( size_t i = 0; i < m_info->GetTypeCount(); i++ )
    {
        if ( m_info->GetTypeName( i ) == type )
        {
            m_handlers[ i ] = name;
            return true;
        }
    }

    return false;
}

wxString EventNode::GetTypeName( size_t index  ) const
{
    if ( index < m_info->GetTypeCount() )
        return m_info->GetTypeName( index );

    return wxEmptyString;
}

wxString EventNode::GetTypeDescription( size_t index  ) const
{
    if ( index < m_info->GetTypeCount() )
        return m_info->GetTypeDescription( index );

    return wxEmptyString;
}
//*****************************************************************************
// PropertyNode Class
//*****************************************************************************

PropertyNode::~PropertyNode()
{
    if ( !m_value.IsNull() )
        m_value.MakeNull();

    m_children.clear();
}

void PropertyNode::AddChild( Property prop )
{
    m_children.push_back( prop );
}

wxArrayString PropertyNode::GetAsArrayString() const
{
    if ( m_value.CheckType< wxArrayString >() )
        return m_value.As< wxArrayString >();

    return wxArrayString();
}

wxBitmap PropertyNode::GetAsBitmap() const
{
    if ( m_value.CheckType< wxBitmap >() )
        return m_value.As< wxBitmap >();

    return wxNullBitmap;
}

bool PropertyNode::GetAsBool() const
{
    if ( m_value.CheckType< bool >() )
        return m_value.As< bool >();

    return false;
}

Colour PropertyNode::GetAsColour() const
{
    if ( m_value.CheckType< wxString >() )
    {
        wxString strCol = m_value.As< wxString >();
        if ( strCol.StartsWith("wxSYS_COLOUR_") )
        {
            int sysColIdx = wxGDConv::GetSystemColourIndex( strCol );
            if ( sysColIdx != wxNOT_FOUND )
            {
                Colour sysCol = { wxGDConv::GetSystemColourIndex( strCol ) };
                return sysCol;
            }
        }
    }
    else if ( m_value.CheckType< Colour >() )
    {
        return m_value.As< Colour >();
    }

    return Colour();
}

double PropertyNode::GetAsDouble() const
{
    if ( m_value.CheckType< double >() )
        return m_value.As< double >();

    return 0.0;
}

wxFont PropertyNode::GetAsFont() const
{
    if ( m_value.CheckType< wxString >() )
    {
        wxString strFont = m_value.As< wxString >();
        if ( strFont.StartsWith("wxSYS_") && strFont.EndsWith("_FONT") )
        {
            return wxGDConv::GetSystemFont( strFont );
        }
    }
    else if ( m_value.CheckType< wxFont >() )
    {
        return m_value.As< wxFont >();
    }
    return wxNullFont;
}

int PropertyNode::GetAsInt() const
{
    if ( m_value.CheckType< int >() )
        return m_value.As< int >();

    return 0;
}

wxPoint PropertyNode::GetAsPoint() const
{
    if ( m_value.CheckType< wxPoint >() )
        return m_value.As< wxPoint >();

    return wxDefaultPosition;
}

wxSize PropertyNode::GetAsSize() const
{
    if ( m_value.CheckType< wxSize >() )
        return m_value.As< wxSize >();

    return wxDefaultSize;
}

wxString PropertyNode::GetAsString() const
{
    if ( m_value.CheckType< wxString >() )
        return m_value.As< wxString >();

    return wxEmptyString;
}

int PropertyNode::GetAsStyle() const
{
    if ( m_value.CheckType< int >() )
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
    if ( index < m_children.size() )
        return m_children.at( index );

    return Property();
}
//*****************************************************************************
// ObjectNode Class
//*****************************************************************************

ObjectNode::ObjectNode( Object parent, ClassInfo classInfo, bool expanded )
            : m_parent( parent ), m_info( classInfo ), m_expanded( expanded )
{
}

ObjectNode::~ObjectNode()
{
    m_props.clear();
    m_events.clear();
    m_children.clear();
}

Object ObjectNode::GetChild( size_t index )
{
    if ( index < m_children.size() )
        return m_children.at( index );

    return Object();
}
//-----------------------------------------------------------------------------
// Events
//-----------------------------------------------------------------------------

Event ObjectNode::GetEvent( size_t index )
{
    if ( index < m_events.size() )
        return m_events.at( index );

    return Event();
}

Event ObjectNode::GetEvent( const wxString &name )
{
    for ( Events::iterator it = m_events.begin();
                                it != m_events.end(); ++it )
    {
        if ( (*it)->GetName() == name )
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
    if ( prop.get() )
    {
        m_props.push_back( prop );

        for ( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
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
    if ( index < m_props.size() )
        return m_props.at( index );

    return Property();
}

Property ObjectNode::GetChildProperty( Property parent, const wxString &name )
{
    if ( parent.get() )
    {
        for ( size_t i = 0; i < parent->GetChildCount(); i++ )
        {
            Property prop = parent->GetChild( i );
            if ( prop.get() && prop->GetName() == name )
                return prop;
        }
    }
    return Property();
}

Property ObjectNode::GetProperty( const wxString &name )
{
    for ( Properties::iterator it = m_props.begin(); it != m_props.end(); ++it )
    {
        if ( (*it)->GetName() == name )
            return *it;

        Property prop = GetChildProperty( *it, name );
        if ( prop.get() && prop->GetName() == name )
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

wxString ObjectNode::GetBaseName( size_t index )
{
    if ( m_info.get() )
        return m_info->GetBaseName( index );

    return wxEmptyString;
}

ClassInfo ObjectNode::GetBaseInfo( size_t index )
{
    return ClassInfoDB::Get()->GetClassInfo( GetBaseName( index ) );
}
//*****************************************************************************
// ObjectTree Singleton Class
//*****************************************************************************

ObjectTree::ObjectTree()
:
m_sel
(
    new ObjectNode( Object(),
                    ClassInfo( new ClassNode( "Project", CLASS_TYPE_ROOT ) ) )
)
{
}

ObjectTree::~ObjectTree()
{
    m_handlers.clear();
}

ObjectTree *ObjectTree::ms_instance = NULL;

ObjectTree *ObjectTree::Get()
{
    if( !ms_instance )
    {
        ms_instance = new ObjectTree;
    }

    return ms_instance;
}

void ObjectTree::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

size_t ObjectTree::GetChildCountByType( Object object, ClassType clsType )
{
    size_t count = 0;

    for ( size_t i = 0; i < object->GetChildCount(); i++ )
    {
        if ( clsType == object->GetChild( i )->GetClassInfo()->GetType() )
            count++;
    }

    return count;
}

bool ObjectTree::CreateObject( const wxString &className )
{
//  - Get the requested object class information
//  - Count how many children are allowed for the current object
//  - and how many are already created

//  This function is not called for the special root object.

    ClassInfo   clsInfo = ClassInfoDB::Get()->GetClassInfo( className );
    int         count   = GetChildCountByType( m_sel, clsInfo->GetType() );
    int         max     = clsInfo->GetMaxAllowedBy( m_sel->GetClassName() );
    bool        denied  = true;

    if ( max != 0 ) // Allowed child type of the selected object
    {
        denied = false;

        if ( (max > 0) && (count >= max) )
            denied = true;
    }
    else
    {
        // Try parent
        Object parent = m_sel->GetParent();
        while ( parent.get() )
        {
            int max   = clsInfo->GetMaxAllowedBy( parent->GetClassName() );
            int count = GetChildCountByType( parent, clsInfo->GetType() );

            if ( max && max < count )
            {
                denied = false;
                m_sel = parent;
                break;
            }

            parent = parent->GetParent();
        }
    }

    if ( denied )
    {
        if ( max != 0 )
        {
            wxLogError
            (
                "%s allows only %i %s instances as its child(ren)",
                m_sel->GetClassName(), max, className
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

        return false;
    }

    // Create the object
    Object object( new ObjectNode( m_sel, clsInfo ) );
    m_sel->AddChild( object );

    // Add properties
    for ( size_t i = 0; i < clsInfo->GetPropertyInfoCount(); i++ )
    {
        Property prop( new PropertyNode( clsInfo->GetPropertyInfo( i ) ) );
        object->AddProperty( prop );
    }

    // Add events
    for ( size_t i = 0; i < clsInfo->GetEventInfoCount(); i++ )
    {
        Event evt( new EventNode( clsInfo->GetEventInfo( i ) ) );
        object->AddEvent( evt );
    }

    // Add inherited class informations
    for ( size_t i = 0; i < clsInfo->GetBaseCount(); i++ )
    {
        wxString     baseName = clsInfo->GetBaseName( i );
        ClassInfo    baseInfo = ClassInfoDB::Get()->GetClassInfo( baseName );

        if ( baseInfo.get() )
        {
            PropertyInfo catInfo( new PropertyInfoNode
            ( PROPERTY_TYPE_CATEGORY, baseName, baseName ) );

            Property category( new PropertyNode( catInfo ) );
            object->AddProperty( category );

            for ( size_t n = 0; n < baseInfo->GetPropertyInfoCount(); n++ )
            {
                // Is it possible that the inherit class have a property
                // with same name, skip it (overrided method)
                PropertyInfo info = baseInfo->GetPropertyInfo( n );
                if ( !object->PropertyExists( info->GetName() ) )
                {
                    Property prop( new PropertyNode( info ) );
                    category->AddChild( prop );
                }
            }

            for ( size_t n = 0; n < baseInfo->GetEventInfoCount(); n++ )
            {
                Event evt( new EventNode( baseInfo->GetEventInfo( n ) ) );
                object->AddEvent( evt );
            }
        }
    }

    SendEvent( object, EVT_OBJECT_CREATED );

    return true;
}

void ObjectTree::SelectObject( Object object, bool withEvent )
{
    wxASSERT( object != m_sel );

    m_sel = object;

    // Forward the event to other handlers if needed.
    if ( withEvent )
        SendEvent( object, EVT_OBJECT_SELECTED );
}

void ObjectTree::AddHandler( IObjectHandler *handler )
{
    m_handlers.push_back( handler );
}

void ObjectTree::RemoveHandler( IObjectHandler *handler )
{
    m_handlers.remove( handler );
}

void ObjectTree::SendEvent( Object object, ObjectEventType eventType )
{
    for ( ObjectHandlers::iterator it  = m_handlers.begin();
                                   it != m_handlers.end(); ++it )
    {
        switch ( eventType )
        {
        case EVT_OBJECT_CREATED:
            (*it)->OnObjectCreated( object );
            break;

        case EVT_OBJECT_DELETED:
            (*it)->OnObjectDeleted( object );
            break;

        case EVT_OBJECT_EXPANDED:
            (*it)->OnObjectExpanded( object );
            break;

        case EVT_OBJECT_SELECTED:
            (*it)->OnObjectSelected( object );
            break;
        }
    }
}
