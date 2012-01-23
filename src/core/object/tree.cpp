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
// WidgetNode Class
//-----------------------------------------------------------------------------

WidgetNode::~WidgetNode()
{
    m_props.clear();
    m_events.clear();
    m_children.clear();
}

wxString WidgetNode::GetName() const
{
    // return m_info->GetName() + i;
    return wxEmptyString;
}
//-----------------------------------------------------------------------------
// WidgetTree Singleton Class
//-----------------------------------------------------------------------------

WidgetTree::~WidgetTree()
{
    for ( std::list< IWidgetHandler * >::iterator it = m_handlers.begin();
                                                it != m_handlers.end(); ++it )
    {
        m_handlers.erase( it );
    }
}

WidgetTree *WidgetTree::ms_instance = NULL;

WidgetTree *WidgetTree::Get()
{
    if( !ms_instance )
        ms_instance = new WidgetTree;

    return ms_instance;
}

void WidgetTree::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

bool WidgetTree::CreateObject( const wxString &className )
{
    WidgetInfo clsInfo( WidgetInfoDB::Get()->GetClassInfo( className ) );

    if ( !clsInfo->CanBeChildOf( m_sel->GetClassName() ) )
    {
        wxLogError
        (
            "Selected object '%s' is not a valid parent for class '%s'",
            m_sel->GetClassName(), className
        );

        return false;
    }

    Widget widget( new WidgetNode( m_sel, clsInfo ) );
    SendEvent( widget, EVT_WIDGET_CREATED );

    m_sel->AddChild( widget );

    m_sel = widget;

    return true;
}

void WidgetTree::SelectObject( Widget widget )
{
    m_sel = widget;
    SendEvent( widget, EVT_WIDGET_SELECTED );
}

void WidgetTree::AddHandler( IWidgetHandler *handler )
{
    m_handlers.push_back( handler );
}

void WidgetTree::RemoveHandler( IWidgetHandler *handler )
{
    m_handlers.remove( handler );
}

void WidgetTree::SendEvent( Widget widget, WidgetEventType eventType )
{
    for ( std::list< IWidgetHandler * >::iterator it  = m_handlers.begin();
                                                  it != m_handlers.end(); ++it )
    {
        switch ( eventType )
        {
        case EVT_WIDGET_CREATED:
            (*it)->OnObjectCreated( widget );
            break;

        case EVT_WIDGET_DELETED:
            (*it)->OnObjectDeleted( widget );
            break;

        case EVT_WIDGET_EXPANDED:
            (*it)->OnObjectExpanded( widget );
            break;

        case EVT_WIDGET_SELECTED:
            (*it)->OnObjectSelected( widget );
            break;
        }
    }
}

/*-----------------------------------------------------------------------------
// WidgetEvent
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS( WidgetEvent, wxEvent );

wxDEFINE_EVENT( wxEVT_WIDGET_CREATE,    WidgetEvent );
wxDEFINE_EVENT( wxEVT_WIDGET_CREATED,   WidgetEvent );
wxDEFINE_EVENT( wxEVT_WIDGET_SELECTED,  WidgetEvent );

WidgetEvent::WidgetEvent( wxEventType type, int id ) : wxEvent( id, type )
{
    
}

WidgetEvent::WidgetEvent( const WidgetEvent &event ) : wxEvent( event )
{
    
}

WidgetEvent::~WidgetEvent()
{
    
}*/
