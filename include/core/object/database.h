///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/database.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/23
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_OBJECT_DATABASE_H__
#define __CORE_OBJECT_DATABASE_H__

#include "core/defs.h"

#include <wx/arrstr.h>
#include <wx/string.h>

#include <wx/log.h>

class wxXmlNode;
//*****************************************************************************
//  EventInfoNode
//*****************************************************************************

class EventInfoNode
{
public:
    EventInfoNode( const wxString &name,
                   const wxString &description = wxEmptyString );
    ~EventInfoNode();

    wxString GetName()          { return m_name; }
    wxString GetDescription()   { return m_desc; }

    wxString GetTypeName        ( size_t index );
    wxString GetTypeDescription ( size_t index );
    size_t   GetTypeCount()     { return m_types.size(); }

private:
    friend class ClassInfoDB;

    void AddDescription( const wxString &description ) { m_desc = description; }
    void AddType( const wxString &name, const wxString &description );

    wxString    m_name;
    wxString    m_desc;
    EventTypes  m_types;
};
//*****************************************************************************
//  PropertyInfoNode
//*****************************************************************************

class PropertyInfoNode
{
public:
    PropertyInfoNode( PropertyType   type,
                      const wxString &name,
                      const wxString &label );

    ~PropertyInfoNode();

    PropertyType GetType()          { return m_type; }
    wxString     GetName()          { return m_name; }
    wxString     GetLabel()         { return m_label; }

    wxString     GetDefaultValue()  { return m_value; }
    wxString     GetDescription()   { return m_desc; }
    size_t       GetChildCount()    { return m_children.size(); }

    PropertyInfo GetChild( size_t index );
    void         AddChild( const wxString &name, PropertyInfo info );

private:
    friend class ClassInfoDB;

    void SetDefaultValue( const wxString &value )     { m_value = value; }
    void SetDescription( const wxString &description ){ m_desc  = description; }

    PropertyInfos   m_children;
    PropertyType    m_type;
    wxString        m_name, m_label, m_value, m_desc;
};
//*****************************************************************************
//  ClassNode
//*****************************************************************************

class ClassNode
{
public:
    ClassNode( const wxString &className, ClassType type = CLASS_TYPE_WIDGET );
    ~ClassNode();

    wxString        GetName()        const { return m_name; }
    wxString        GetDescription() const { return m_desc; }
    ClassType       GetType()        const { return m_type; }

    int             GetMaxAllowedBy( const wxString &parentClsName ) const;
    bool            IsKindOf( const wxString &className );
    bool            IsTypeOf( ClassType type );

    size_t          GetBaseCount()          { return m_bases.GetCount(); }
    size_t          GetEventInfoCount()     { return m_events.size(); }
    size_t          GetPropertyInfoCount()  { return m_props.size(); }

    wxString        GetBaseName( size_t index );
    EventInfo       GetEventInfo( size_t index );
    PropertyInfo    GetPropertyInfo( size_t index );

    bool            PropertyInfoExists ( const wxString &name );

private:
    void AddBaseName( const wxString &clsName ) { m_bases.Add( clsName ); }
    void AddEventInfo( EventInfo info )         { m_events.push_back( info ); }
    void AddPropertyInfo( PropertyInfo info )   { m_props.push_back( info ); }

    void AddAllowedChildType( AllowedChildType type )
                            { m_childTypes.push_back( type ); }

    void AddAllowedChildName( AllowedChildName name )
                            { m_childNames.push_back( name ); }

    AllowedChildType GetAllowedChildType( size_t index ) const;
    AllowedChildName GetAllowedChildName( size_t index ) const;

    size_t GetChildTypeCount() { return m_childTypes.size(); }
    size_t GetChildNameCount() { return m_childNames.size(); }

    wxString        m_name;
    wxString        m_desc;
    ClassType       m_type;
    wxArrayString   m_bases;
    EventInfos      m_events;
    PropertyInfos   m_props;

    AllowedChildTypes m_childTypes;
    AllowedChildNames m_childNames;

    friend class ClassInfoDB;
};
//*****************************************************************************
//  ClassInfoDB
//*****************************************************************************

class DLLIMPEXP_CORE ClassInfoDB
{
public:
    static ClassInfoDB *Get();
    static void Free();

    bool            ClassInfoExists( const wxString &name );
    ClassInfo       GetClassInfo   ( const wxString &name ) const;
    PropertyType    GetPropertyType( const wxString &name ) const;

private:
    ClassInfoDB();
    ~ClassInfoDB();

    ClassInfoDB( const ClassInfoDB & );
    ClassInfoDB& operator=( ClassInfoDB const & );

    bool            InitClassList( const wxString &path );
    void            InitPropertyTypes();
    void            Init();

    bool            LoadXML    ( const wxString &path );
    bool            CheckClass ( const wxString &name );
    void            Parse      ( wxXmlNode *node, bool recursively = false );

    EventInfo       DoGetEventInfo     ( wxXmlNode *eventNode );
    PropertyInfo    DoGetPropertyInfo  ( wxXmlNode *propertyNode );

    ClassInfoMap    m_classes;
    PropertyTypeMap m_types;
    wxArrayString   m_classList;

    static ClassInfoDB *ms_instance;
};

#endif //__CORE_OBJECT_DATABASE_H__
