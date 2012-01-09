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

    virtual void OnObjectCreated ( const IObject &object ) = 0;
    virtual void OnObjectDeleted ( const IObject &object ) = 0;
    virtual void OnObjectExpanded( const IObject &object ) = 0;
    virtual void OnObjectSelected( const IObject &object ) = 0;

protected:
    IObjectHandler() {}
};
//-----------------------------------------------------------------------------
// IObjectTree Class Interface
//-----------------------------------------------------------------------------

class IObjectTree
{
public:
    virtual bool CreateObject   ( const wxString &className,
                                  const wxString &category ) = 0;

    virtual void AddHandler     ( IObjectHandler *handler ) = 0;
    virtual void RemoveHandler  ( IObjectHandler *handler ) = 0;

protected:
    IObjectTree() {}
    virtual ~IObjectTree() {}
};

#endif //__WXGD_IOBJECT_H__
