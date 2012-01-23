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
// IWidgetNode Class Interface
//-----------------------------------------------------------------------------

class IWidgetNode
{
public:
    IWidgetNode() {}
    virtual ~IWidgetNode() {}

    virtual wxString GetClassName() const = 0;
    virtual wxString GetName()      const = 0;
};
//-----------------------------------------------------------------------------
// IWidgetHandler Class Interface
//-----------------------------------------------------------------------------

class IWidgetHandler
{
public:
    virtual ~IWidgetHandler() {}

    virtual void OnObjectCreated ( Widget widget ) = 0;
    virtual void OnObjectDeleted ( Widget widget ) = 0;
    virtual void OnObjectExpanded( Widget widget ) = 0;
    virtual void OnObjectSelected( Widget widget ) = 0;

protected:
    IWidgetHandler() {}
};
//-----------------------------------------------------------------------------
// IWidgetManager Class Interface
//-----------------------------------------------------------------------------

class IWidgetManager
{
public:
    virtual bool CreateObject   ( const wxString &className ) = 0;
    virtual void SelectObject   ( Widget widget ) = 0;

    virtual void AddHandler     ( IWidgetHandler *handler ) = 0;
    virtual void RemoveHandler  ( IWidgetHandler *handler ) = 0;

protected:
    IWidgetManager() {}
    virtual ~IWidgetManager() {}
};

#endif //__CORE_IWIDGET_H__
