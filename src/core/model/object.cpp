///////////////////////////////////////////////////////////////////////////////
// Name:        model/object.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "model/object.h"

wxIMPLEMENT_DYNAMIC_CLASS( ObjectEvent, wxEvent );

wxDEFINE_EVENT( wxEVT_OBJECT_CREATE,    ObjectEvent );
wxDEFINE_EVENT( wxEVT_OBJECT_CREATED,   ObjectEvent );
wxDEFINE_EVENT( wxEVT_OBJECT_SELECTED,  ObjectEvent );

ObjectEvent::ObjectEvent( wxEventType type, int id )
                : wxEvent( id, type )
{

}

ObjectEvent::ObjectEvent( const ObjectEvent &event )
            : wxEvent( event )
{
    
}

ObjectEvent::~ObjectEvent()
{
    
}
