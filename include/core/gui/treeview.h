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

class wxTreeCtrl;
class wxTreeEvent;

class TreeViewHandler
{
    friend class GUIManager;

public:
    TreeViewHandler( wxTreeCtrl *owner ) : m_treeView( owner ) {}

private:
    void OnBeginDrag        ( wxTreeEvent &event );
    void OnEndDrag          ( wxTreeEvent &event );
    void OnSelChanged       ( wxTreeEvent &event );
    void OnItemCollapsed    ( wxTreeEvent &event );
    void OnItemExpanded     ( wxTreeEvent &event );
    void OnItemRightClick   ( wxTreeEvent &event );

    wxTreeCtrl *m_treeView;
};

#endif //__WXGDCORE_TREEVIEW_H__


