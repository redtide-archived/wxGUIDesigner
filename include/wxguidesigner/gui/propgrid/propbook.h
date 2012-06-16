///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propbook.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/30
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPBOOK_H__
#define __WXGUIDESIGNER_GUI_PROPBOOK_H__

class wxNotebook;
class wxWindow;
class wxSizeEvent;
class wxPropertyGridEvent;
class wxHtmlLinkEvent;
class wxPGProperty;
class wxPropertyGrid;
class wxSplitterWindow;
class wxPanel;
class wxHtmlWindow;

class wxGDHandler;
class wxGDObjectEvent;

#include "wxguidesigner/defs.h"
#include "wxguidesigner/interfaces/iobject.h"

class wxGDPropertyBook : public wxNotebook
{
public:
    wxGDPropertyBook( wxGDHandler *handler, wxWindow* parent );
    ~wxGDPropertyBook();

private:
    void OnSize                 ( wxSizeEvent           &event );
    void OnPropGridChanged      ( wxPropertyGridEvent   &event );
    void OnPropGridSelected     ( wxPropertyGridEvent   &event );
    void OnEventGridChanged     ( wxPropertyGridEvent   &event );
    void OnEventGridSelected    ( wxPropertyGridEvent   &event );
    void OnEventGridLeftDClick  ( wxPropertyGridEvent   &event );
    void OnLinkClicked          ( wxHtmlLinkEvent       &event );

    void OnObjectCreated        ( wxGDObjectEvent &event );
    void OnObjectDeleted        ( wxGDObjectEvent &event );
    void OnObjectExpanded       ( wxGDObjectEvent &event );
    void OnObjectSelected       ( wxGDObjectEvent &event );

    void LoadEvents             ( Object object );
    void LoadProperties         ( Object object );

    wxPGProperty *AddProperty   ( Property prop );

    wxGDHandler     * m_handler;
    wxPropertyGrid  * m_pgEvents;
    wxPropertyGrid  * m_pgProps;
    wxSplitterWindow* m_egSplitter;
    wxSplitterWindow* m_pgSplitter;
    wxPanel         * m_egDesc;
    wxPanel         * m_pgDesc;
    wxHtmlWindow    * m_egHtml;
    wxHtmlWindow    * m_pgHtml;
};

#endif //__WXGUIDESIGNER_PROPBOOK_H__
