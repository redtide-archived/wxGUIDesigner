///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/classinfo.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/23
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_RTTI_CLASSTREE_H__
#define __WXGUIDESIGNER_RTTI_CLASSTREE_H__

#include <string>
#include <vector>

#include <tr1/memory>
#include <tr1/unordered_map>

using std::string;

namespace RTTI
{
enum PropertyType
{
    ArrayString = 1,
    Bitmap,
    Bool,
    Category,
    Colour,
    Dimension,
    Double,
    Enum,
    Event,
    Float,
    Flag,
    Font,
    Int,
    Items,
    ListColFmt,
    Point,
    Size,
    String,
    Style,
    Text,
    URL
};

enum ClassType
{
    Root = 1,
    Widget,
    Container,
    Custom,
    Window = (Widget|Container|Custom),
    TopLevel,
    Layout,
    Sizer,
    Abstract,
    Item
};
//=============================================================================
// ClassTree
//
// Note: unique_ptr not available and scoped_ptr can't be used
//       inside STL containers
//=============================================================================
class EventInfoNode;
class EventTypeInfoNode;
class PropertyInfoNode;
class ChildInfoNode;
class ClassNode;

typedef std::tr1::shared_ptr< ChildInfoNode >                   ChildInfo;
typedef std::tr1::shared_ptr< EventInfoNode >                   EventInfo;
typedef std::tr1::shared_ptr< EventTypeInfoNode >               EventTypeInfo;
typedef std::tr1::shared_ptr< PropertyInfoNode >                PropertyInfo;
typedef std::tr1::shared_ptr< ClassNode >                       ClassInfo;

typedef std::vector< string >                              Strings;
typedef std::vector< ChildInfo >                                ChildInfos;
typedef std::vector< EventTypeInfo >                            EventTypeInfos;
typedef std::vector< EventInfo >                                EventInfos;
typedef std::vector< PropertyInfo >                             PropertyInfos;
typedef std::vector< ClassInfo >                                ClassInfos;

typedef std::pair< string, Strings >                       StringsPair;

typedef std::tr1::unordered_map< string, string >     StringMap;
typedef std::tr1::unordered_map< int, string >             TypeMap;
typedef std::tr1::unordered_map< string, Strings >         StringsMap;
typedef std::tr1::unordered_map< string, PropertyType >    PropertyTypeMap;
typedef std::tr1::unordered_map< ClassInfo, Strings >           ClassInfoMap;
//=============================================================================
// ChildInfoNode
//=============================================================================
class ChildInfoNode
{
public:
    ChildInfoNode( int          type    = 0,
                   const string &name   = "",
                   int          max     = 0,
                   bool         option  = false );
    ~ChildInfoNode();

    int     GetType()   const;
    string  GetName()   const;
    int     GetMax()    const;
    bool    IsOption()  const;
    bool    IsType()    const;

private:
    int     m_type;
    string  m_name;
    int     m_max;
    bool    m_option;
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
    EventTypeInfoNode ( const string &name,
                        const string &description = ""/*,
                        const string &macro       = ""*/ )
                                                    :
                                                    m_name( name ),
                                                    m_desc( description )/*,
                                                    m_macro( macro )*/ {}
    string GetName()          const { return m_name; }
    string GetDescription()   const { return m_desc; }
//  string GetMacroName()     const { return m_macro; }

private:
    string m_name;
    string m_desc;
//  string m_macro;
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
    EventInfoNode ( const string &name        = "",
                    const string &description = "" );
    ~EventInfoNode();

    string GetName()                            const;
    string GetDescription()                     const;
//------------------------------------------------------------------------------
// EventTypeInfos
//------------------------------------------------------------------------------
    string GetTypeName       ( size_t index )   const;
    string GetTypeDescription( size_t index )   const;
    size_t GetTypeCount()                       const;

private:
    void AddType( const string &name, const string &description );
    string          m_name,
                    m_desc;
    EventTypeInfos  m_types;
};
//=============================================================================
// PropertyInfoNode
//=============================================================================
class PropertyInfoNode
{
public:
    PropertyInfoNode  ( int         type            = 0,
                        const string &name          = "",
                        const string &label         = "",
                        const string &defValue      = "",
                        const string &description   = "");
    ~PropertyInfoNode();

    int             GetType()                   const;
    string          GetName()                   const;
    string          GetLabel()                  const;
    string          GetDefaultValue()           const;
    string          GetDescription()            const;
    size_t          GetChildCount()             const;

    PropertyInfo    GetChild( size_t index )    const;
    void            AddChild( PropertyInfo info );

private:
    int             m_type;
    string          m_name, m_label, m_value, m_desc;
    PropertyInfos   m_children;
};
//=============================================================================
// ClassNode
//=============================================================================
class ClassNode
{
public:
    ClassNode ( int             type        = 0,
                const string   &name        = "",
                const string   &description = "" );
    ~ClassNode();

    int             GetType()                                       const;
    string          GetName()                                       const;
    string          GetDescription()                                const;
//-----------------------------------------------------------------------------
// Inherited classes
//-----------------------------------------------------------------------------
    string          GetBaseName ( size_t index )                    const;
    size_t          GetBaseCount()                                  const;
    bool            IsA         ( const string &className )         const;
    bool            IsTypeOf    ( int type )                        const;
//-----------------------------------------------------------------------------
// PropertyInfos
//-----------------------------------------------------------------------------
    PropertyInfo    GetPropertyInfo     ( size_t index )            const;
    PropertyInfo    GetPropertyInfo     ( const string &name )      const;
    size_t          GetPropertyInfoCount()                          const;
//-----------------------------------------------------------------------------
// EventInfos
//-----------------------------------------------------------------------------
    EventInfo       GetEventInfo        ( size_t index )            const;
    size_t          GetEventInfoCount()                             const;
//-----------------------------------------------------------------------------
// ChildInfos
//-----------------------------------------------------------------------------
    ChildInfo       GetChildInfo        ( size_t index )            const;
    ChildInfo       GetChildInfo        ( const string &name )      const;
    ChildInfos      GetChildInfos()                                 const;
    size_t          GetChildInfoCount()                             const;

private:
    void            AddBaseName     ( const string &className );
    void            AddPropertyInfo ( PropertyInfo      propertyInfo );
    void            AddEventInfo    ( EventInfo         eventInfo );
    void            AddChildInfo    ( ChildInfo         childInfo );

    int             m_type;
    string          m_name,
                    m_desc;
    Strings         m_baseNames;
    ChildInfos      m_children;
    PropertyInfos   m_propInfos;
    EventInfos      m_eventInfos;
};
//=============================================================================
// RTTI
//=============================================================================
    void        Init();
    void        UnInit();

    string      GetDataBasePath();
    string      GetResourcePath();

    ClassInfo   GetClassInfo          ( const string &name );
    void        LoadClassesFromXML    ( const string &path );

    bool        IsChildInfoOkForClass ( const string &className,
                                        const string &childName,
                                        size_t      count );
    static Strings          baseNames;
    static StringsMap       classNameMap;
    static ClassInfoMap     baseInfoMap;
    static ClassInfoMap     classInfoMap;
    static PropertyTypeMap  propertyTypes;
}; // namespace RTTI

#endif //__WXGUIDESIGNER_RTTI_CLASSTREE_H__
