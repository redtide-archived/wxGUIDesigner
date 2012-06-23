///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/editor.h
// Purpose:     wxGDEditorBook: Visual and code editor wxNotebook
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_EDITOR_H__
#define __WXGUIDESIGNER_GUI_EDITOR_H__

#include <utility>
#include <map>

class wxNotebook;
class wxPanel;
class wxScrolledWindow;
class wxStaticBitmap;
class wxStaticText;
class wxWindow;

class wxGDHandler;
class wxGDObjectEvent;
class wxGDTitleBarPanel;
class wxGDResizingPanel;
class wxGlossyButton;

class wxGDEditorBook : public wxNotebook
{
public:
    wxGDEditorBook( wxGDHandler *handler, wxWindow* parent );
    ~wxGDEditorBook();

private:
    typedef pair< Object, wxObject * > wxGDObject;
    typedef map< Object, wxObject * >  wxGDObjects;

    void LoadCodeEditorPages();

    void OnDesignerResize( wxSizeEvent &event );

    void OnObjectCreated ( wxGDObjectEvent &event );
    void OnObjectDeleted ( wxGDObjectEvent &event );
    void OnObjectSelected( wxGDObjectEvent &event );

    wxGDHandler         *m_handler;
    wxScrolledWindow    *m_scrolled;
    wxGDResizingPanel   *m_resizer;
    wxPanel             *m_border;
    wxPanel             *m_designer;
    wxGDTitleBarPanel   *m_title;
    wxPanel             *m_client;

    wxStaticText        *m_titleLbl;
    wxStaticBitmap      *m_titleBmp;
    wxGlossyButton      *m_titleBmpMin;
    wxGlossyButton      *m_titleBmpMax;
    wxGlossyButton      *m_titleBmpClose;

    wxGDObjects         m_objects;
};

#endif //__WXGUIDESIGNER_GUI_EDITOR_H__
