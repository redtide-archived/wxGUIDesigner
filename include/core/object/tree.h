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
class wxFont;
class wxPoint;
class wxSize;
//*****************************************************************************
// EventNode Class
//*****************************************************************************

class EventNode
{
public:
    EventNode( EventInfo eventInfo );
    ~EventNode();

    wxString GetName();
    wxString GetDescription();

    wxString GetHandlerName( size_t typeIndex )     const;
    bool     SetHandlerName( size_t typeIndex,      const wxString &name );
    bool     SetHandlerName( const wxString &type , const wxString &name );

    wxString GetTypeName( size_t typeIndex  )        const;
    wxString GetTypeDescription( size_t typeIndex  ) const;
    size_t   GetTypeCount();

private:
    EventInfo     m_info;
    wxArrayString m_handlers;
};
//*****************************************************************************
// PropertyNode Class
//*****************************************************************************

class PropertyNode
{
public:
    PropertyNode( PropertyInfo info );
    ~PropertyNode();

    wxString        GetName()           const;
    wxString        GetLabel()          const;
    wxString        GetDescription()    const;
    PropertyType    GetType()           const;
    PropertyInfo    GetInfo()           const;

    wxArrayString   GetAsArrayString()  const;
    Bitmap          GetAsBitmap()       const;
    bool            GetAsBool()         const;
    Colour          GetAsColour()       const;
    double          GetAsDouble()       const;
    wxFont          GetAsFont()         const;
    int             GetAsInt()          const;
    wxPoint         GetAsPoint()        const;
    wxSize          GetAsSize()         const;
    wxString        GetAsString()       const;
    int             GetAsStyle()        const;
    wxString        GetAsText()         const;
    wxString        GetAsURL()          const;

    size_t          GetChildCount();
    Property        GetChild( size_t index ) const;
    void            AddChild( Property prop );

    size_t          GetAttributeCount();
    Attribute       GetAttribute( size_t index ) const;
    void            AddAttribute( const wxString &name, const wxString &value );

    void            SetValue( const wxAny &value );

private:
    Attributes      m_attributes;
    PropertyInfo    m_info;
    Properties      m_children;
    wxAny           m_value;
};
//*****************************************************************************
// ObjectNode Class
//*****************************************************************************

class ObjectNode : public IObjectNode
{
public:
    ObjectNode( ClassInfo classInfo,
                Object parent, 
                bool expanded = true );
    ~ObjectNode();

    virtual wxString GetClassName()   const;
    virtual wxString GetDescription() const;

    ClassInfo   GetClassInfo() const;

    bool        IsRoot();
    bool        IsExpanded();
    void        Collapse();
    void        Expand();

    // Events
    void        AddEvent( Event event );
    Event       GetEvent( size_t index );
    Event       GetEvent( const wxString &name );
    size_t      GetEventCount();

    // Properties
    void        AddProperty( Property prop );
    Property    GetProperty( size_t index );
    Property    GetProperty( const wxString &name );
    bool        PropertyExists( const wxString &name );
    size_t      GetPropertyCount();
    Property    GetChildProperty( Property parent, const wxString &name );

    // Inherited classes
    void        AddBaseInfo( ClassInfo info );
    ClassInfo   GetBaseInfo( size_t index );
    wxString    GetBaseName( size_t index );
    size_t      GetBaseCount();

    // Parent / children objects
    void        AddChild( Object child );
    Object      GetChild( size_t index );
    Object      GetParent();
    size_t      GetChildCount();

private:
    ClassInfo   m_info;
    ClassInfos  m_baseinfos;
    Object      m_parent;
    Objects     m_children;
    bool        m_expanded;
    Events      m_events;
    Properties  m_props;
};
//*****************************************************************************
// ObjectTree Singleton Class
//*****************************************************************************

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

    size_t          GetChildInfoCount( Object parent, ClassInfo info );

    typedef std::list< IObjectHandler * > ObjectHandlers;

    Object          m_sel;
    ObjectHandlers  m_handlers;

    static ObjectTree *ms_instance;
};

#endif //__CORE_OBJECT_H__
