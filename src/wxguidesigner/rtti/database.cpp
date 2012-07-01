///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/database.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/29
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/utils.h"

#include <wx/app.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>

#include <wx/log.h>

using namespace std;
//=============================================================================
//  PropertyInfoNode
//=============================================================================
PropertyInfoNode::PropertyInfoNode( PropertyType type, const wxString &name,
                                    const wxString &label )
:
m_type  ( type ),
m_name  ( name ),
m_label ( label ),
m_value ( wxEmptyString ),
m_desc  ( wxEmptyString )
{
}

PropertyInfoNode::~PropertyInfoNode()
{
    m_children.clear();
}

void PropertyInfoNode::AddChild( PropertyInfo info )
{
    m_children.push_back( info );
}

PropertyInfo PropertyInfoNode::GetChild( size_t index )
{
    if( index < m_children.size() )
        return m_children.at( index );

    return PropertyInfo();
}
//=============================================================================
//  ClassNode
//=============================================================================
ClassNode::ClassNode( const wxString &className, ClassType type )
:
m_name( className ),
m_type( type )
{
}

ClassNode::~ClassNode()
{
    m_events.clear();
    m_props.clear();
    m_children.clear();
    m_baseInfos.clear();
}
//=============================================================================
// Inherited classes
//=============================================================================
ClassInfo ClassNode::GetBaseInfo( size_t index )
{
    if( index < GetBaseCount() )
        return m_baseInfos.at( index );

    return ClassInfo();
}

wxString ClassNode::GetBaseName( size_t index )
{
    if( index < GetBaseCount() )
        return m_baseInfos.at( index )->GetName();

    return wxEmptyString;
}

size_t ClassNode::GetBaseCount()
{
    return m_baseInfos.size();
}

void ClassNode::AddBaseInfo( ClassInfo info )
{
    m_baseInfos.push_back( info );
}

bool ClassNode::IsKindOf( const wxString &name )
{
    for( size_t i = 0; i < GetBaseCount(); i++ )
        if( GetBaseName(i) == name )
            return true;

    return false;
}

bool ClassNode::IsTypeOf( ClassType type )
{
    return( m_type == type );
}

ChildInfo ClassNode::GetChildInfo( size_t index )
{
    if( index < m_children.size() )
        return m_children.at( index );

    return ChildInfo();
}

bool ClassNode::IsChildInfoOk( const wxString &name, size_t count )
{
    // Child class validation check
    ClassInfo childClsInfo = ClassInfoDB::Get()->GetClassInfo( name );
    if( !childClsInfo )
        return false;

    for( size_t i = 0; i < m_children.size(); i++ )
    {
        // Check type / name
        ChildInfo childInfo = m_children.at( i );
        if( ( childInfo->GetType() == childClsInfo->GetType() ) ||
            ( childInfo->GetName() == childClsInfo->GetName() ) )
        {
            // Check maximum allowed instances
            if( ( childInfo->GetMax() == -1 ) ||
                ( childInfo->GetMax() > ( int )count ) )
            {
                // TODO: Check option
                return true;
            }
        }
    }

    return false;
}

EventInfo ClassNode::GetEventInfo( size_t index )
{
    if( index < m_events.size() )
        return m_events.at( index );

    return EventInfo();
}

PropertyInfo ClassNode::GetPropertyInfo( size_t index )
{
    if( index < m_props.size() )
        return m_props.at( index );

    return PropertyInfo();
}

PropertyInfo ClassNode::GetPropertyInfo( const wxString &name )
{
    for( PropertyInfos::iterator it = m_props.begin(); it != m_props.end(); ++it )
    {
        PropertyInfo propertyInfo = *it;
        if( propertyInfo->GetName() == name )
            return propertyInfo;
    }

    return PropertyInfo();
}

