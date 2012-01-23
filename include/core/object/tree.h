///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/tree.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGD_WIDGET_H__
#define __WXGD_WIDGET_H__

#include "core/object/database.h"
#include "core/defs.h"

#include "interfaces/iwidget.h"

#include <wx/any.h>

// Note: in wxPropertyGrid a string property can be used as category

//-----------------------------------------------------------------------------
// PropertyBase Class
//-----------------------------------------------------------------------------

class wxBitmap;
class wxColour;
class wxFont;
class wxPoint;
class wxSize;

enum WidgetEventType
{
    EVT_WIDGET_CREATED,
    EVT_WIDGET_DELETED,
    EVT_WIDGET_EXPANDED,
    EVT_WIDGET_SELECTED
};

class PropertyBase
{
public:
    PropertyBase( PropertyInfo info ) : m_info( info ),
                                        m_value( info->GetDefaultValue() ) {}
    ~PropertyBase();

    wxString     GetName()        const { return m_info->GetName(); }
    wxString     GetLabel()       const { return m_info->GetLabel(); }
    wxString     GetDescription() const { return m_info->GetDescription(); }
    PropertyType GetType()        const { return m_info->GetType(); }

    wxBitmap    GetAsBitmap()   const;
    bool        GetAsBool()     const;
    wxColour    GetAsColour()   const;
    float       GetAsFloat()    const;
    wxFont      GetAsFont()     const;
    int         GetAsInt()      const;
    wxPoint     GetAsPoint()    const;
    wxSize      GetAsSize()     const;
    wxString    GetAsString()   const;
    long        GetAsStyle()    const;
    wxString    GetAsText()     const;
    wxString    GetAsURL()      const;

private:
    PropertyInfo m_info;
    wxAny        m_value;
};

//-----------------------------------------------------------------------------
// EventBase Class
//-----------------------------------------------------------------------------

class EventBase
{
public:
    EventBase( EventInfo eventInfo ) : m_info( eventInfo )
    {
        m_handlers.Alloc( m_info->GetTypeCount() );
    }
    ~EventBase() {}

    wxString GetClassName()         { return m_info->GetClassName(); }
    wxString GetClassDescription()  { return m_info->GetClassDescription(); }

    wxString GetHandlerName( size_t eventTypeIndex )
    {
        if ( eventTypeIndex < m_handlers.GetCount() )
            return m_handlers.Item( eventTypeIndex );

        return wxEmptyString;
    }

    bool SetHandlerName( size_t eventTypeIndex, const wxString &name )
    {
        if ( eventTypeIndex < m_handlers.GetCount() )
        {
            m_handlers[eventTypeIndex] = name;
            return true;
        }

        return false;
    }

private:
    EventInfo     m_info;
    wxArrayString m_handlers;
};

//-----------------------------------------------------------------------------
// WidgetNode Class
//-----------------------------------------------------------------------------

class WidgetNode : public IWidgetNode
{
public:
    WidgetNode( Widget      parent,
                WidgetInfo  classInfo = WidgetInfo(),
                bool        expanded  = true )
    : m_parent( parent ), m_info( classInfo ), m_expanded( expanded ) {}

    ~WidgetNode();

    void AddChild   ( Widget child )        { m_children.push_back( child ); }
    void AddProperty( Property property )   { m_props.push_back( property ); }
    void AddEvent   ( Event event )         { m_events.push_back( event ); }

    Widget   GetParent()    const           { return m_parent; }
    Widgets  GetChildren()  const           { return m_children; }

    virtual wxString GetName()      const;
    virtual wxString GetClassName() const   { return m_info->GetClassName(); }

    bool HasChildren()                      { return m_children.size() > 0; }

    bool IsExpanded()                       { return m_expanded; }
    void Collapse()                         { m_expanded = false; }
    void Expand()                           { m_expanded = true; }

    bool IsRoot()
    {
        bool isroot = m_info->GetClassName() == "Root";
        wxLogDebug( isroot ? "is root" : "not root" );

        if ( m_info.get() )
            return m_info->GetClassName() == "Root";

        return false;
    }

private:
    Widget      m_parent;
    WidgetInfo  m_info;
    bool        m_expanded;
    Widgets     m_children;
    Events      m_events;
    Properties  m_props;
};

//-----------------------------------------------------------------------------
// WidgetTree Singleton Class
//-----------------------------------------------------------------------------

class WidgetTree : public IWidgetManager
{
public:
    static WidgetTree *Get();
    void Free();

    virtual bool CreateObject   ( const wxString &className );
    virtual void SelectObject   ( Widget widget );

    virtual void AddHandler     ( IWidgetHandler *handler );
    virtual void RemoveHandler  ( IWidgetHandler *handler );

private:
    WidgetTree() : m_root( new WidgetNode( Widget(),
                           WidgetInfo( new WidgetInfoBase("Root") ) ) ),
                   m_sel( m_root ) {}
    ~WidgetTree();

    bool CheckType( const wxString &parentType, const wxString &childType );

    void SendEvent( Widget widget, WidgetEventType eventType );

    Widget m_root;
    Widget m_sel;

    std::list< IWidgetHandler * > m_handlers;

    static WidgetTree *ms_instance;
};
/*
//-----------------------------------------------------------------------------
// WidgetEvent
//-----------------------------------------------------------------------------

class WidgetEvent : public wxEvent
{
public:
    WidgetEvent( wxEventType type = wxEVT_NULL, int id = 0 );

#ifndef SWIG
    WidgetEvent( const WidgetEvent &event );
#endif
    ~WidgetEvent();

    virtual wxEvent *Clone() const { return new WidgetEvent( *this ); }

private:
#ifndef SWIG
    wxDECLARE_DYNAMIC_CLASS( WidgetEvent );
#endif
};

#ifndef SWIG
    wxDECLARE_EVENT( DLLIMPEXP_CORE, wxEVT_WIDGET_CREATE, WidgetEvent );
    wxDECLARE_EVENT( DLLIMPEXP_CORE, wxEVT_WIDGET_CREATED, WidgetEvent );
    wxDECLARE_EVENT( DLLIMPEXP_CORE, wxEVT_WIDGET_SELECTED, WidgetEvent );
#else
    enum
    {
        wxEVT_WIDGET_CREATE,
        wxEVT_WIDGET_CREATED,
        wxEVT_WIDGET_SELECTED
    };
#endif

#ifndef SWIG
typedef void ( wxEvtHandler::*WidgetEventFunction )( WidgetEvent & );

#define WidgetEventHandler( func ) wxEVENT_HANDLER_CAST( WidgetEventFunction, func )

#define EVT_WIDGET_CREATE( id, fn )     wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_WIDGET_CREATE, id, wxID_ANY, \
                                            WidgetEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#define EVT_WIDGET_CREATED( id, fn )    wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_WIDGET_CREATED, id, wxID_ANY, \
                                            WidgetEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#define EVT_WIDGET_SELECTED( id, fn )   wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_WIDGET_SELECTED, id, wxID_ANY, \
                                            WidgetEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#endif // SWIG
*/
#endif //__WXGD_WIDGET_H__
