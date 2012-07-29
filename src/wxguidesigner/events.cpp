///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/events.cpp
// Purpose:     Custom events
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/event.h>

#include "wxguidesigner/events.h"

wxDEFINE_EVENT( wxGD_EVT_EVENT_CHANGED,    wxGD::RTTI::EventPropertyEvent );
wxDEFINE_EVENT( wxGD_EVT_PROPERTY_CHANGED, wxGD::RTTI::PropertyEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_CREATED,   wxGD::RTTI::ObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_DELETED,   wxGD::RTTI::ObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_EXPANDED,  wxGD::RTTI::ObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_OBJECT_SELECTED,  wxGD::RTTI::ObjectEvent );
wxDEFINE_EVENT( wxGD_EVT_PROJECT_LOADED,   wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_PROJECT_SAVED,    wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_PROJECT_UPDATED,  wxCommandEvent );
wxDEFINE_EVENT( wxGD_EVT_CODE_GENERATED,   wxCommandEvent );
//=============================================================================
// EventPropertyEvent
//=============================================================================
wxGD::RTTI::EventPropertyEvent::EventPropertyEvent( wxEventType type, int id,
                                                    EventProperty event )
:
wxEvent( id, type ),
m_event( event )
{
}

wxGD::RTTI::EventPropertyEvent::~EventPropertyEvent()
{
}

wxEvent *wxGD::RTTI::EventPropertyEvent::Clone() const
{
    return new EventPropertyEvent( *this );
}

wxGD::RTTI::EventProperty
wxGD::RTTI::EventPropertyEvent::GetEvent() const
{
    return m_event;
}
//=============================================================================
// PropertyEvent
//=============================================================================
wxGD::RTTI::PropertyEvent::PropertyEvent( wxEventType type, int id,
                                            Property property )
:
wxEvent( id, type ),
m_property( property )
{
}

wxGD::RTTI::PropertyEvent::~PropertyEvent()
{
}

wxEvent *wxGD::RTTI::PropertyEvent::Clone() const
{
    return new PropertyEvent( *this );
}

wxGD::RTTI::Property
wxGD::RTTI::PropertyEvent::GetProperty() const
{
    return m_property;
}
//=============================================================================
// ObjectEvent
//=============================================================================
wxGD::RTTI::ObjectEvent::ObjectEvent( wxEventType type, int id, Object object )
:
wxEvent( id, type ),
m_object( object )
{
}

wxGD::RTTI::ObjectEvent::~ObjectEvent()
{
}

wxEvent *wxGD::RTTI::ObjectEvent::Clone() const
{
    return new ObjectEvent( *this );
}

wxGD::RTTI::Object
wxGD::RTTI::ObjectEvent::GetObject() const
{
    return m_object;
}
