///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/treeview.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/11
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_TREEVIEW_H__
#define __CORE_TREEVIEW_H__

#include "interfaces/iobject.h"
#include "core/defs.h"

class wxTreeCtrl;
class wxTreeEvent;

class TreeViewHandler : public IObjectHandler
{
    friend class GUIManager;

    TreeViewHandler( wxTreeCtrl *owner ) : m_treeView( owner ) {}

    virtual void OnObjectCreated ( Object object );
    virtual void OnObjectDeleted ( Object object );
    virtual void OnObjectExpanded( Object object );
    virtual void OnObjectSelected( Object object );

    void OnBeginDrag        ( wxTreeEvent &event );
    void OnEndDrag          ( wxTreeEvent &event );
    void OnSelChanged       ( wxTreeEvent &event );
    void OnItemCollapsed    ( wxTreeEvent &event );
    void OnItemExpanded     ( wxTreeEvent &event );
    void OnItemRightClick   ( wxTreeEvent &event );

    wxTreeCtrl *m_treeView;
};

#endif //__CORE_TREEVIEW_H__


