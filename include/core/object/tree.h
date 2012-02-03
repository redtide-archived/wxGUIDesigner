///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/tree.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_OBJECT_H__
#define __CORE_OBJECT_H__

#include "core/object/database.h"
#include "core/defs.h"

#include "interfaces/iobject.h"

#include <wx/any.h>

class wxBitmap;
class wxColour;
class wxFont;
class wxPoint;
class wxSize;
//-----------------------------------------------------------------------------
// EventNode Class
//-----------------------------------------------------------------------------

class EventNode
{
public:
    EventNode( EventInfo eventInfo );
    ~EventNode();

    wxString GetName()         { return m_info->GetName(); }
    wxString GetDescription()  { return m_info->GetDescription(); }

    wxString GetHandlerName     ( size_t evtTypeIndex ) const;
    bool     SetHandlerName     ( size_t evtTypeIndex, const wxString &name );
    bool     SetHandlerName     ( const wxString &type , const wxString &name );

    wxString GetTypeName        ( size_t evtTypeIndex  ) const;
    wxString GetTypeDescription ( size_t evtTypeIndex  ) const;
    size_t   GetTypeCount()     { return m_info->GetTypeCount(); }

private:
    EventInfo     m_info;
    wxArrayString m_handlers;
};
//-----------------------------------------------------------------------------
// PropertyNode Class
//-----------------------------------------------------------------------------

class PropertyNode
{
public:
    PropertyNode( PropertyInfo info ) : m_info( info ),
                                        m_value( info->GetDefaultValue() ) {}
    ~PropertyNode();

    wxString        GetName()        const { return m_info->GetName(); }
    wxString        GetLabel()       const { return m_info->GetLabel(); }
    wxString        GetDescription() const { return m_info->GetDescription(); }
    PropertyType    GetType()        const { return m_info->GetType(); }
    PropertyInfo    GetInfo()        const { return m_info; }

    void            AddChild( Property prop )   { m_children.push_back( prop ); }
    size_t          GetChildCount()             { return m_children.size(); }
    Property        GetChild( size_t index ) const;

    wxArrayString   GetAsArrayString()  const;
    wxBitmap        GetAsBitmap()       const;
    bool            GetAsBool()         const;
    wxColour        GetAsColour()       const;
    float           GetAsFloat()        const;
    wxFont          GetAsFont()         const;
    int             GetAsInt()          const;
    wxPoint         GetAsPoint()        const;
    wxSize          GetAsSize()         const;
    wxString        GetAsString()       const;
    int             GetAsStyle()        const;
    wxString        GetAsText()         const;
    wxString        GetAsURL()          const;

private:
    PropertyInfo    m_info;
    Properties      m_children;
    wxAny           m_value;
};
//-----------------------------------------------------------------------------
// ObjectNode Class
//-----------------------------------------------------------------------------

class ObjectNode : public IObjectNode
{
public:
    ObjectNode( Object parent, ClassInfo classInfo = ClassInfo(),
                bool expanded = true );
    ~ObjectNode();

//  virtual wxString GetNameValue() const;
    virtual wxString GetClassName()   const { return m_info->GetName(); }
    virtual wxString GetDescription() const { return m_info->GetDescription(); }

    ClassInfo   GetClassInfo() const            { return m_info; }

    bool        IsRoot() { return m_info->GetType() == CLASS_TYPE_ROOT; }
    bool        IsExpanded()                    { return m_expanded; }
    void        Collapse()                      { m_expanded = false; }
    void        Expand()                        { m_expanded = true; }

    // Events
    void        AddEvent( Event event )         { m_events.push_back( event ); }
    Event       GetEvent( size_t index );
    Event       GetEvent( const wxString &name );
    size_t      GetEventCount()                 { return m_events.size(); }

    // Properties
    void        AddProperty( Property prop );
    Property    GetProperty( size_t index );
    Property    GetProperty( const wxString &name );
    size_t      GetPropertyCount();
    bool        PropertyExists( const wxString &name );

    // Inherited classes
    void        AddBaseInfo( ClassInfo info ) { m_baseinfos.push_back( info ); }
    ClassInfo   GetBaseInfo( size_t index );
    wxString    GetBaseName( size_t index );
    size_t      GetBaseCount()              { return m_info->GetBaseCount(); }

    // Parent / children objects
    void        AddChild( Object child )    { m_children.push_back( child ); }
    Object      GetChild( size_t index );
    Object      GetParent()                 { return m_parent; }
    size_t      GetChildCount()             { return m_children.size(); }

private:
    Property    GetChildProperty( Property parent, const wxString &name );

    Object      m_parent;
    Objects     m_children;
    ClassInfo   m_info;
    ClassInfos  m_baseinfos;
    bool        m_expanded;
    Events      m_events;
    Properties  m_props;
};

//-----------------------------------------------------------------------------
// ObjectTree Singleton Class
//-----------------------------------------------------------------------------

class ObjectTree : public IObjectManager
{
public:
    static ObjectTree *Get();
    static void Free();

    virtual bool    CreateObject   ( const wxString &className );
    virtual void    SelectObject   ( Object object, bool withEvent = true );

    virtual void    AddHandler     ( IObjectHandler *handler );
    virtual void    RemoveHandler  ( IObjectHandler *handler );

    virtual Object  GetSelectObject() const { return m_sel; }

private:
    ObjectTree();
    ~ObjectTree();

    bool            CheckType( const wxString &parentType,
                                const wxString &childType );
    void            SendEvent( Object object, ObjectEventType eventType );

    size_t          GetChildCountByType( Object parent, ClassType clsType );

    typedef std::list< IObjectHandler * > ObjectHandlers;

    Object          m_sel;
    ObjectHandlers  m_handlers;

    static ObjectTree *ms_instance;
};

#endif //__CORE_OBJECT_H__
