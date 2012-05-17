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
// IObject Class Interface
//-----------------------------------------------------------------------------

class IObject
{
public:
    IObject() {}
    virtual ~IObject() {}

    virtual wxString GetClassName()   const = 0;
    virtual wxString GetDescription() const = 0;
//  virtual wxString GetName()        const = 0;
};
//-----------------------------------------------------------------------------
// IObjectHandler Class Interface
//-----------------------------------------------------------------------------

class IObjectHandler
{
public:
    virtual ~IObjectHandler() {}

    virtual void OnObjectCreated ( Object ) = 0;
    virtual void OnObjectDeleted ( Object ) = 0;
    virtual void OnObjectExpanded( Object ) = 0;
    virtual void OnObjectSelected( Object ) = 0;

protected:
    IObjectHandler() {}
};
//-----------------------------------------------------------------------------
// IObjectManager Class Interface
//-----------------------------------------------------------------------------

class IObjectManager
{
public:
    virtual void    AddHandler     ( IObjectHandler * ) = 0;
    virtual void    RemoveHandler  ( IObjectHandler * ) = 0;

    virtual Object  CreateObject   ( const wxString &, Object ) = 0;
    virtual Object  GetSelectObject() const = 0;

    virtual void    SelectObject   ( Object, bool ) = 0;

    virtual bool    Load           ( const wxString & ) = 0;
    virtual bool    Serialize      ( const wxString & ) = 0;

protected:
    IObjectManager() {}
    virtual ~IObjectManager() {}
};

#endif //__CORE_IWIDGET_H__
