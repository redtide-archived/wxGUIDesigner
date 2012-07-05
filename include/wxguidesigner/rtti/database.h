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
                   bool            option    = false );
    ~ChildInfoNode();

    ClassType GetType()     const;
    wxString  GetName()     const;
    int       GetMax()      const;
    bool      IsOption()    const;
    bool      IsType()      const;

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
    EventInfoNode ( const wxString &name,
                    const wxString &description = wxEmptyString );
    ~EventInfoNode();

    wxString GetName()        const;
    wxString GetDescription() const;
//------------------------------------------------------------------------------
// EventTypeInfos
//------------------------------------------------------------------------------
    wxString GetTypeName       ( size_t index ) const;
    wxString GetTypeDescription( size_t index ) const;
    size_t   GetTypeCount()                     const;

private:
    void SetDescription( const wxString &description );
    void AddType( const wxString &name, const wxString &description );

    wxString        m_name;
    wxString        m_desc;
    EventTypeInfos  m_types;

    friend class ClassInfoDB;
};
//=============================================================================
// PropertyInfoNode
//=============================================================================
class PropertyInfoNode
{
public:
    PropertyInfoNode( PropertyType type,
                      const wxString &name, const wxString &label );
    ~PropertyInfoNode();

    PropertyType GetType()         const;
    wxString     GetName()         const;
    wxString     GetLabel()        const;
    wxString     GetDefaultValue() const;
    wxString     GetDescription()  const;
    size_t       GetChildCount()   const;

    PropertyInfo GetChild( size_t index );
    void         AddChild( PropertyInfo info );

private:
    void SetDefaultValue( const wxString &value );
    void SetDescription ( const wxString &description );

    PropertyInfos   m_children;
    PropertyType    m_type;
    wxString        m_name, m_label, m_value, m_desc;

    friend class ClassInfoDB;
};
//=============================================================================
// ClassNode
//=============================================================================
class ClassNode
{
public:
    ClassNode( const wxString &className, ClassType type = CLASS_TYPE_WIDGET );
    ~ClassNode();

    wxString        GetName()        const;
    wxString        GetDescription() const;
    ClassType       GetType()        const;

    void            SetDescription( const wxString &description );
//-----------------------------------------------------------------------------
// Inherited classes
//-----------------------------------------------------------------------------
    ClassInfo       GetBaseInfo ( size_t index )              const;
    wxString        GetBaseName ( size_t index )              const;
    size_t          GetBaseCount()                            const;
    bool            IsA         ( const wxString &className ) const;
    bool            IsTypeOf    ( ClassType type )            const;
//-----------------------------------------------------------------------------
// PropertyInfos
//-----------------------------------------------------------------------------
    PropertyInfo    GetPropertyInfo     ( size_t index )         const;
    PropertyInfo    GetPropertyInfo     ( const wxString &name ) const;
    bool            PropertyInfoExists  ( const wxString &name ) const;
    size_t          GetPropertyInfoCount()                       const;
//-----------------------------------------------------------------------------
// EventInfos
//-----------------------------------------------------------------------------
    EventInfo       GetEventInfo( size_t index ) const;
    size_t          GetEventInfoCount()          const;
//-----------------------------------------------------------------------------
// ChildInfos
//-----------------------------------------------------------------------------
    ChildInfo       GetChildInfo( size_t index )                        const;
    ChildInfo       GetChildInfo( const wxString &name )                const;
    size_t          GetChildInfoCount()                                 const;
    bool            IsChildInfoOk( const wxString &name, size_t count ) const;

private:
    void            AddBaseInfo    ( ClassInfo    classInfo );
    void            AddPropertyInfo( PropertyInfo propertyInfo );
    void            AddEventInfo   ( EventInfo    eventInfo );
    void            AddChildInfo   ( ChildInfo    childInfo );

    wxString        m_name;
    wxString        m_desc;
    ClassType       m_type;
    ClassInfos      m_baseInfos;
    ChildInfos      m_children;
    EventInfos      m_eventInfos;
    PropertyInfos   m_propInfos;

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

    // Class name and type (e.g. wxFrame, toplevel )
    typedef map< wxString, wxString > ClassNameMap;

    ClassInfoMap    m_classInfos;
    wxArrayString   m_baseNames;  // e.g. 'wxTopLevelWindow'
    wxArrayString   m_classNames; // e.g. 'wxFrame'
    wxArrayString   m_classTypes; // e.g. 'toplevel'

    // TODO: unordered_map needed

    PropertyTypeMap m_propTypes;

    static ClassInfoDB *ms_instance;
};

#endif //__WXGUIDESIGNER_RTTI_DATABASE_H__
