///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/classinfo.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/29
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <cctype>
#include <algorithm>

#include <wx/app.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>

#include <wx/log.h>

#include "wxguidesigner/rtti/classinfo.h"
#include "wxguidesigner/rtti/convert.h"
//=============================================================================
// ChildInfoNode
//=============================================================================
RTTI::ChildInfoNode::ChildInfoNode( int type,   const string &name,
                                    int max,    bool option )
:
m_type  ( type ),
m_name  ( name ),
m_max   ( max ),
m_option( option )
{
}

RTTI::ChildInfoNode::~ChildInfoNode()
{
}

int RTTI::ChildInfoNode::GetType() const
{
    return m_type;
}

string RTTI::ChildInfoNode::GetName() const
{
    return m_name;
}

int RTTI::ChildInfoNode::GetMax() const
{
    return m_max;
}

bool RTTI::ChildInfoNode::IsOption() const
{
    return m_option;
}

bool RTTI::ChildInfoNode::IsType() const
{
    return m_name.empty();
}
//=============================================================================
// EventInfoNode
//=============================================================================
RTTI::EventInfoNode::EventInfoNode( const string &name,
                                    const string &description )
:
m_name( name ),
m_desc( description )
{
}

RTTI::EventInfoNode::~EventInfoNode()
{
    m_types.clear();
}

string RTTI::EventInfoNode::GetName() const
{
    return m_name;
}

string RTTI::EventInfoNode::GetDescription() const
{
    return m_desc;
}
//-----------------------------------------------------------------------------
// EventTypeInfos
//-----------------------------------------------------------------------------
void RTTI::EventInfoNode::AddType(const string &name,
                                        const string &description)
{
    EventTypeInfo info( new EventTypeInfoNode( name, description ) );
    m_types.push_back( info );
}

string RTTI::EventInfoNode::GetTypeName( size_t index ) const
{
    if( index < m_types.size() )
        return m_types.at( index )->GetName();

    return "";
}

string RTTI::EventInfoNode::GetTypeDescription( size_t index ) const
{
    if( index < m_types.size() )
        return m_types.at( index )->GetDescription();

    return "";
}

size_t RTTI::EventInfoNode::GetTypeCount() const
{
    return m_types.size();
}
//=============================================================================
//  PropertyInfoNode
//=============================================================================
RTTI::PropertyInfoNode::PropertyInfoNode  ( int                 type,
                                            const string   &name,
                                            const string   &label,
                                            const string   &defValue,
                                            const string   &description )
:
m_type  ( type ),
m_name  ( name ),
m_label ( label ),
m_value ( defValue ),
m_desc  ( description )
{
}

RTTI::PropertyInfoNode::~PropertyInfoNode()
{
    m_children.clear();
}

int RTTI::PropertyInfoNode::GetType() const
{
    return m_type;
}

string RTTI::PropertyInfoNode::GetName() const
{
    return m_name;
}

string RTTI::PropertyInfoNode::GetLabel() const
{
    return m_label;
}

string RTTI::PropertyInfoNode::GetDefaultValue() const
{
    return m_value;
}

string RTTI::PropertyInfoNode::GetDescription() const
{
    return m_desc;
}

size_t RTTI::PropertyInfoNode::GetChildCount() const
{
    return m_children.size();
}

void RTTI::PropertyInfoNode::AddChild( PropertyInfo info )
{
    m_children.push_back( info );
}

RTTI::PropertyInfo RTTI::PropertyInfoNode::GetChild( size_t index ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return PropertyInfo();
}
//=============================================================================
// ClassNode
//=============================================================================
RTTI::ClassNode::ClassNode( int type, const string &name,
                                      const string &description )
:
m_type( type ),
m_name( name ),
m_desc( description )
{
}

RTTI::ClassNode::~ClassNode()
{
    m_eventInfos.clear();
    m_propInfos.clear();
    m_children.clear();
}
string RTTI::ClassNode::GetName() const
{
    return m_name;
}

string RTTI::ClassNode::GetDescription() const
{
    return m_desc;
}

int RTTI::ClassNode::GetType() const
{
    return m_type;
}
//-----------------------------------------------------------------------------
// Inherited classes
//-----------------------------------------------------------------------------
string RTTI::ClassNode::GetBaseName( size_t index ) const
{
    if( index < m_baseNames.size() )
        return m_baseNames.at( index );

    return "";
}

size_t RTTI::ClassNode::GetBaseCount() const
{
    return m_baseNames.size();
}

void RTTI::ClassNode::AddBaseName( const string &className )
{
    if( className.empty() )
        return;

    m_baseNames.push_back( className );
}

bool RTTI::ClassNode::IsA( const string &className ) const
{
    if( m_name == className )
        return true;

    for( Strings::const_iterator it = m_baseNames.begin();
                                it != m_baseNames.end(); ++it )
    {
        if( *it == className )
            return true;

        // Recursively check a level up
        ClassInfo baseInfo = RTTI::GetClassInfo( *it );

        if( baseInfo && baseInfo->IsA( className ) )
            return true;
    }

    return false;
}