bool ClassNode::PropertyInfoExists( const wxString &name )
{
    for( PropertyInfos::iterator it = m_props.begin(); it != m_props.end(); ++it )
    {
        if( (*it)->GetName() == name )
            return true;
    }

    return false;
}
//=============================================================================
//  ClassInfoDB
//=============================================================================
ClassInfoDB::ClassInfoDB()
{
    Init();
}

ClassInfoDB::~ClassInfoDB()
{
    m_classes.clear();
    m_classMap.clear();
    m_propTypes.clear();
    m_baseList.Clear();
}

ClassInfoDB *ClassInfoDB::ms_instance = NULL;

ClassInfoDB *ClassInfoDB::Get()
{
    if( !ms_instance )
        ms_instance = new ClassInfoDB;

    return ms_instance;
}

void ClassInfoDB::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void ClassInfoDB::InitPropertyTypes()
{
    m_propTypes.insert( PropertyTypeMap::value_type( "arraystring", PROPERTY_TYPE_ARRAYSTRING ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "bitmap",      PROPERTY_TYPE_BITMAP ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "bool",        PROPERTY_TYPE_BOOL ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "category",    PROPERTY_TYPE_CATEGORY ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "colour",      PROPERTY_TYPE_COLOUR ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "dimension",   PROPERTY_TYPE_DIMENSION ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "double",      PROPERTY_TYPE_DOUBLE ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "event",       PROPERTY_TYPE_EVENT ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "enum",        PROPERTY_TYPE_ENUM ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "float",       PROPERTY_TYPE_FLOAT ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "flag",        PROPERTY_TYPE_FLAG ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "font",        PROPERTY_TYPE_FONT ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "listcolfmt",  PROPERTY_TYPE_LISTCOLFMT ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "items",       PROPERTY_TYPE_ITEMS ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "int",         PROPERTY_TYPE_INT ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "name",        PROPERTY_TYPE_NAME ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "point",       PROPERTY_TYPE_POINT ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "size",        PROPERTY_TYPE_SIZE ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "string",      PROPERTY_TYPE_STRING ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "style",       PROPERTY_TYPE_STYLE ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "exstyle",     PROPERTY_TYPE_STYLE ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "text",        PROPERTY_TYPE_TEXT ) );
    m_propTypes.insert( PropertyTypeMap::value_type( "url",         PROPERTY_TYPE_URL ) );
}

bool ClassInfoDB::InitClassList( const wxString &category )
{
    wxString path = GetDataBasePath() + wxFILE_SEP_PATH + category + ".xml";

    wxXmlDocument doc;
    if( !doc.Load( path ) )
        return false;

    wxXmlNode *node = doc.GetRoot()->GetChildren();
    while( node )
    {
        wxString nodeName = node->GetName();

        if( category == "controlbase" )
        {
            if( nodeName == "item" )
            {
                wxString className = node->GetNodeContent();

                m_baseList.Add( className );
//              wxLogDebug("Loaded %s", className );
            }
        }
        else if( category == "controls" )
        {
            wxXmlNode *itemNode = node->GetChildren();

            while( itemNode )
            {
                if( itemNode->GetName() == "item" )
                {
                    wxString className = itemNode->GetNodeContent();
                    m_classMap.insert
                        ( ClassNameMap::value_type( className, nodeName ) );

//                  wxLogDebug("Loaded %s", className );
                }

                itemNode = itemNode->GetNext();
            }
        }

        node = node->GetNext();
    }
    if( category == "controlbase" )
        return( !m_baseList.empty() );
    else if( category == "controls" )
        return( !m_classMap.empty() );

    return false;
}

void ClassInfoDB::Init()
{
    InitPropertyTypes();

    if( !InitClassList("controlbase") || !InitClassList("controls") )
        return;

    LoadClasses("controlbase");
    LoadClasses("controls");
}

