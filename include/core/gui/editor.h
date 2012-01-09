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

#include "core/gui/manager.h"

#include <plugin/plugin.h>
#include <wx/notebook.h>

class EditorHandler : public PluginHandler
{
    friend class GUIManager;

public:
    EditorHandler( wxNotebook *owner ) : m_editBook( owner ) {}

private:
    void OnPluginLoaded( Plugin *plugin );

    wxNotebook *m_editBook;
};

#endif //__WXGDCORE_EDITOR_H__
