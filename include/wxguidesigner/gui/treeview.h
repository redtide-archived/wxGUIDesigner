///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/treeview.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/11
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_TREEVIEW_H__
#define __WXGUIDESIGNER_GUI_TREEVIEW_H__

#include "wxguidesigner/interfaces/iobject.h"
#include "wxguidesigner/defs.h"

class wxTreeCtrl;
class wxTreeEvent;

class wxGDHandler;
class wxGDObjectEvent;

class wxGDTreeView : public wxTreeCtrl
{
public:
    wxGDTreeView( wxGDHandler *handler, wxWindow* parent );
    ~wxGDTreeView();

    virtual void OnObjectCreated ( wxGDObjectEvent &event );
    virtual void OnObjectDeleted ( wxGDObjectEvent &event );
    virtual void OnObjectExpanded( wxGDObjectEvent &event );
    virtual void OnObjectSelected( wxGDObjectEvent &event );

    void OnBeginDrag        ( wxTreeEvent &event );
    void OnEndDrag          ( wxTreeEvent &event );
    void OnSelChanged       ( wxTreeEvent &event );
    void OnItemCollapsed    ( wxTreeEvent &event );
    void OnItemExpanded     ( wxTreeEvent &event );
    void OnItemRightClick   ( wxTreeEvent &event );

private:
    wxGDHandler *m_handler;
};

#endif //__WXGUIDESIGNER_GUI_TREEVIEW_H__


