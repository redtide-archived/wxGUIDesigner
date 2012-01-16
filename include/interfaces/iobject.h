///////////////////////////////////////////////////////////////////////////////
// Name:        interfaces/iobject.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/01/06
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGD_IOBJECT_H__
#define __WXGD_IOBJECT_H__

class wxString;

//-----------------------------------------------------------------------------
// IObject Class Interface
//-----------------------------------------------------------------------------

class IObject
{
public:
    IObject() {}
    virtual ~IObject() {}
};
//-----------------------------------------------------------------------------
// IObjectHandler Class Interface
//-----------------------------------------------------------------------------

class IObjectHandler
{
public:
    virtual ~IObjectHandler() {}

protected:
    virtual void OnObjectCreated ( IObject *object ) = 0;
    virtual void OnObjectDeleted ( IObject *object ) = 0;
    virtual void OnObjectExpanded( IObject *object ) = 0;
    virtual void OnObjectSelected( IObject *object ) = 0;

    IObjectHandler() {}
};
//-----------------------------------------------------------------------------
// IObjectManager Class Interface
//-----------------------------------------------------------------------------

class IObjectManager
{
public:
    virtual bool CreateObject   ( const wxString &className ) = 0;

    virtual void AddHandler     ( IObjectHandler *handler ) = 0;
    virtual void RemoveHandler  ( IObjectHandler *handler ) = 0;

protected:
    IObjectManager() {}
    virtual ~IObjectManager() {}
};

#endif //__WXGD_IOBJECT_H__