void ClassInfoDB::LoadClasses( const wxString &category )
{
    wxString path = GetDataBasePath() + wxFILE_SEP_PATH + category;

    if( !wxDirExists( path ) )
        return;

    wxDir dbDir( path );
    if( !dbDir.IsOpened() )
        return;

    if( category == "controlbase" )
    {
        for( size_t i = 0; i < m_baseList.GetCount(); i++ )
        {
            wxString xmlFile = m_baseList.Item(i).Lower() + ".xml";
            xmlFile.Replace("wx", "");
            wxString xmlFilePath = path + wxFILE_SEP_PATH + xmlFile;
            wxFileName xmlFileName( xmlFilePath ); 

            if( xmlFileName.FileExists() )
            {
                if(!xmlFileName.IsAbsolute())
                    xmlFileName.MakeAbsolute();

                LoadXML( xmlFileName.GetFullPath() );
            }
//          else
//              wxLogDebug("Path %s not found", xmlFilePath);
        }
    }
    else if( category == "controls" )
    {
        for( ClassNameMap::iterator it = m_classMap.begin();
                                    it != m_classMap.end(); ++it )
        {
            wxString className = (*it).first;
            wxString classType = (*it).second;
            wxString classPath = path + wxFILE_SEP_PATH + classType +
                                                        wxFILE_SEP_PATH;
            wxDir classTypeDir( classPath );

            if( classTypeDir.IsOpened() )
            {
                wxString xmlFile = className.Lower() + ".xml";
                xmlFile.Replace("wx", "");
                wxString xmlFilePath = classPath + xmlFile;
                wxFileName xmlFileName( xmlFilePath ); 

                if( xmlFileName.FileExists() )
                {
                    if(!xmlFileName.IsAbsolute())
                        xmlFileName.MakeAbsolute();

                    LoadXML( xmlFileName.GetFullPath() );
                }
//              else
//                  wxLogDebug("Path %s not found", xmlFilePath);
            }
        }
    }
}

bool ClassInfoDB::LoadXML( const wxString &path )
{
    // Check document
    wxXmlDocument doc;
    if( !doc.Load( path ) )
        return false;

    // If the 'class' element isn't the root node, then we could have
    // other classes defined in the xml: tell it to the parser.
    wxXmlNode *classNode = doc.GetRoot();
    bool      hasClasses = false;

    if( !( classNode->GetName() == "class" ) )
    {
        classNode = classNode->GetChildren();
        if( !classNode || ( classNode->GetName() != "class" ) )
            return false;

        hasClasses = true;
    }

    Parse( classNode, hasClasses );
    return true;
}

bool ClassInfoDB::CheckClass( const wxString &name )
{
    // Test for existance in wxRTTI system
    return (wxClassInfo::FindClass( name ) != NULL);
}

ClassType ClassInfoDB::ClassTypeFromString( const wxString &value ) const
{
    if     ( value == "abstract"  ) return CLASS_TYPE_ABSTRACT;
    else if( value == "container" ) return CLASS_TYPE_CONTAINER;
    else if( value == "custom"    ) return CLASS_TYPE_CUSTOM;
    else if( value == "item"      ) return CLASS_TYPE_ITEM;
    else if( value == "layout"    ) return CLASS_TYPE_LAYOUT;
    else if( value == "root"      ) return CLASS_TYPE_ROOT;
    else if( value == "sizer"     ) return CLASS_TYPE_SIZER;
    else if( value == "toplevel"  ) return CLASS_TYPE_TOPLEVEL;
    else if( value == "window"    ) return CLASS_TYPE_WINDOW;
    else if( value == "widget"    ) return CLASS_TYPE_WIDGET;

    return CLASS_TYPE_UNKNOWN;
}

