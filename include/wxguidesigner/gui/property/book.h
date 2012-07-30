///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/property/book.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/30
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPERTY_BOOK_H__
#define __WXGUIDESIGNER_GUI_PROPERTY_BOOK_H__

class wxNotebook;
class wxWindow;
class wxChildFocusEvent;
class wxSizeEvent;
class wxPropertyGridEvent;
class wxHtmlLinkEvent;
class wxPGProperty;
class wxPropertyGrid;
class wxSplitterWindow;
class wxPanel;
class wxHtmlWindow;

namespace wxGD
{
namespace RTTI { class ObjectEvent; };

class Handler;

namespace Property
{
class Book : public wxNotebook
{
public:
    Book( Handler *handler, wxWindow *parent );
    ~Book();

private:
    void OnChildFocus           ( wxChildFocusEvent     &event );
    void OnSize                 ( wxSizeEvent           &event );
    void OnPropGridChanged      ( wxPropertyGridEvent   &event );
    void OnPropGridSelected     ( wxPropertyGridEvent   &event );
    void OnEventGridChanged     ( wxPropertyGridEvent   &event );
    void OnEventGridSelected    ( wxPropertyGridEvent   &event );
    void OnEventGridLeftDClick  ( wxPropertyGridEvent   &event );
    void OnLinkClicked          ( wxHtmlLinkEvent       &event );

    void OnObjectSelected       ( RTTI::ObjectEvent &event );

    void LoadEvents             ( RTTI::Object object );
    void LoadProperties         ( RTTI::Object object );

    wxPGProperty *AddProperty   ( RTTI::Property prop );

    Handler             *m_handler;
    wxPropertyGrid      *m_pgEvents;
    wxPropertyGrid      *m_pgProps;
    wxSplitterWindow    *m_egSplitter;
    wxSplitterWindow    *m_pgSplitter;
    wxPanel             *m_egDesc;
    wxPanel             *m_pgDesc;
    wxHtmlWindow        *m_egHtml;
    wxHtmlWindow        *m_pgHtml;
};

};
};

#endif //__WXGUIDESIGNER_GUI_PROPERTY_BOOK_H__
