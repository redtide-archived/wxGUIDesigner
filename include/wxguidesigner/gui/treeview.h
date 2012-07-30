///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/treeview.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/11
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_TREEVIEW_H__
#define __WXGUIDESIGNER_GUI_TREEVIEW_H__

class wxTreeCtrl;
class wxTreeEvent;

namespace wxGD
{
class Handler;

namespace RTTI
{
class ObjectEvent;
};
//=============================================================================
// TreeViewItemData
//=============================================================================
class TreeViewItemData : public wxTreeItemData
{
public:
    TreeViewItemData( RTTI::Object object );
    ~TreeViewItemData();

    RTTI::Object GetObject();

private:
    RTTI::Object m_object;
};
//=============================================================================
// TreeView
//=============================================================================
class TreeView : public wxTreeCtrl
{
public:
    TreeView( Handler *handler, wxWindow* parent );
    ~TreeView();

    void OnObjectCreated    ( RTTI::ObjectEvent &event );
    void OnObjectDeleted    ( RTTI::ObjectEvent &event );
    void OnObjectExpanded   ( RTTI::ObjectEvent &event );
    void OnObjectSelected   ( RTTI::ObjectEvent &event );

    void OnBeginDrag        ( wxTreeEvent &event );
    void OnEndDrag          ( wxTreeEvent &event );
    void OnSelChanged       ( wxTreeEvent &event );
    void OnItemCollapsed    ( wxTreeEvent &event );
    void OnItemExpanded     ( wxTreeEvent &event );
    void OnItemRightClick   ( wxTreeEvent &event );

private:
    Handler *m_handler;
};
};

#endif //__WXGUIDESIGNER_GUI_TREEVIEW_H__


