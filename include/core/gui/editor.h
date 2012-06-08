///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/editor.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGD_GUI_EDITOR_H__
#define __WXGD_GUI_EDITOR_H__

#include "interfaces/iobject.h"
#include "core/defs.h"

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

    virtual void OnObjectCreated ( wxGDObjectEvent &event );
    virtual void OnObjectDeleted ( wxGDObjectEvent &event );
    virtual void OnObjectExpanded( wxGDObjectEvent &event );
    virtual void OnObjectSelected( wxGDObjectEvent &event );

private:
    void OnDesignerResize( wxSizeEvent &event );

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
};

#endif //__WXGD_GUI_EDITOR_H__
