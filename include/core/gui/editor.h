///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/editor.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_GUI_EDITOR_H__
#define __CORE_GUI_EDITOR_H__

#include "interfaces/iwidget.h"
#include "core/defs.h"

class wxNotebook;

class EditorHandler : public IWidgetHandler
{
    friend class GUIManager;

    EditorHandler( wxNotebook *owner ) : m_editBook( owner ) {}

    virtual void OnObjectCreated ( Widget widget );
    virtual void OnObjectDeleted ( Widget widget );
    virtual void OnObjectExpanded( Widget widget );
    virtual void OnObjectSelected( Widget widget );

    wxNotebook *m_editBook;
};

#endif //__CORE_GUI_EDITOR_H__