bool RTTI::ClassNode::IsTypeOf( int classType ) const
{
    return( m_type == classType );
}
//-----------------------------------------------------------------------------
// PropertyInfos
//-----------------------------------------------------------------------------
void RTTI::ClassNode::AddPropertyInfo( PropertyInfo propertyInfo )
{
    if( !propertyInfo )
        return;

    m_propInfos.push_back( propertyInfo );
}

RTTI::PropertyInfo RTTI::ClassNode::GetPropertyInfo( size_t index ) const
{
    if( index < m_propInfos.size() )
        return m_propInfos.at( index );

    return PropertyInfo();
}

RTTI::PropertyInfo RTTI::ClassNode::GetPropertyInfo( const string &name ) const
{
    for( PropertyInfos::const_iterator it = m_propInfos.begin();
                                    it != m_propInfos.end(); ++it )
    {
        PropertyInfo propertyInfo = *it;
        if( propertyInfo->GetName() == name )
            return propertyInfo;
    }

    return PropertyInfo();
}

size_t RTTI::ClassNode::GetPropertyInfoCount() const
{
    return m_propInfos.size();
}
//-----------------------------------------------------------------------------
// EventInfos
//-----------------------------------------------------------------------------
void RTTI::ClassNode::AddEventInfo( EventInfo eventInfo )
{
    m_eventInfos.push_back( eventInfo );
}

RTTI::EventInfo RTTI::ClassNode::GetEventInfo( size_t index ) const
{
    if( index < m_eventInfos.size() )
        return m_eventInfos.at( index );

    return EventInfo();
}

size_t RTTI::ClassNode::GetEventInfoCount() const
{
    return m_eventInfos.size();
}
//-----------------------------------------------------------------------------
// ChildInfos
//-----------------------------------------------------------------------------
void RTTI::ClassNode::AddChildInfo( ChildInfo childInfo )
{
    m_children.push_back( childInfo );
}

RTTI::ChildInfo RTTI::ClassNode::GetChildInfo( size_t index ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return ChildInfo();
}

RTTI::ChildInfo RTTI::ClassNode::GetChildInfo( const string &name ) const
{
    for( ChildInfos::const_iterator it = m_children.begin();
                                        it != m_children.end(); ++it )
    {
        ChildInfo childInfo = *it;
        if( childInfo->GetName() == name )
            return childInfo;
    }

    return ChildInfo();
}

