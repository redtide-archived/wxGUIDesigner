///////////////////////////////////////////////////////////////////////////////
// Name:        model/object.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGD_OBJECT_H__
#define __WXGD_OBJECT_H__

#include <wx/event.h>

#include "defs.h"

class DLLIMPEXP_CORE ObjectEvent : public wxEvent
{
public:
    ObjectEvent( wxEventType type = wxEVT_NULL, int id = 0 );

#ifndef SWIG
    ObjectEvent( const ObjectEvent &event );
#endif
    ~ObjectEvent();

    virtual wxEvent *Clone() const { return new ObjectEvent( *this ); }

private:
#ifndef SWIG
    wxDECLARE_DYNAMIC_CLASS( ObjectEvent );
#endif
};

#ifndef SWIG
    wxDECLARE_EXPORTED_EVENT( DLLIMPEXP_CORE, wxEVT_OBJECT_CREATE, ObjectEvent );
    wxDECLARE_EXPORTED_EVENT( DLLIMPEXP_CORE, wxEVT_OBJECT_CREATED, ObjectEvent );
    wxDECLARE_EXPORTED_EVENT( DLLIMPEXP_CORE, wxEVT_OBJECT_SELECTED, ObjectEvent );
#else
    enum
    {
        wxEVT_OBJECT_CREATE,
        wxEVT_OBJECT_CREATED,
        wxEVT_OBJECT_SELECTED
    };
#endif

#ifndef SWIG
typedef void ( wxEvtHandler::*ObjectEventFunction )( ObjectEvent & );

#define ObjectEventHandler( func ) wxEVENT_HANDLER_CAST( ObjectEventFunction, func )

#define EVT_OBJECT_CREATE( id, fn )     wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_OBJECT_CREATE, id, wxID_ANY, \
                                            ObjectEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#define EVT_OBJECT_CREATED( id, fn )    wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_OBJECT_CREATED, id, wxID_ANY, \
                                            ObjectEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#define EVT_OBJECT_SELECTED( id, fn )   wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_OBJECT_SELECTED, id, wxID_ANY, \
                                            ObjectEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#endif // SWIG

#endif //__WXGD_OBJECT_H__
