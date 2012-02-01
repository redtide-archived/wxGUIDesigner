///////////////////////////////////////////////////////////////////////////////
// Name:        interfaces/iwidget.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/01/06
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_IWIDGET_H__
#define __CORE_IWIDGET_H__

#include "core/defs.h"

class wxString;
//-----------------------------------------------------------------------------
// IObjectNode Class Interface
//-----------------------------------------------------------------------------

class IObjectNode
{
public:
    IObjectNode() {}
    virtual ~IObjectNode() {}

    virtual wxString GetClassName() const = 0;
//  virtual wxString GetName()      const = 0;
};
//-----------------------------------------------------------------------------
// IObjectHandler Class Interface
//-----------------------------------------------------------------------------

class IObjectHandler
{
public:
    virtual ~IObjectHandler() {}

    virtual void OnObjectCreated ( Object object ) = 0;
    virtual void OnObjectDeleted ( Object object ) = 0;
    virtual void OnObjectExpanded( Object object ) = 0;
    virtual void OnObjectSelected( Object object ) = 0;

protected:
    IObjectHandler() {}
};
//-----------------------------------------------------------------------------
// IObjectManager Class Interface
//-----------------------------------------------------------------------------

class IObjectManager
{
public:
    virtual bool CreateObject   ( const wxString &className ) = 0;
    virtual void SelectObject   ( Object object, bool withEvent ) = 0;

    virtual void AddHandler     ( IObjectHandler *handler ) = 0;
    virtual void RemoveHandler  ( IObjectHandler *handler ) = 0;

    virtual Object GetSelectObject() const = 0;

protected:
    IObjectManager() {}
    virtual ~IObjectManager() {}
};

#endif //__CORE_IWIDGET_H__
