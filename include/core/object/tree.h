///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/tree.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGD_OBJECT_H__
#define __WXGD_OBJECT_H__

#include "core/object/database.h"
#include "core/defs.h"

#include "interfaces/iobject.h"

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

class PropertyBase
{
    friend class ObjectTree;
    friend class ObjectBase;

    PropertyBase( PropertyType type, const wxString& label, const wxAny& value )
            : m_type( type ), m_label( label ), m_value( value )
            {}
    ~PropertyBase();

    const wxString&    GetLabel() const { return m_label; }
    const PropertyType GetType()  const { return m_type; }

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

    PropertyType m_type;
    wxString     m_label;
    wxAny        m_value;
};

//-----------------------------------------------------------------------------
// EventBase Class
//-----------------------------------------------------------------------------

class EventBase
{
    friend class ObjectTree;
    friend class ObjectBase;

    EventBase( const wxString& name, const wxString& className,
               const wxString& description = wxEmptyString )
            : m_name( name ), m_class( className ), m_description( description )
            {}
    ~EventBase();

    const wxString m_name;
    const wxString m_class;
    const wxString m_description;
};

//-----------------------------------------------------------------------------
// ObjectBase Class
//-----------------------------------------------------------------------------

class ObjectBase : public IObject
{
public:
    ObjectBase( ClassInfo classInfo = ClassInfo(), Object parent = Object() )
    : m_info( classInfo ), m_parent( parent ) {}

    ~ObjectBase();

//  bool Destroy();

    void AddChild( Object child )           { m_children.push_back( child ); }
    void AddProperty( Property property )   { m_props.push_back( property ); }
    void AddEvent( Event event )            { m_events.push_back( event ); }

    const Object    GetParent()     { return m_parent; }
    const Objects   GetChildren()   { return m_children; }
    const wxString  GetClassName()  { return m_info->GetClassName(); }

    bool HasChildren()              { return m_children.size() > 0; }

    bool IsExpanded()               { return m_expanded; }
    void Collapse()                 { m_expanded = false; }
    void Expand()                   { m_expanded = true; }

private:
    bool        m_expanded;
    ClassInfo   m_info;
    Object      m_parent;
    Objects     m_children;
    Events      m_events;
    Properties  m_props;
};

//-----------------------------------------------------------------------------
// ObjectTree Singleton Class
//-----------------------------------------------------------------------------

class ObjectTree : public IObjectTree
{
public:
    static ObjectTree *Get();
    void Free();

    virtual bool CreateObject( const wxString &className,
                                const wxString &category );

    virtual void AddHandler( IObjectHandler *handler );
    virtual void RemoveHandler( IObjectHandler *handler );

    void SetRoot( Object object );
    bool HaveRoot()                 { return m_root.get() != NULL; }
    bool HaveSelection()            { return m_sel.get() != NULL; }

private:
    ObjectTree() : m_root( Object() ), m_sel( Object() ) {}
    ~ObjectTree();

    bool CheckType( const wxString &parentType, const wxString &childType );

    Object m_root;
    Object m_sel;

    std::list< IObjectHandler * > m_handlers;

    static ObjectTree *ms_instance;
};
/*
//-----------------------------------------------------------------------------
// ObjectEvent
//-----------------------------------------------------------------------------

class ObjectEvent : public wxEvent
{
public:
    ObjectEvent( wxEventType type = wxEVT_NULL, int id = 0 );

#ifndef SWIG
    ObjectEvent( const ObjectEvent &event );
#endif
    ~ObjectEvent();

    virtual wxEvent *Clone() const { return new ObjectEvent( *this ); }

private:
#ifndef SWIG
    wxDECLARE_DYNAMIC_CLASS( ObjectEvent );
#endif
};

#ifndef SWIG
    wxDECLARE_EVENT( DLLIMPEXP_CORE, wxEVT_OBJECT_CREATE, ObjectEvent );
    wxDECLARE_EVENT( DLLIMPEXP_CORE, wxEVT_OBJECT_CREATED, ObjectEvent );
    wxDECLARE_EVENT( DLLIMPEXP_CORE, wxEVT_OBJECT_SELECTED, ObjectEvent );
#else
    enum
    {
        wxEVT_OBJECT_CREATE,
        wxEVT_OBJECT_CREATED,
        wxEVT_OBJECT_SELECTED
    };
#endif

#ifndef SWIG
typedef void ( wxEvtHandler::*ObjectEventFunction )( ObjectEvent & );

#define ObjectEventHandler( func ) wxEVENT_HANDLER_CAST( ObjectEventFunction, func )

#define EVT_OBJECT_CREATE( id, fn )     wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_OBJECT_CREATE, id, wxID_ANY, \
                                            ObjectEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#define EVT_OBJECT_CREATED( id, fn )    wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_OBJECT_CREATED, id, wxID_ANY, \
                                            ObjectEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#define EVT_OBJECT_SELECTED( id, fn )   wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_OBJECT_SELECTED, id, wxID_ANY, \
                                            ObjectEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#endif // SWIG
*/
#endif //__WXGD_OBJECT_H__
