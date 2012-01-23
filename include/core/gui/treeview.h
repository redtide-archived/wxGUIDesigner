///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/treeview.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/11
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_TREEVIEW_H__
#define __WXGDCORE_TREEVIEW_H__

#include "interfaces/iwidget.h"
#include "core/defs.h"

#include <wx/treectrl.h>

class TreeViewItemData : public wxTreeItemData
{
public:
    TreeViewItemData( Widget widget ) : m_widget( widget ) {}
    ~TreeViewItemData() {}

    Widget GetWidget() { return m_widget; }

private:
    Widget m_widget;
};

class TreeViewHandler : public IWidgetHandler
{
    friend class GUIManager;

    TreeViewHandler( wxTreeCtrl *owner ) : m_treeView( owner ) {}

    virtual void OnObjectCreated ( Widget widget );
    virtual void OnObjectDeleted ( Widget widget );
    virtual void OnObjectExpanded( Widget widget );
    virtual void OnObjectSelected( Widget widget );

    void OnBeginDrag        ( wxTreeEvent &event );
    void OnEndDrag          ( wxTreeEvent &event );
    void OnSelChanged       ( wxTreeEvent &event );
    void OnItemCollapsed    ( wxTreeEvent &event );
    void OnItemExpanded     ( wxTreeEvent &event );
    void OnItemRightClick   ( wxTreeEvent &event );

    wxTreeCtrl *m_treeView;
};

#endif //__WXGDCORE_TREEVIEW_H__


