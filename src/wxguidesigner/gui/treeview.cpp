///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/treeview.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/11
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/imaglist.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/tree.h"

#include "wxguidesigner/gui/treeview.h"
#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/events.h"
#include "wxguidesigner/rtti/tree.h"
//============================================================================
// TreeViewItemData
//============================================================================
wxGD::TreeViewItemData::TreeViewItemData( RTTI::Object object )
:
m_object( object )
{
}

wxGD::TreeViewItemData::~TreeViewItemData()
{
}

wxGD::RTTI::Object wxGD::TreeViewItemData::GetObject()
{
    return m_object;
}
//============================================================================
// TreeView
//============================================================================
wxGD::TreeView::TreeView( Handler *handler, wxWindow *parent )
:
wxTreeCtrl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                    wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT ),
m_handler( handler )
{
    wxImageList *imageList = m_handler->GetLargeImageList();
    if( !imageList )
        return;

    SetImageList( imageList );

    int imageIndex = ArtProvider::GetItemImageListIndex( "controls", "Project" );

    AddRoot( "Project", imageIndex );

    Bind( wxEVT_COMMAND_TREE_BEGIN_DRAG,        &TreeView::OnBeginDrag,     this );
    Bind( wxEVT_COMMAND_TREE_END_DRAG,          &TreeView::OnEndDrag,       this );
    Bind( wxEVT_COMMAND_TREE_ITEM_COLLAPSED,    &TreeView::OnItemCollapsed, this );
    Bind( wxEVT_COMMAND_TREE_ITEM_EXPANDED,     &TreeView::OnItemExpanded,  this );
    Bind( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,  &TreeView::OnItemRightClick,this );
    Bind( wxEVT_COMMAND_TREE_SEL_CHANGED,       &TreeView::OnSelChanged,    this );

    Bind( wxGD_EVT_OBJECT_CREATED, &TreeView::OnObjectCreated, this );
}

wxGD::TreeView::~TreeView()
{
}

void wxGD::TreeView::OnObjectCreated( RTTI::ObjectEvent &event )
{
    RTTI::Object object = event.GetObject();
    if( !object )
        return;

    wxTreeItemId item;
    wxString name   = object->GetClassName();
    int      imgIdx = ArtProvider::GetItemImageListIndex( "controls", name );

    if( object->IsRoot() )
    {
        item = AddRoot( name, imgIdx );
    }
    else
    {
        wxTreeItemId parent;
        if( object->GetParent()->IsRoot() )
        {
            parent = GetRootItem();
        }
        else
        {
            parent = GetSelection();
            while( parent.IsOk() )
            {
                TreeViewItemData *data = dynamic_cast< TreeViewItemData * >
                                                    ( GetItemData( parent ) );
                if( !data )
                    return;

                if( object->GetParent() == data->GetObject() )
                    break;

                parent = GetItemParent( parent );
            }

            if( !parent.IsOk() )
                return;
        }

        item = AppendItem( parent, name, imgIdx );
    }

    SetItemData( item, new TreeViewItemData( object ) );
    SelectItem( item );
}

void wxGD::TreeView::OnObjectDeleted( RTTI::ObjectEvent &event )
{
//  wxLogDebug( "Deleted %s", object->GetClassName() );
}

void wxGD::TreeView::OnObjectExpanded( RTTI::ObjectEvent &event )
{
//  wxLogDebug( "Expanded %s", object->GetClassName() );
}

void wxGD::TreeView::OnObjectSelected( RTTI::ObjectEvent &event )
{
//  wxLogDebug( "Selected %s", object->GetClassName() );
}

void wxGD::TreeView::OnBeginDrag( wxTreeEvent &event )
{
    event.Allow();
}

void wxGD::TreeView::OnEndDrag( wxTreeEvent &event )
{
    
}

void wxGD::TreeView::OnSelChanged( wxTreeEvent &event )
{
    wxTreeItemId item  = event.GetItem();
    TreeViewItemData *data = dynamic_cast< TreeViewItemData * >
                                        ( GetItemData( item ) );
    if( data )
    {
        RTTI::Object object( data->GetObject() );
        if( object )
        {
            m_handler->SelectObject( object, GetId() );
/*
            ObjectEvent event( wxGD_EVT_OBJECT_SELECTED, GetId(), object );
            m_handler->SendEvent( event );
*/
        }
    }

    event.Skip();
}

void wxGD::TreeView::OnItemCollapsed( wxTreeEvent &event )
{
    wxTreeItemId item  = event.GetItem();
    TreeViewItemData *data = dynamic_cast< TreeViewItemData * >
                                        ( GetItemData( item ) );
    if( data )
    {
        RTTI::Object object( data->GetObject() );

        if( object )
            object->Collapse();
    }

    event.Skip();
}

void wxGD::TreeView::OnItemExpanded( wxTreeEvent &event )
{
    wxTreeItemId item  = event.GetItem();
    TreeViewItemData *data = dynamic_cast< TreeViewItemData * >
                                        ( GetItemData( item ) );
    if( data )
    {
        RTTI::Object object( data->GetObject() );

        if( object )
            object->Expand();
    }

    event.Skip();
}

void wxGD::TreeView::OnItemRightClick( wxTreeEvent &event )
{
    
}
