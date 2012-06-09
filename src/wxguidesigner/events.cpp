///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/events.cpp
// Purpose:     Custom events
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/20
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/events.h"

wxDEFINE_EVENT( wxGD_EVT_EVENT_CHANGED,    wxGDEvent );
wxDEFINE_EVENT( wxGD_EVT_PROPERTY_CHANGED, wxGDPropertyEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_CREATED,   wxGDObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_DELETED,   wxGDObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_EXPANDED,  wxGDObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_SELECTED,  wxGDObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_PROJECT_LOADED,   wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_PROJECT_SAVED,    wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_PROJECT_UPDATED,  wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_CODE_GENERATED,   wxCommandEvent );
//=============================================================================
// wxGDEvent
//=============================================================================
wxGDEvent::wxGDEvent( wxEventType type, int id, Event event )
:
wxEvent( id, type ),
m_event( event )
{
}

wxGDEvent::~wxGDEvent()
{
}

wxEvent *wxGDEvent::Clone() const
{
    return new wxGDEvent( *this );
}

Event wxGDEvent::GetEvent() const
{
    return m_event;
}
//=============================================================================
// wxGDPropertyEvent
//=============================================================================
wxGDPropertyEvent::wxGDPropertyEvent( wxEventType type, int id,
                                        Property property )
:
wxEvent( id, type ),
m_property( property )
{
}

wxGDPropertyEvent::~wxGDPropertyEvent()
{
}

wxEvent *wxGDPropertyEvent::Clone() const
{
    return new wxGDPropertyEvent( *this );
}

Property wxGDPropertyEvent::GetProperty() const
{
    return m_property;
}
//=============================================================================
// wxGDObjectEvent
//=============================================================================
wxGDObjectEvent::wxGDObjectEvent( wxEventType type, int id, Object object )
:
wxEvent( id, type ),
m_object( object )
{
}

wxGDObjectEvent::~wxGDObjectEvent()
{
}

wxEvent *wxGDObjectEvent::Clone() const
{
    return new wxGDObjectEvent( *this );
}

Object wxGDObjectEvent::GetObject() const
{
    return m_object;
}
