///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/database.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/23
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_RTTI_DATABASE_H__
#define __WXGUIDESIGNER_RTTI_DATABASE_H__

#include "wxguidesigner/defs.h"

#include <wx/arrstr.h>
#include <wx/string.h>

#include <wx/log.h>

class wxXmlNode;

using namespace std;
//=============================================================================
// ChildInfoNode
//=============================================================================
class ChildInfoNode
{
public:
    ChildInfoNode( ClassType       type      = CLASS_TYPE_UNKNOWN,
                   const wxString &name      = wxEmptyString,
                   int             max       = 0,
                   bool            option    = false )
                   :    m_type( type ),
                        m_name( name ),
                        m_max( max ),
                        m_option( option ) {}
    ~ChildInfoNode() {}

    ClassType GetType()     const { return m_type; }
    wxString  GetName()     const { return m_name; }
    int       GetMax()      const { return m_max;  }
    bool      IsOption()    const { return m_option; }
    bool      IsType()      const { return m_name.empty(); }

private:
    ClassType m_type;
    wxString  m_name;
    int       m_max;
    bool      m_option;
};
//=============================================================================
// EventTypeInfoNode
//
// Stores informations about a single wxEventType.
//
// type name        = "wxEVT_CLOSE_WINDOW"
//      macro       = "EVT_CLOSE"
//      description = ...
//=============================================================================
class EventTypeInfoNode
{
public:
    EventTypeInfoNode( const wxString &name,
                        const wxString &description = wxEmptyString/*,
                        const wxString &macro       = wxEmptyString*/ )
                                                    :
                                                    m_name( name ),
                                                    m_desc( description )/*,
                                                    m_macro( macro )*/ {}
    wxString GetName()          const { return m_name; }
    wxString GetDescription()   const { return m_desc; }
//  wxString GetMacroName()     const { return m_macro; }

private:
    wxString m_name;
    wxString m_desc;
//  wxString m_macro;
};
//=============================================================================
// EventInfoNode
//
// Stores informations about a wxEvent class and its own wxEventType(s).
//
// class name        = "wxCloseEvent"
//       description = ...
//=============================================================================
class EventInfoNode
{
public:
    EventInfoNode( const wxString &name,
                   const wxString &description = wxEmptyString )
                                : m_name( name ),
                                  m_desc( description ) {}
    ~EventInfoNode()
    {
        m_evtTypes.clear();
    }

    wxString GetName()          const { return m_name; }
    wxString GetDescription()   const { return m_desc; }

    // EventTypeInfos

    wxString GetTypeName( size_t index ) const
    {
        if( index < m_evtTypes.size() )
            return m_evtTypes.at( index )->GetName();

        return wxEmptyString;
    }

    wxString GetTypeDescription( size_t index ) const
    {
        if( index < m_evtTypes.size() )
            return m_evtTypes.at( index )->GetDescription();

        return wxEmptyString;
    }

    size_t GetTypeCount() const { return m_evtTypes.size(); }

private:
    friend class ClassInfoDB;

    void SetDescription( const wxString &description ) { m_desc = description; }

    void AddType( const wxString &name, const wxString &description )
    {
        EventTypeInfo info( new EventTypeInfoNode( name, description ) );
        m_evtTypes.push_back( info );
    }

    wxString        m_name;
    wxString        m_desc;
    EventTypeInfos  m_evtTypes;
};
//=============================================================================
// PropertyInfoNode
//=============================================================================
class PropertyInfoNode
{
public:
    PropertyInfoNode( PropertyType   type,
                      const wxString &name,
                      const wxString &label );

    ~PropertyInfoNode();

    PropertyType GetType()         const { return m_type; }
    wxString     GetName()         const { return m_name; }
    wxString     GetLabel()        const { return m_label; }

    wxString     GetDefaultValue() const { return m_value; }
    wxString     GetDescription()  const { return m_desc; }
    size_t       GetChildCount()   const { return m_children.size(); }

    PropertyInfo GetChild( size_t index );
    void         AddChild( PropertyInfo info );

private:
    friend class ClassInfoDB;