void ClassInfoDB::Parse( wxXmlNode *classNode, bool recursively )
{
    // 'class' element must have a non-empty 'name' attribute
    wxString name = classNode->GetAttribute("name");
    if( name.empty() )
    {
        wxLogError("Unnamed class was found.");
        return;
    }

    // Check only real classes
    // - Base   classes: not instantiable.
    // - Item   classes: pseudo classes, no need to check them here.
    // - Custom classes: CustomCtrl will be managed somewhere else.
    // - Root class is application specific.
    //
    // Disabled classes in XML (wxXML_COMMENT_NODEs) will be skipped.
    ClassType type = ClassTypeFromString( classNode->GetAttribute("type") );
    if
    (
        // TODO: better check for wx abstract classes
        (type != CLASS_TYPE_ROOT)     &&
        (type != CLASS_TYPE_CUSTOM)   &&
        (type != CLASS_TYPE_ITEM)
    )
    {
        ClassNameMap::iterator it = m_classMap.find( name );
        if( (it == m_classMap.end()) && (m_baseList.Index(name) == wxNOT_FOUND) )
        {
            wxLogError( "Class '%s' was not found in registered list.", name );
            return;
        }
        else if( !CheckClass( name ) )
        {
            wxLogError( "Can't register class '%s'.", name );
            return;
        }
    }

//  wxLogDebug( "Loading class %s", name );

    ClassInfo classInfo( new ClassNode( name, type ) );
    m_classes.insert( ClassInfoMap::value_type( name, classInfo ) );

    wxXmlNode *node = classNode->GetChildren();
    while( node )
    {
        wxString nodeName = node->GetName();
        if( nodeName == "description" )
        {
            classInfo->m_desc = node->GetNodeContent();
        }
        else if( nodeName == "inherits" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while( childNode && childNode->GetName() == "class" )
            {
                wxString  baseName = childNode->GetNodeContent();
                ClassInfo baseInfo = GetClassInfo( baseName );

                if( baseInfo )
                    classInfo->AddBaseInfo( baseInfo );

                childNode = childNode->GetNext();
            }
        }
        else if( nodeName == "children" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while( childNode )
            {
                ChildInfo info = DoGetChildInfo( childNode );

                if( info )
                    classInfo->AddChildInfo( info );

                childNode = childNode->GetNext();
            }
        }
        else if( nodeName == "event" )
        {
            EventInfo eventInfo = DoGetEventInfo( node );

            if( eventInfo )
                classInfo->AddEventInfo( eventInfo );
        }
        else
        {
            PropertyInfo propertyInfo = DoGetPropertyInfo( node );

            if( propertyInfo )
                classInfo->AddPropertyInfo( propertyInfo );
        }

        node = node->GetNext();
    }

    for( size_t i = 0; i < classInfo->GetBaseCount(); i++ )
    {
        ClassInfo baseInfo = classInfo->GetBaseInfo(i);
        AddBaseInfos( classInfo, baseInfo );
    }

    if( recursively )
    {
        classNode = classNode->GetNext();

        if( classNode )
            Parse( classNode, true );
    }
}

void ClassInfoDB::AddBaseInfos( ClassInfo classInfo, ClassInfo baseInfo )
{
    wxString baseName = baseInfo->GetName();

    if( m_baseList.Index( baseName ) == wxNOT_FOUND )
        return;

    for( size_t i = 0; i < baseInfo->GetPropertyInfoCount(); i++ )
    {
        PropertyInfo propInfoBase = baseInfo->GetPropertyInfo(i);
        wxString     propBaseName = propInfoBase->GetName();
        PropertyInfo propertyInfo = classInfo->GetPropertyInfo( propBaseName );

        if( propertyInfo )
        {
            for( size_t j = 0; j < propInfoBase->GetChildCount(); j++ )
            {
                PropertyInfo child = propInfoBase->GetChild(j);
                propertyInfo->AddChild( child );
            }
        }
        else
        {
            classInfo->AddPropertyInfo( propInfoBase );
        }
    }

    for( size_t i = 0; i < baseInfo->GetEventInfoCount(); i++ )
    {
        EventInfo eventInfoBase = baseInfo->GetEventInfo(i);
        classInfo->AddEventInfo( eventInfoBase );
    }
}

