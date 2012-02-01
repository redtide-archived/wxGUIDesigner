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

class TreeViewItemData : public wxTreeItemData
{
public:
    TreeViewItemData( Object object ) : m_object( object ) {}
    ~TreeViewItemData() {}

    Object GetObject() { return m_object; }

private:
    Object m_object;
};

void TreeViewHandler::OnObjectCreated( Object object )
{
    wxString name   = object->GetClassName();
    int      imgIdx = GUIManager::Get()->GetImageIndex( name );

    wxTreeItemId item;
    if ( object->IsRoot() )
    {
        item = m_treeView->AddRoot( name, imgIdx );
    }
    else
    {
        wxTreeItemId parent;
        if ( object->GetParent()->IsRoot() )
        {
            parent = m_treeView->GetRootItem();
        }
        else
        {
            parent = m_treeView->GetSelection();
            while ( parent.IsOk() )
            {
                TreeViewItemData *data =
                            dynamic_cast< TreeViewItemData * >
                                        ( m_treeView->GetItemData( parent ) );
                if ( !data )
                    return;

                if ( object->GetParent() == data->GetObject() )
                    break;

                parent = m_treeView->GetItemParent( parent );
            }

            if ( !parent.IsOk() )
                return;
        }

        item = m_treeView->AppendItem( parent, name, imgIdx );
    }

    m_treeView->SetItemData( item, new TreeViewItemData( object ) );
    m_treeView->SelectItem( item );
}

void TreeViewHandler::OnObjectDeleted( Object object )
{
    wxLogDebug( "Deleted %s", object->GetClassName() );
}

void TreeViewHandler::OnObjectExpanded( Object object )
{
    wxLogDebug( "Expanded %s", object->GetClassName() );
}

void TreeViewHandler::OnObjectSelected( Object object )
{
    wxLogDebug( "Selected %s", object->GetClassName() );
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
    wxTreeItemId item = event.GetItem();
    TreeViewItemData *data =
    dynamic_cast< TreeViewItemData * >( m_treeView->GetItemData( item ) );

    if ( data )
    {
        Object object( data->GetObject() );

        if ( object.get() )
            ObjectTree::Get()->SelectObject( object );
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
        Object object( data->GetObject() );

        if ( object.get() )
            object->Collapse();
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
        Object object( data->GetObject() );

        if ( object.get() )
            object->Expand();
    }

    event.Skip();
}

void TreeViewHandler::OnItemRightClick( wxTreeEvent &event )
{
    
}
