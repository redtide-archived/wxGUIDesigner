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

#include <wx/bitmap.h>
#include <wx/gdicmn.h>
#include <wx/event.h>
#include <wx/font.h>

#include <wx/log.h>
//-----------------------------------------------------------------------------
// PropertyBase Class
//-----------------------------------------------------------------------------

PropertyBase::~PropertyBase()
{
    if ( !m_value.IsNull() )
        m_value.MakeNull();
}

wxBitmap PropertyBase::GetAsBitmap() const
{
    if ( m_value.CheckType< wxBitmap >() )
        return m_value.As< wxBitmap >();

    return wxNullBitmap;
}

bool PropertyBase::GetAsBool() const
{
    if ( m_value.CheckType< bool >() )
        return m_value.As< bool >();

    return false;
}

wxColour PropertyBase::GetAsColour() const
{
    if ( m_value.CheckType< wxColour >() )
        return m_value.As< wxColour >();

    return wxColour( *wxBLACK );
}

float PropertyBase::GetAsFloat() const
{
    if ( m_value.CheckType< float >() )
        return m_value.As< float >();

    return 0;
}

wxFont PropertyBase::GetAsFont() const
{
    if ( m_value.CheckType< wxFont >() )
        return m_value.As< wxFont >();

    return wxNullFont;
}

int PropertyBase::GetAsInt() const
{
    if ( m_value.CheckType< int >() )
        return m_value.As< int >();

    return 0;
}

wxPoint PropertyBase::GetAsPoint() const
{
    if ( m_value.CheckType< wxPoint >() )
        return m_value.As< wxPoint >();

    return wxDefaultPosition;
}

wxSize PropertyBase::GetAsSize() const
{
    if ( m_value.CheckType< wxSize >() )
        return m_value.As< wxSize >();

    return wxDefaultSize;
}

wxString PropertyBase::GetAsString() const
{
    if ( m_value.CheckType< wxString >() )
        return m_value.As< wxString >();

    return wxEmptyString;
}

long PropertyBase::GetAsStyle() const
{
    return GetAsInt();
}

wxString PropertyBase::GetAsText() const
{
    return GetAsString(); // TODO
}

wxString PropertyBase::GetAsURL() const
{
    return GetAsString(); // TODO
}
//-----------------------------------------------------------------------------
// ObjectBase Class
//-----------------------------------------------------------------------------

ObjectBase::~ObjectBase()
{
    m_props.clear();
    m_children.clear();
}
/*
bool ObjectBase::Destroy()
{
    return false;
}
*/
//-----------------------------------------------------------------------------
// ObjectTree Singleton Class
//-----------------------------------------------------------------------------

ObjectTree::~ObjectTree()
{
    for ( std::list< IObjectHandler * >::iterator
            it = m_handlers.begin(); it != m_handlers.end(); ++it )
    {
        m_handlers.erase( it );
    }
}

ObjectTree *ObjectTree::ms_instance = NULL;

ObjectTree *ObjectTree::Get()
{
    if( !ms_instance )
        ms_instance = new ObjectTree;

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

bool ObjectTree::CreateObject( const wxString &className,
                               const wxString &category )
{
    if( !HaveRoot() || !HaveSelection() )
    {
        wxLogError("No root object");
        return false;
    }

    ClassInfo clsInfo
    (
        ObjectDBManager::Get()->GetClassInfo( className, category )
    );

    if ( clsInfo->GetBaseNames().Index( m_sel->GetClassName() ) == wxNOT_FOUND )
    {
        wxLogError
        (
            "No parent object '%s' found in class '%s'",
            m_sel->GetClassName(), className
        );

        return false;
    }

    Object obj( new ObjectBase( clsInfo, m_sel ) );
    m_sel->AddChild( obj );

    return true;
}

void ObjectTree::AddHandler( IObjectHandler *handler )
{
    m_handlers.push_back( handler );
}

void ObjectTree::RemoveHandler( IObjectHandler *handler )
{
    m_handlers.remove( handler );
}

void ObjectTree::SetRoot( Object object )
{
    m_root.reset( object.get() );
}

/*-----------------------------------------------------------------------------
// ObjectEvent
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS( ObjectEvent, wxEvent );

wxDEFINE_EVENT( wxEVT_OBJECT_CREATE,    ObjectEvent );
wxDEFINE_EVENT( wxEVT_OBJECT_CREATED,   ObjectEvent );
wxDEFINE_EVENT( wxEVT_OBJECT_SELECTED,  ObjectEvent );

ObjectEvent::ObjectEvent( wxEventType type, int id ) : wxEvent( id, type )
{
    
}

ObjectEvent::ObjectEvent( const ObjectEvent &event ) : wxEvent( event )
{
    
}

ObjectEvent::~ObjectEvent()
{
    
}*/
