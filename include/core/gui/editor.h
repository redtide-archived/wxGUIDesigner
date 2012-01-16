///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/editor.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_EDITOR_H__
#define __WXGDCORE_EDITOR_H__

#include "interfaces/iobject.h"

#include <plugin/plugin.h>

class wxNotebook;

class EditorHandler : public IPluginHandler,
                      public IObjectHandler
{
    friend class GUIManager;

public:
    EditorHandler( wxNotebook *owner ) : m_editBook( owner ) {}

private:
    virtual void OnPluginLoaded( Plugin *plugin );

    virtual void OnObjectCreated ( IObject *object );
    virtual void OnObjectDeleted ( IObject *object );
    virtual void OnObjectExpanded( IObject *object );
    virtual void OnObjectSelected( IObject *object );

    wxNotebook *m_editBook;
};

#endif //__WXGDCORE_EDITOR_H__
