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

#ifndef __WXGDCORE_DATABASE_H__
#define __WXGDCORE_DATABASE_H__

#include "core/defs.h"
#include <wx/arrstr.h>

class wxXmlNode;
class wxString;
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

    PropertyType GetType()         const { return m_type; }
    wxString     GetName()         const { return m_name; }
    wxString     GetLabel()        const { return m_label; }
    wxString     GetDefaultValue() const { return m_value; }
    wxString     GetDescription()  const { return m_desc; }

    void AddChild( const wxString &name, PropertyInfo info )
    {
        m_children.insert( PropertyInfoMap::value_type( name, info ) );
    }

    void SetChildren( PropertyInfoMap children ) { m_children = children; }

private:
    PropertyType    m_type;
    wxString        m_name, m_label, m_value, m_desc;
    PropertyInfoMap m_children;
};
//-----------------------------------------------------------------------------
//  EventInfoBase
//-----------------------------------------------------------------------------

class EventInfoBase
{
public:
    EventInfoBase( const wxString &classname ) : m_name( classname ) {}

    ~EventInfoBase() {}

    wxString GetClassName() const { return m_name; }

    void AddEventType( const wxString &typeName, const wxString &description )
    {
        m_types.insert( EventTypeMap::value_type( typeName, description ) );
    }

private:
    wxString     m_name;
    EventTypeMap m_types;
};
//-----------------------------------------------------------------------------
//  ClassInfoBase
//-----------------------------------------------------------------------------

class ClassInfoBase
{
public:
    ClassInfoBase( const wxString       &name,
                   const wxArrayString  &basenames,
                   const wxArrayString  &parents,
                   EventInfoMap         eventInfoMap,
                   PropertyInfoMap      propInfoMap )
                 : m_className( name ),
                   m_baseNames( basenames ), m_parents( parents ),
                   m_evtInfos( eventInfoMap ), m_propInfos( propInfoMap ) {}
    ~ClassInfoBase()
    {
        m_evtInfos.erase( m_evtInfos.begin(), m_evtInfos.end() );
        m_propInfos.erase( m_propInfos.begin(), m_propInfos.end() );
    }

    const wxString          GetClassName()  const { return m_className; }
    const wxArrayString     GetBaseNames()  const { return m_baseNames; }
    const wxArrayString     GetParents()    const { return m_parents; }
    const PropertyInfoMap   GetPropInfos()  const { return m_propInfos; }

private:
    wxString        m_className;
    wxArrayString   m_baseNames;
    wxArrayString   m_parents;
    EventInfoMap    m_evtInfos;
    PropertyInfoMap m_propInfos;
};
//-----------------------------------------------------------------------------
//  ClassInfoDataBase
//-----------------------------------------------------------------------------

class DLLIMPEXP_CORE ClassInfoDataBase
{
public:
    static ClassInfoDataBase *Get();
    void Free();

    const wxString  GetBasePath() const;
    ClassInfo       GetClassInfo( const wxString &className );
    PropertyType    GetPropertyType( const wxString &tagname ) const;

private:
    ClassInfoDataBase() { Init(); }
    ~ClassInfoDataBase() {}

    ClassInfoDataBase( const ClassInfoDataBase& );
    ClassInfoDataBase& operator=( ClassInfoDataBase const& );

    void Init();
    void InitPropertyTypes();
    bool ReadXML( const wxString &xmlpath );
    void Parse( wxXmlNode *node, bool recursively = false );

    EventInfoBase    *DoGetEventInfo( wxXmlNode *eventNode );
    PropertyInfoBase *DoGetPropertyInfo( wxXmlNode *propertyNode );

    typedef std::map< wxString, wxArrayString > ParentInfoMap;

    ClassInfoMap    m_classes;
    ParentInfoMap   m_infoMap;
    PropertyTypeMap m_types;

    static ClassInfoDataBase *ms_instance;
};

#endif //__WXGDCORE_DATABASE_H__
