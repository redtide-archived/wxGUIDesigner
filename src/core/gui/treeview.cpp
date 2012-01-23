///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/treeview.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/11
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/gui/treeview.h"
#include "core/gui/iconprovider.h"
#include "core/gui/manager.h"
#include "core/object/tree.h"

#include <wx/imaglist.h>
#include <wx/treectrl.h>

void TreeViewHandler::OnObjectCreated( Widget widget )
{
    wxTreeItemId parent = m_treeView->GetRootItem();
    wxString     name   = widget->GetClassName();
    int          imgIdx = GUIManager::Get()->GetImageIndex( name );

    wxTreeItemId item = m_treeView->AppendItem( parent, name, imgIdx );

    m_treeView->SetItemData( item, new TreeViewItemData( widget ) );
    m_treeView->SelectItem( item );
}

void TreeViewHandler::OnObjectDeleted( Widget widget )
{
    wxLogDebug( "Deleted %s", widget->GetClassName() );
}

void TreeViewHandler::OnObjectExpanded( Widget widget )
{
    wxLogDebug( "Expanded %s", widget->GetClassName() );
}

void TreeViewHandler::OnObjectSelected( Widget widget )
{
    wxLogDebug( "Selected %s", widget->GetClassName() );
}

void TreeViewHandler::OnBeginDrag( wxTreeEvent &event )
{
    event.Allow();
}

void TreeViewHandler::OnEndDrag( wxTreeEvent &event )
{
    
}

void TreeViewHandler::OnSelChanged( wxTreeEvent &event )
{
    wxTreeItemId      item = event.GetItem();
    TreeViewItemData *data =
    dynamic_cast< TreeViewItemData * >( m_treeView->GetItemData( item ) );

    if ( data )
    {
        Widget widget( data->GetWidget() );

        if ( widget.get() )
            WidgetTree::Get()->SelectObject( widget );
    }

    event.Skip();
}

void TreeViewHandler::OnItemCollapsed( wxTreeEvent &event )
{
    wxTreeItemId      item = event.GetItem();
    TreeViewItemData *data =
    dynamic_cast< TreeViewItemData * >( m_treeView->GetItemData( item ) );

    if ( data )
    {
        Widget widget( data->GetWidget() );

        if ( widget.get() )
            widget->Collapse();
    }

    event.Skip();
}

void TreeViewHandler::OnItemExpanded( wxTreeEvent &event )
{
    wxTreeItemId      item = event.GetItem();
    TreeViewItemData *data =
    dynamic_cast< TreeViewItemData * >( m_treeView->GetItemData( item ) );

    if ( data )
    {
        Widget widget( data->GetWidget() );

        if ( widget.get() )
            widget->Expand();
    }

    event.Skip();
}

void TreeViewHandler::OnItemRightClick( wxTreeEvent &event )
{
    
}
