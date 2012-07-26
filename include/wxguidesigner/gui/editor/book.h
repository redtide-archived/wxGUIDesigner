///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/editor/book.h
// Purpose:     wxGDEditorBook: Visual and code editor wxNotebook
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_EDITOR_BOOK_H__
#define __WXGUIDESIGNER_GUI_EDITOR_BOOK_H__

#include <utility>
#include <map>

#include <wx/xml/xml.h>

#include "wxguidesigner/rtti.h"

class wxNotebook;
class wxWindow;

class wxGDEvent;
class wxGDPropertyEvent;
class wxGDObjectEvent;
class wxGDHandler;
class wxGDEditor;

class wxGDEditorBook : public wxNotebook
{
public:
    wxGDEditorBook( wxGDHandler *handler, wxWindow* parent );
    ~wxGDEditorBook();

    wxGDEditor *GetGUIEditor() const;

private:
    void LoadCodeEditorPages();
    void Reload();
    void SetupWindow        ( wxWindow *window );

    void OnObjectCreated    ( wxGDObjectEvent   &event );
    void OnObjectDeleted    ( wxGDObjectEvent   &event );
    void OnObjectSelected   ( wxGDObjectEvent   &event );
    void OnEventChanged     ( wxGDEvent         &event );
    void OnPropertyChanged  ( wxGDPropertyEvent &event );

    wxGDHandler *m_handler;
    wxGDEditor  *m_editor;

    typedef pair< Object, wxObject * > wxGDObject;
    typedef map< Object, wxObject * >  wxGDObjects;
    wxGDObjects m_objects;
    Object      m_oldTop;
};

#endif //__WXGUIDESIGNER_GUI_EDITOR_BOOK_H__
