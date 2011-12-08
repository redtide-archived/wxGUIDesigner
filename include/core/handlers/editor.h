///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/editor.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_EDITOR_H__
#define __WXGDCORE_EDITOR_H__

#include <wx/event.h>
#include <wx/notebook.h>
#include <wx/stc/stc.h>
#include <wx/vector.h>

#include <plugin.h>

class EditorHandler
{
public:
    EditorHandler( wxNotebook *owner );

    void OnPluginLoaded( PluginEvent &event );

protected:
    wxNotebook *m_editor;
};

#endif //__WXGDCORE_EDITOR_H__