ChildInfo ClassInfoDB::DoGetChildInfo( wxXmlNode *node )
{
    ClassType type = CLASS_TYPE_UNKNOWN;
    wxString  name = wxEmptyString;
    int       max  = 0;

    if( node->GetName() == "child" )
    {
        type = ClassTypeFromString( node->GetAttribute("type") );
    }
    else if( node->GetName() == "class" )
    {
        name = node->GetAttribute("name");
    }

    if( !node->GetAttribute("max").empty() )
    {
        max = wxGDConv::IntFromString( node->GetAttribute("max") );
    }

    bool hasOption = node->HasAttribute("option");

    ChildInfo info( new ChildInfoNode( type, name, max, hasOption ) );

    return info;
}

EventInfo ClassInfoDB::DoGetEventInfo( wxXmlNode *eventNode )
{
    wxString evtClsName = eventNode->GetAttribute("name");
    if( evtClsName.empty() )
    {
        wxLogError("Event info without a name in class '%s'",
                    eventNode->GetParent()->GetAttribute("name"));
        return EventInfo();
    }

    EventInfo evtInfo( new EventInfoNode( evtClsName ) );

    wxXmlNode *node = eventNode->GetChildren();
    while( node )
    {
        if( node->GetName() == "description" )
        {
            evtInfo->SetDescription( node->GetNodeContent() );
        }
        else if( node->GetName() == "type" )
        {
            wxString evtTypeDesc = wxEmptyString;

            wxXmlNode *childNode = node->GetChildren();
            while( childNode )
            {
                if( childNode->GetName() == "description" )
                {
                    evtTypeDesc = childNode->GetNodeContent();
                }
                childNode = childNode->GetNext();
            }
            // TODO: Add macros
            evtInfo->AddType( node->GetAttribute("name"), evtTypeDesc );
        }
        node = node->GetNext();
    }
    return evtInfo;
}

PropertyInfo ClassInfoDB::DoGetPropertyInfo( wxXmlNode *propertyNode )
{
    if( !propertyNode || (propertyNode->GetType() == wxXML_COMMENT_NODE) )
        return PropertyInfo();

    PropertyType type = GetPropertyType( propertyNode->GetName() );
    PropertyInfo propInfo;

    if( type != PROPERTY_TYPE_UNKNOWN )
    {
        wxString name  = propertyNode->GetAttribute("name");
        wxString label = propertyNode->GetAttribute("label");
        wxString description;

        if( name.empty() )
            name = propertyNode->GetName();

        if( label.empty() )
            label = name.Capitalize();

        wxXmlNode *childNode = propertyNode->GetChildren();

        propInfo = PropertyInfo( new PropertyInfoNode( type, name, label ) );

        while( childNode )
        {
            if( childNode->GetName() == "description" )
            {
                propInfo->SetDescription( childNode->GetNodeContent() );
            }
            else if( childNode->GetName() == "value" )
            {
                propInfo->SetDefaultValue( childNode->GetNodeContent() );
            }
            else
            {
                PropertyInfo childInfo( DoGetPropertyInfo( childNode ) );
                if( childInfo )
                    propInfo->AddChild( childInfo );
            }
            childNode = childNode->GetNext();
        }
    }
    else
    {
        wxLogError( "Unknown property '%s'", propertyNode->GetName() );
    }

    return propInfo;
}
//=============================================================================
//  ClassInfoDB public functions
//=============================================================================
ClassInfo ClassInfoDB::GetClassInfo( const wxString &name ) const
{
    ClassInfoMap::const_iterator it = m_classes.find( name );

    if( it != m_classes.end() )
        return it->second;

    return ClassInfo();
}

PropertyType ClassInfoDB::GetPropertyType( const wxString &name ) const
{
    PropertyTypeMap::const_iterator it = m_propTypes.find( name );

    if( it != m_propTypes.end() )
        return it->second;

    return PROPERTY_TYPE_UNKNOWN;
}

bool ClassInfoDB::ClassInfoExists( const wxString &name )
{
    ClassInfoMap::iterator it = m_classes.find( name );
    return( it != m_classes.end() );
}
