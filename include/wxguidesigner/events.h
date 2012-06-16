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

#include <wx/event.h>
#include "wxguidesigner/defs.h"

class wxGDEvent;
class wxGDPropertyEvent;
class wxGDObjectEvent;

wxDECLARE_EVENT( wxGD_EVT_EVENT_CHANGED,    wxGDEvent );
wxDECLARE_EVENT( wxGD_EVT_PROPERTY_CHANGED, wxGDPropertyEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_CREATED,   wxGDObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_DELETED,   wxGDObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_EXPANDED,  wxGDObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_OBJECT_SELECTED,  wxGDObjectEvent );
wxDECLARE_EVENT( wxGD_EVT_PROJECT_LOADED,   wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_PROJECT_SAVED,    wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_PROJECT_UPDATED,  wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_CODE_GENERATED,   wxCommandEvent );
//=============================================================================
// wxGDEvent
//=============================================================================
class wxGDEvent: public wxEvent
{
public:
    wxGDEvent( wxEventType type = wxEVT_NULL, int id = 0,
                    Event event = Event() );
    ~wxGDEvent();

    wxEvent *Clone() const;
    Event GetEvent() const;

private:
    Event m_event;
};
//=============================================================================
// wxGDPropertyEvent
//=============================================================================
class wxGDPropertyEvent: public wxEvent
{
public:
    wxGDPropertyEvent( wxEventType type = wxEVT_NULL, int id = 0,
                        Property property = Property() );
    ~wxGDPropertyEvent();

    wxEvent *Clone()        const;
    Property GetProperty()  const;

private:
    Property m_property;
};
//=============================================================================
// wxGDObjectEvent
//=============================================================================
class wxGDObjectEvent: public wxEvent
{
public:
    wxGDObjectEvent( wxEventType type = wxEVT_NULL, int id = 0,
                        Object object = Object() );
    ~wxGDObjectEvent();

    wxEvent *Clone()    const;
    Object  GetObject() const;

private:
    Object m_object;
};

#endif //__WXGUIDESIGNER_EVENTS_H__