    void SetDefaultValue( const wxString &value )     { m_value = value; }
    void SetDescription( const wxString &description ){ m_desc  = description; }

    PropertyInfos   m_children;
    PropertyType    m_type;
    wxString        m_name, m_label, m_value, m_desc;
};
//=============================================================================
// ClassNode
//=============================================================================
class ClassNode
{
public:
    ClassNode( const wxString &className, ClassType type = CLASS_TYPE_WIDGET );
    ~ClassNode();

    wxString        GetName()        const { return m_name; }
    wxString        GetDescription() const { return m_desc; }
    ClassType       GetType()        const { return m_type; }
//-----------------------------------------------------------------------------
// Inherited classes
//-----------------------------------------------------------------------------
    ClassInfo       GetBaseInfo( size_t index );
    wxString        GetBaseName( size_t index );
    size_t          GetBaseCount();
    bool            IsKindOf( const wxString &className );
    bool            IsTypeOf( ClassType type );
//-----------------------------------------------------------------------------
// PropertyInfos
//-----------------------------------------------------------------------------
    PropertyInfo    GetPropertyInfo( size_t index );
    PropertyInfo    GetPropertyInfo( const wxString &name );
    size_t          GetPropertyInfoCount()  { return m_props.size(); }
    bool            PropertyInfoExists( const wxString &name );
//-----------------------------------------------------------------------------
// EventInfos
//-----------------------------------------------------------------------------
    EventInfo       GetEventInfo( size_t index );
    size_t          GetEventInfoCount()     { return m_events.size(); }
//-----------------------------------------------------------------------------
// ChildInfos
//-----------------------------------------------------------------------------
    ChildInfo       GetChildInfo( size_t index );
    size_t          GetChildInfoCount()     { return m_children.size(); }
    bool            IsChildInfoOk( const wxString &name, size_t count );

private:
    void AddBaseInfo    ( ClassInfo info );
    void AddChildInfo   ( ChildInfo info )    { m_children.push_back( info ); }
    void AddEventInfo   ( EventInfo info )    { m_events.push_back( info ); }
    void AddPropertyInfo( PropertyInfo info ) { m_props.push_back( info ); }

    wxString        m_name;
    wxString        m_desc;
    ClassType       m_type;
    ClassInfos      m_baseInfos;
    ChildInfos      m_children;
    EventInfos      m_events;
    PropertyInfos   m_props;

    friend class ClassInfoDB;
};
//=============================================================================
// ClassInfoDB
//=============================================================================
class DLLIMPEXP_WXGUIDESIGNER ClassInfoDB
{
public:
    static ClassInfoDB *Get();
    static void         Free();

    bool            ClassInfoExists ( const wxString &name );
    ClassInfo       GetClassInfo    ( const wxString &name ) const;
    PropertyType    GetPropertyType ( const wxString &name ) const;

private:
    ClassInfoDB();
    ~ClassInfoDB();

    ClassInfoDB( const ClassInfoDB & );
    ClassInfoDB& operator=( ClassInfoDB const & );

    bool            InitClassList( const wxString &path );
    void            InitPropertyTypes();
    void            Init();

    void            LoadClasses( const wxString &category );
    bool            LoadXML    ( const wxString &category );
    bool            CheckClass ( const wxString &name );
    void            Parse      ( wxXmlNode *node, bool recursively = false );

    void            AddBaseInfos        ( ClassInfo classInfo,
                                          ClassInfo baseInfo );

    ChildInfo       DoGetChildInfo      ( wxXmlNode *childNode );
    EventInfo       DoGetEventInfo      ( wxXmlNode *eventNode );
    PropertyInfo    DoGetPropertyInfo   ( wxXmlNode *propertyNode );

    ClassType       ClassTypeFromString ( const wxString &value ) const;

    ClassInfoMap    m_classes;
    PropertyTypeMap m_propTypes;

    typedef map< wxString, wxString > ClassNameMap;

    wxArrayString   m_baseList;
    ClassNameMap    m_classMap;

    static ClassInfoDB *ms_instance;
};

#endif //__WXGUIDESIGNER_RTTI_DATABASE_H__