size_t RTTI::ClassNode::GetChildInfoCount() const
{
    return m_children.size();
}
//=============================================================================
// RTTI
//=============================================================================
void RTTI::Init()
{
    wxString classPath = GetDataBasePath() + "classes";

    if( !wxDirExists( classPath ) )
        return;
//-----------------------------------------------------------------------------
// Init Property Types
//-----------------------------------------------------------------------------
    propertyTypes.insert( PropertyTypeMap::value_type( "arraystring", ArrayString ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "bitmap",    Bitmap      ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "bool",      Bool        ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "category",  Category    ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "colour",    Colour      ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "dimension", Dimension   ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "double",    Double      ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "event",     Event       ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "enum",      Enum        ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "float",     Float       ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "flag",      Flag        ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "font",      Font        ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "listcolfmt", ListColFmt ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "items",     Items       ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "int",       Int         ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "point",     Point       ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "size",      Size        ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "string",    String      ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "style",     Style       ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "exstyle",   Style       ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "text",      Text        ) );
    propertyTypes.insert( PropertyTypeMap::value_type( "url",       URL         ) );
//-----------------------------------------------------------------------------
// Init Class List
//-----------------------------------------------------------------------------
    wxString      listPath = classPath + ".xml";
    wxXmlDocument doc;
    if( !doc.Load( listPath ) || (doc.GetRoot()->GetName() != "classes") )
        return;


    // <class> --> <abstract> or concrete type
    wxXmlNode *classTypeNode = doc.GetRoot()->GetChildren();
    while( classTypeNode )
    {
        Strings classNames;

        string classType  = classTypeNode->GetName().ToStdString();
        wxXmlNode   *classNode = classTypeNode->GetChildren();

        while( classNode )
        {
            string nodeName  = classNode->GetName().ToStdString();
            if( nodeName == "name" )
            {
                string className =
                            classNode->GetNodeContent().ToStdString();

                if( classType == "abstract" )
                {
                    baseNames.push_back( className );
//                  wxLogDebug("Abstract class: %s", className );
                }
                else
                {
                    classNames.push_back( className );
//                  wxLogDebug("Concrete class: %s", className );
                }
            }

            classNode = classNode->GetNext();
        }

        if( !classNames.empty() )
        {
            StringsPair category( classType, classNames );
            classNameMap.insert( category );
        }

        classTypeNode = classTypeNode->GetNext();
    }
//-----------------------------------------------------------------------------
// Load Classes
//-----------------------------------------------------------------------------
    for( Strings::iterator iter = baseNames.begin();
                            iter != baseNames.end(); ++iter )
    {
        string xmlFile = *iter;
        transform( xmlFile.begin(), xmlFile.end(), xmlFile.begin(), tolower );
        xmlFile.append(".xml");
        xmlFile.erase( 0, 1 );

        wxString xmlFilePath = classPath + wxFILE_SEP_PATH + "abstract" +
                                            wxFILE_SEP_PATH + xmlFile;
        wxFileName xmlFileName( xmlFilePath ); 

        if( xmlFileName.FileExists() )
        {
            if(!xmlFileName.IsAbsolute())
                xmlFileName.MakeAbsolute();

            LoadClassesFromXML( xmlFileName.GetFullPath().ToStdString() );
        }
//      else
//          wxLogError("Path %s not found", xmlFilePath);
    }

    for( StringsMap::iterator iter = classNameMap.begin();
                            iter != classNameMap.end(); ++iter )
    {
        string category = (*iter).first;
        Strings     classes  = (*iter).second;
        wxString    path = classPath + wxFILE_SEP_PATH + category +
                                        wxFILE_SEP_PATH;
        wxDir categoryDir( path );

        if( categoryDir.IsOpened() )
        {
            for( Strings::iterator i = classes.begin(); i != classes.end(); ++i )
            {
                string xmlFile = *i;
                transform( xmlFile.begin(), xmlFile.end(), xmlFile.begin(), tolower );
                xmlFile.append(".xml");
                xmlFile.erase( 0, 1 );

                wxString xmlFilePath = path + xmlFile;
                wxFileName xmlFileName( xmlFilePath ); 

                if( xmlFileName.FileExists() )
                {
                    if(!xmlFileName.IsAbsolute())
                        xmlFileName.MakeAbsolute();

                    LoadClassesFromXML( xmlFileName.GetFullPath().ToStdString() );
                }
//              else
//                  wxLogError("Path %s not found", xmlFilePath);
            }
        }
    }

//  Since all classes are now loaded, add 'inherited' base properties/events to each one
/*
    for( ClassInfoMap::iterator it = baseInfoMap.begin();
                                it != baseInfoMap.end(); ++it )
    {
        ClassInfo   className = (*it).first;
        Strings     baseInfos = (*it).second;

        for( Strings::iterator iter = baseInfos.begin();
                                iter != baseInfos.end(); ++iter )
        {
            ClassInfo baseInfo = GetClassInfo( *iter );
            if( baseInfo )
            {
                classInfo->AddBaseInfo( baseInfo );
                // Add baseInfo's inherited classes informations
                for( size_t i = 0; i < baseInfo->GetBaseCount(); i++ )
                {
                    ClassInfo baseParentInfo = baseInfo->GetBaseInfo(i);
                    AddBaseInfos( classInfo, baseParentInfo );
                    wxLogMessage( "%s: Added base %s", classInfo->GetName(),
                                                        baseParentInfo->GetName() );
                }
            }
        }
    }
*/
}

void RTTI::UnInit()
{
/*
    for( ClassInfoMap::iterator it = classInfoMap.begin();
                                it != classInfoMap.end(); ++it )
        it->second = ClassInfo();
*/
    classInfoMap.clear();
/*
    for( ClassInfoMap::iterator it = baseInfoMap.begin();
                                it != baseInfoMap.end(); ++it )
        it->second = ClassInfo();
*/
    baseInfoMap.clear();
}

RTTI::ClassInfo RTTI::GetClassInfo( const string &name )
{
/*
    ClassInfoMap::iterator it = classInfoMap.find( name );

    if( it != classInfoMap.end() )
        return it->second;
*/
    return ClassInfo();
}

void RTTI::LoadClassesFromXML( const string &path )
{
    
}

bool RTTI::IsChildInfoOkForClass  ( const string   &className,
                                    const string   &childName,
                                    size_t              count )
{
    // TODO: IsOption check
    ClassInfo classInfo = GetClassInfo( className );
    ClassInfo childInfo = GetClassInfo( childName );

    if( !classInfo || !childInfo )
        return false;

    ChildInfos children = classInfo->GetChildInfos();

    for( ChildInfos::const_iterator it = children.begin();
                                    it != children.end(); ++it )
    {
        int  childMax  = (*it)->GetMax();
        bool isAllowed = ((childMax == -1) || (childMax > (int)count));

        if( childInfo->IsA( (*it)->GetName() ) && isAllowed )
            return true;
    }

    return false;
}

string RTTI::GetDataBasePath()
{
    wxString path = GetResourcePath() + "db" + wxFILE_SEP_PATH;
    return path.ToStdString();
}

string RTTI::GetResourcePath()
{
    wxString path =
#ifdef __WXMSW__
    wxStandardPaths::Get().GetDataDir() + wxFILE_SEP_PATH;
#else
    wxStandardPaths::Get().GetResourcesDir().BeforeLast( wxFILE_SEP_PATH ) +
                                            wxFILE_SEP_PATH + "wxguidesigner" +
                                            wxFILE_SEP_PATH;
#endif
    return path.ToStdString();
}
