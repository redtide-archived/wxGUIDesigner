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

#include "interfaces/iobject.h"
#include "core/defs.h"

class wxNotebook;

class EditorHandler : public IObjectHandler
{
    friend class GUIManager;

    EditorHandler( wxNotebook *owner ) : m_editBook( owner ) {}

    virtual void OnObjectCreated ( Object object );
    virtual void OnObjectDeleted ( Object object );
    virtual void OnObjectExpanded( Object object );
    virtual void OnObjectSelected( Object object );

    wxNotebook *m_editBook;
};

#endif //__CORE_GUI_EDITOR_H__
