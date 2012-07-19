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
// wxGDTreeItemData
//============================================================================
class wxGDTreeItemData : public wxTreeItemData
{
public:
    wxGDTreeItemData( Object object ) : m_object( object ) {}
    ~wxGDTreeItemData() {}

    Object GetObject() { return m_object; }

private:
    Object m_object;
};
//============================================================================
// wxGDTreeView
//============================================================================
wxGDTreeView::wxGDTreeView( wxGDHandler *handler, wxWindow *parent )
:
wxTreeCtrl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                    wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT ),
m_handler( handler )
{
    wxImageList *imageList = m_handler->GetLargeImageList();
    if( !imageList )
        return;

    SetImageList( imageList );

    int imageIndex = wxGDArtProvider::GetItemImageListIndex( "controls", "Project" );

    AddRoot( "Project", imageIndex );

    Bind( wxEVT_COMMAND_TREE_BEGIN_DRAG,        &wxGDTreeView::OnBeginDrag,     this );
    Bind( wxEVT_COMMAND_TREE_END_DRAG,          &wxGDTreeView::OnEndDrag,       this );
    Bind( wxEVT_COMMAND_TREE_ITEM_COLLAPSED,    &wxGDTreeView::OnItemCollapsed, this );
    Bind( wxEVT_COMMAND_TREE_ITEM_EXPANDED,     &wxGDTreeView::OnItemExpanded,  this );
    Bind( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,  &wxGDTreeView::OnItemRightClick,this );
    Bind( wxEVT_COMMAND_TREE_SEL_CHANGED,       &wxGDTreeView::OnSelChanged,    this );

    Bind( wxGD_EVT_OBJECT_CREATED, &wxGDTreeView::OnObjectCreated, this );
}

wxGDTreeView::~wxGDTreeView()
{
}

void wxGDTreeView::OnObjectCreated( wxGDObjectEvent &event )
{
    Object object = event.GetObject();
    if( !object )
        return;

    wxTreeItemId item;
    wxString name   = object->GetClassName();
    int      imgIdx = wxGDArtProvider::GetItemImageListIndex( "controls", name );

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
                wxGDTreeItemData *data = dynamic_cast< wxGDTreeItemData * >
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

    SetItemData( item, new wxGDTreeItemData( object ) );
    SelectItem( item );
}

void wxGDTreeView::OnObjectDeleted( wxGDObjectEvent &event )
{
//  wxLogDebug( "Deleted %s", object->GetClassName() );
}

void wxGDTreeView::OnObjectExpanded( wxGDObjectEvent &event )
{
//  wxLogDebug( "Expanded %s", object->GetClassName() );
}

void wxGDTreeView::OnObjectSelected( wxGDObjectEvent &event )
{
//  wxLogDebug( "Selected %s", object->GetClassName() );
}

void wxGDTreeView::OnBeginDrag( wxTreeEvent &event )
{
    event.Allow();
}

void wxGDTreeView::OnEndDrag( wxTreeEvent &event )
{
    
}

void wxGDTreeView::OnSelChanged( wxTreeEvent &event )
{
    wxTreeItemId      item = event.GetItem();
    wxGDTreeItemData *data = dynamic_cast< wxGDTreeItemData * >
                                        ( GetItemData( item ) );
    if( data )
    {
        Object object( data->GetObject() );
        if( object )
        {
            m_handler->SelectObject( object, GetId() );
/*
            wxGDObjectEvent event( wxGD_EVT_OBJECT_SELECTED, GetId(), object );
            m_handler->SendEvent( event );
*/
        }
    }

    event.Skip();
}

void wxGDTreeView::OnItemCollapsed( wxTreeEvent &event )
{
    wxTreeItemId      item = event.GetItem();
    wxGDTreeItemData *data = dynamic_cast< wxGDTreeItemData * >
                                        ( GetItemData( item ) );
    if( data )
    {
        Object object( data->GetObject() );

        if( object )
            object->Collapse();
    }

    event.Skip();
}

void wxGDTreeView::OnItemExpanded( wxTreeEvent &event )
{
    wxTreeItemId      item = event.GetItem();
    wxGDTreeItemData *data = dynamic_cast< wxGDTreeItemData * >
                                        ( GetItemData( item ) );
    if( data )
    {
        Object object( data->GetObject() );

        if( object )
            object->Expand();
    }

    event.Skip();
}

void wxGDTreeView::OnItemRightClick( wxTreeEvent &event )
{
    
}
