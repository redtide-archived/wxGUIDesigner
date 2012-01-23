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

#ifndef __CORE_WIDGET_DATABASE_H__
#define __CORE_WIDGET_DATABASE_H__

#include "core/defs.h"

#include <utility>

#include <wx/arrstr.h>
#include <wx/string.h>

#include <wx/log.h>

enum WidgetType
{
    WIDGET_TYPE_DEFAULT = 0,
    WIDGET_TYPE_ABSTRACT,
    WIDGET_TYPE_ITEM,
    WIDGET_TYPE_SIZER,
    WIDGET_TYPE_TOPLEVEL
};

class wxXmlNode;
//-----------------------------------------------------------------------------
//  PropertyInfoBase
//-----------------------------------------------------------------------------
class PropertyInfoBase
{
public:
    PropertyInfoBase( PropertyType type,
                      const wxString &name, const wxString &label,
                      const wxString &defaultValue = wxEmptyString,
                      const wxString &description = wxEmptyString )
                    : m_type( type ), m_name( name ), m_label( label ),
                      m_value( defaultValue ), m_desc( description ) {}

    ~PropertyInfoBase()
    {
        m_children.erase( m_children.begin(), m_children.end() );
    }

    PropertyType GetType()         { return m_type; }
    wxString     GetName()         { return m_name; }
    wxString     GetLabel()        { return m_label; }
    wxString     GetDefaultValue() { return m_value; }
    wxString     GetDescription()  { return m_desc; }
    size_t       GetChildCount()   { return m_children.size(); }

    void AddChild( const wxString &name, PropertyInfo info )
    {
        m_children.insert
        ( std::map< wxString, PropertyInfo >::value_type( name, info ) );
    }

    void SetChildren( std::map< wxString, PropertyInfo > children )
    {
        m_children = children;
    }

private:
    PropertyType    m_type;
    wxString        m_name, m_label, m_value, m_desc;
    std::map< wxString, PropertyInfo > m_children;
};
//-----------------------------------------------------------------------------
//  EventInfoBase
//-----------------------------------------------------------------------------

class EventInfoBase
{
public:
    EventInfoBase( const wxString &name,
                   const wxString &description = wxEmptyString )
                 : m_name( name ), m_desc( description ) {}
    ~EventInfoBase() {}

    wxString GetClassName()         { return m_name; }
    wxString GetClassDescription()  { return m_desc; }
    size_t   GetTypeCount()         { return m_types.size(); }

    wxString GetTypeName( size_t eventTypeIndex )
    {
        if ( !eventTypeIndex >= m_types.size() )
            return m_types.at( eventTypeIndex ).first;

        return wxEmptyString;
    }

    wxString GetTypeDescription( size_t eventTypeIndex )
    {
        if ( !eventTypeIndex >= m_types.size() )
            return m_types.at( eventTypeIndex ).second;

        return wxEmptyString;
    }

    void AddType( const wxString &name, const wxString &description )
    {
        EventTypeInfo info = std::make_pair( name, description );
        m_types.push_back( info );
    }

private:
    wxString                     m_name; // event class name
    wxString                     m_desc; // event class description
    std::vector< EventTypeInfo > m_types;
};
//-----------------------------------------------------------------------------
//  WidgetInfoBase
//-----------------------------------------------------------------------------

class WidgetInfoBase
{
public:
    WidgetInfoBase( const wxString &classname,
                    WidgetType type = WIDGET_TYPE_DEFAULT )
                    : m_classname( classname ), m_type( type ) {}
    ~WidgetInfoBase();

    wxString GetClassName() const { return m_classname; }

    bool CanBeChildOf   ( const wxString &classname );
    bool IsKindOf       ( const wxString &classname );
    bool IsAbstract()   { return m_type == WIDGET_TYPE_ABSTRACT; }
    bool IsItem()       { return m_type == WIDGET_TYPE_ITEM; }
    bool IsSizer()      { return m_type == WIDGET_TYPE_SIZER; }
    bool IsTopLevel()   { return m_type == WIDGET_TYPE_TOPLEVEL; }

private:
    void AddBaseName    ( const wxString &name ) { m_bases.Add( name ); }
    void AddParentName  ( const wxString &name ) { m_parents.Add( name ); }

    void AddEventInfo   ( EventInfo    info );
    void AddPropertyInfo( PropertyInfo info );

    wxString        m_classname;
    WidgetType      m_type;
    wxArrayString   m_bases;
    wxArrayString   m_parents;

    std::map< wxString, EventInfo >    m_evtInfos;
    std::map< wxString, PropertyInfo > m_propInfos;

    friend class WidgetInfoDB;
};
//-----------------------------------------------------------------------------
//  WidgetInfoDB
//-----------------------------------------------------------------------------

class DLLIMPEXP_CORE WidgetInfoDB
{
public:
    static WidgetInfoDB *Get();
    void Free();

    WidgetInfo   GetClassInfo   ( const wxString &name );
    PropertyType GetPropertyType( const wxString &name ) const;

private:
    WidgetInfoDB() { Init(); }
    ~WidgetInfoDB() {}

    WidgetInfoDB( const WidgetInfoDB& );
    WidgetInfoDB& operator=( WidgetInfoDB const& );

    bool InitClassList( const wxString &path );
    void InitPropertyTypes();
    void Init();

    bool LoadXML( const wxString &path );
    bool CheckClass( const wxString &name );
    void Parse( wxXmlNode *node, bool recursively = false );

    EventInfo    DoGetEventInfo    ( wxXmlNode *eventNode );
    PropertyInfo DoGetPropertyInfo ( wxXmlNode *propertyNode );

    std::map< wxString, wxArrayString > m_parents;
    std::map< wxString, WidgetInfo >    m_classes;
    std::map< wxString, PropertyType >  m_types;

    wxArrayString m_classList;

    static WidgetInfoDB *ms_instance;
};

#endif //__CORE_WIDGET_DATABASE_H__
