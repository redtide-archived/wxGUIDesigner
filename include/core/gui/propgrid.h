///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propgrid.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/10
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_PROPGRID_H__
#define __CORE_PROPGRID_H__

#include "interfaces/iobject.h"
#include "core/defs.h"

class wxNotebook;
class wxPGProperty;
class wxPropertyGridEvent;
class wxSizeEvent;

class PropBookHandler : public IObjectHandler
{
    friend class GUIManager;

    PropBookHandler( wxNotebook *owner ) : m_propBook( owner ) {}

    virtual void OnObjectCreated ( Object object );
    virtual void OnObjectDeleted ( Object object );
    virtual void OnObjectExpanded( Object object );
    virtual void OnObjectSelected( Object object );

    void OnPGChanged( wxPropertyGridEvent &event );
    void OnEGChanged( wxPropertyGridEvent &event );
    void OnPGSelected( wxPropertyGridEvent &event );
    void OnEGSelected( wxPropertyGridEvent &event );
    void OnEGDblClick( wxPropertyGridEvent &event );

    void OnSize( wxSizeEvent &event );

    void LoadEvents( Object object );
    void LoadProperties( Object object );

    wxPGProperty *AddProperty( Property prop );

    wxNotebook *m_propBook;
};

#endif //__CORE_PROPGRID_H__
