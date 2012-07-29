///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/events.h
// Purpose:     Custom events
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_EVENTS_H__
#define __WXGUIDESIGNER_EVENTS_H__

#include "wxguidesigner/rtti.h"

class wxEvent;

namespace wxGD
{
    namespace RTTI
    {
        class EventPropertyEvent;
        class PropertyEvent;
        class ObjectEvent;
    };
};

wxDECLARE_EVENT( wxGD_EVT_EVENT_CHANGED,    wxGD::RTTI::EventPropertyEvent );
wxDECLARE_EVENT( wxGD_EVT_PROPERTY_CHANGED, wxGD::RTTI::PropertyEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_CREATED,   wxGD::RTTI::ObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_DELETED,   wxGD::RTTI::ObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_EXPANDED,  wxGD::RTTI::ObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_SELECTED,  wxGD::RTTI::ObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_PROJECT_LOADED,   wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_PROJECT_SAVED,    wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_PROJECT_UPDATED,  wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_CODE_GENERATED,   wxCommandEvent );

namespace wxGD
{
namespace RTTI
{
//=============================================================================
// EventPropertyEvent
//=============================================================================
class EventPropertyEvent: public wxEvent
{
public:
    EventPropertyEvent( wxEventType     type  = wxEVT_NULL, int id = 0,
                        EventProperty   event = EventProperty() );
    ~EventPropertyEvent();

    wxEvent *Clone() const;
    EventProperty GetEvent() const;

private:
    EventProperty m_event;
};
//=============================================================================
// PropertyEvent
//=============================================================================
class PropertyEvent: public wxEvent
{
public:
    PropertyEvent ( wxEventType type = wxEVT_NULL, int id = 0,
                    wxGD::RTTI::Property property = Property() );
    ~PropertyEvent();

    wxEvent *Clone()        const;
    Property GetProperty()  const;

private:
    Property m_property;
};
//=============================================================================
// ObjectEvent
//=============================================================================
class ObjectEvent: public wxEvent
{
public:
    ObjectEvent   ( wxEventType type   = wxEVT_NULL, int id = 0,
                    Object      object = Object() );
    ~ObjectEvent();

    wxEvent *Clone()    const;
    Object  GetObject() const;

private:
    Object m_object;
};

}; // namespace RTTI
}; // namespace wxGD

#endif //__WXGUIDESIGNER_EVENTS_H__
