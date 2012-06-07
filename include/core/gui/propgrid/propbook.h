///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propbook.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/30
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGD_PROPBOOK_H__
#define __WXGD_PROPBOOK_H__

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

#include "core/defs.h"
#include "interfaces/iobject.h"

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

#endif //__WXGD_PROPBOOK_H__
