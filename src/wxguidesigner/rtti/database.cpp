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
// ChildInfoNode
//=============================================================================
ChildInfoNode::ChildInfoNode  ( ClassType type, const wxString &name,
                                int max, bool option )
:
m_type  ( type ),
m_name  ( name ),
m_max   ( max ),
m_option( option )
{
}

ChildInfoNode::~ChildInfoNode()
{
}

ClassType ChildInfoNode::GetType() const
{
    return m_type;
}

wxString ChildInfoNode::GetName() const
{
    return m_name;
}

int ChildInfoNode::GetMax() const
{
    return m_max;
}

bool ChildInfoNode::IsOption() const
{
    return m_option;
}

bool ChildInfoNode::IsType() const
{
    return m_name.empty();
}
//=============================================================================
// EventInfoNode
//=============================================================================
EventInfoNode::EventInfoNode( const wxString &name, const wxString &description )
:
m_name( name ),
m_desc( description )
{
}

EventInfoNode::~EventInfoNode()
{
    m_types.clear();
}

wxString EventInfoNode::GetName() const
{
    return m_name;
}

wxString EventInfoNode::GetDescription() const
{
    return m_desc;
}
//-----------------------------------------------------------------------------
// EventTypeInfos
//-----------------------------------------------------------------------------
void EventInfoNode::AddType( const wxString &name, const wxString &description )
{
    EventTypeInfo info( new EventTypeInfoNode( name, description ) );
    m_types.push_back( info );
}

void EventInfoNode::SetDescription( const wxString &description )
{
    m_desc = description;
}

wxString EventInfoNode::GetTypeName( size_t index ) const
{
    if( index < m_types.size() )
        return m_types.at( index )->GetName();

    return wxEmptyString;
}

wxString EventInfoNode::GetTypeDescription( size_t index ) const
{
    if( index < m_types.size() )
        return m_types.at( index )->GetDescription();

    return wxEmptyString;
}

size_t EventInfoNode::GetTypeCount() const
{
    return m_types.size();
}
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

PropertyType PropertyInfoNode::GetType() const
{
    return m_type;
}

wxString PropertyInfoNode::GetName() const
{
    return m_name;
}

wxString PropertyInfoNode::GetLabel() const
{
    return m_label;
}

wxString PropertyInfoNode::GetDefaultValue() const
{
    return m_value;
}

wxString PropertyInfoNode::GetDescription() const
{
    return m_desc;
}

size_t PropertyInfoNode::GetChildCount() const
{
    return m_children.size();
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

void PropertyInfoNode::SetDefaultValue( const wxString &value )
{
    m_value = value;
}

void PropertyInfoNode::SetDescription( const wxString &description )
{
    m_desc = description;
}
//=============================================================================
//  ClassNode
//=============================================================================
ClassNode::ClassNode( const wxString &className, ClassType classType )
:
m_name( className ),
m_type( classType )
{
}

ClassNode::~ClassNode()
{
    m_eventInfos.clear();
    m_propInfos.clear();
    m_children.clear();
    m_baseInfos.clear();
}

wxString ClassNode::GetName() const
{
    return m_name;
}

wxString ClassNode::GetDescription() const
{
    return m_desc;
}

ClassType ClassNode::GetType() const
{
    return m_type;
}

void ClassNode::SetDescription( const wxString &description )
{
    m_desc = description;
}
//-----------------------------------------------------------------------------
// Inherited classes
//-----------------------------------------------------------------------------
ClassInfo ClassNode::GetBaseInfo( size_t index ) const
{
    if( index < m_baseInfos.size() )
        return m_baseInfos.at( index );

    return ClassInfo();
}

wxString ClassNode::GetBaseName( size_t index ) const
{
    if( index < m_baseInfos.size() )
        return m_baseInfos.at( index )->GetName();

    return wxEmptyString;
}

size_t ClassNode::GetBaseCount() const
{
    return m_baseInfos.size();
}

void ClassNode::AddBaseInfo( ClassInfo classInfo )
{
    if( !classInfo )
        return;

    m_baseInfos.push_back( classInfo );
}

bool ClassNode::IsA( const wxString &className ) const
{
    if( m_name == className )
        return true;

    for( size_t i = 0; i < m_baseInfos.size(); i++ )
    {
        wxString baseName = GetBaseName(i);

        if( baseName == className )
            return true;

        // Recursively check a level up
        ClassInfo baseInfo = ClassInfoDB::Get()->GetClassInfo( baseName );

        if( baseInfo && baseInfo->IsA( className ) )
            return true;
    }

    return false;
}

bool ClassNode::IsTypeOf( ClassType type ) const
{
    return( m_type == type );
}
//-----------------------------------------------------------------------------
// PropertyInfos
//-----------------------------------------------------------------------------
void ClassNode::AddPropertyInfo( PropertyInfo propertyInfo )
{
    if( !propertyInfo )
        return;

    m_propInfos.push_back( propertyInfo );
}

PropertyInfo ClassNode::GetPropertyInfo( size_t index ) const
{
    if( index < m_propInfos.size() )
        return m_propInfos.at( index );

    return PropertyInfo();
}

PropertyInfo ClassNode::GetPropertyInfo( const wxString &name ) const
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

size_t ClassNode::GetPropertyInfoCount() const
{
    return m_propInfos.size();
}
//-----------------------------------------------------------------------------
// EventInfos
//-----------------------------------------------------------------------------
void ClassNode::AddEventInfo( EventInfo eventInfo )
{
    m_eventInfos.push_back( eventInfo );
}

EventInfo ClassNode::GetEventInfo( size_t index ) const
{
    if( index < m_eventInfos.size() )
        return m_eventInfos.at( index );

    return EventInfo();
}

size_t ClassNode::GetEventInfoCount() const
{
    return m_eventInfos.size();
}
//-----------------------------------------------------------------------------
// ChildInfos
//-----------------------------------------------------------------------------
void ClassNode::AddChildInfo( ChildInfo childInfo )
{
    m_children.push_back( childInfo );
}

ChildInfo ClassNode::GetChildInfo( size_t index ) const
{
    if( index < m_children.size() )
        return m_children.at( index );

    return ChildInfo();
}

ChildInfo ClassNode::GetChildInfo( const wxString &name ) const
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

size_t ClassNode::GetChildInfoCount() const
{
    return m_children.size();
}

bool ClassNode::IsChildInfoOk( const wxString &className, size_t count ) const
{
    ClassInfo classInfo = ClassInfoDB::Get()->GetClassInfo( className );
    if( !classInfo )
        return false;

    for( size_t i = 0; i < m_children.size(); i++ )
    {
        ChildInfo childInfo = m_children.at(i);
        int       childMax  = childInfo->GetMax();
        bool      countOK   = ((childMax == -1) || (childMax > (int)count));
        wxString  childName = childInfo->GetName();

        if( classInfo->IsA( childName ) && countOK )
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
    m_classInfos.clear();
    m_classNames.clear();
    m_classTypes.clear();
    m_propTypes.clear();
    m_baseNames.clear();
}

ClassInfoDB *ClassInfoDB::ms_instance = NULL;

ClassInfoDB *ClassInfoDB::Get()
{
    if(!ms_instance)
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

                m_baseNames.push_back( className );
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
                    m_classNames.push_back( className );
                    m_classTypes.push_back( nodeName );

//                  wxLogDebug("Loaded %s", className );
                }

                itemNode = itemNode->GetNext();
            }
        }

        node = node->GetNext();
    }
    if( category == "controlbase" )
        return( !m_baseNames.empty() );
    else if( category == "controls" )
        return( !m_classNames.empty() );

    return false;
}

void ClassInfoDB::Init()
{
    InitPropertyTypes();

    if( !InitClassList("controlbase") || !InitClassList("controls") )
        return;

    LoadClasses("controlbase");

    if( m_classInfos.empty() )
        return;

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
        for( size_t i = 0; i < m_baseNames.size(); i++ )
        {
            wxString xmlFile = m_baseNames.at(i).Lower() + ".xml";
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
        for( size_t i = 0; i < m_classNames.size(); i++ )
        {
            wxString className = m_classNames.at(i);
            wxString classType = m_classTypes.at(i);
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
    return( wxClassInfo::FindClass( name ) != NULL );
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
    wxString className = classNode->GetAttribute("name");
    if( className.empty() )
    {
        wxLogError("Unnamed class was found.");
        return;
    }

    // Check only real classes
    // Disabled classes in XML (wxXML_COMMENT_NODEs) will be skipped.
    ClassType classType = ClassTypeFromString( classNode->GetAttribute("type") );
    bool      classIsOk = CheckClass( className );

    // TODO: Why wxAnyButton is not recognized by wxRTTI?
    if( (classType != CLASS_TYPE_ROOT) && (classType != CLASS_TYPE_CUSTOM) &&
        (classType != CLASS_TYPE_ITEM) && (classType != CLASS_TYPE_ABSTRACT) )
    {
        for( Strings::iterator baseIt = m_baseNames.begin();
                                baseIt < m_baseNames.end(); ++baseIt )

        for( Strings::iterator classIt = m_classNames.begin();
                                classIt < m_classNames.end(); ++classIt )

        if( (baseIt == m_baseNames.end()) && (classIt == m_classNames.end()) )
        {
            wxLogError( "Class '%s' was not found in registered list.", className );
            return;
        }
        else if( !classIsOk )
        {
            wxLogError( "Can't register class '%s'.", className );
            return;
        }
    }

//  wxLogDebug( "Loading class %s", className );

    ClassInfo classInfo( new ClassNode( className, classType ) );
    m_classInfos.insert( ClassInfoMap::value_type( className, classInfo ) );

    wxXmlNode *node = classNode->GetChildren();

    while( node )
    {
        wxString nodeName = node->GetName();
        if( nodeName == "description" )
        {
            wxString description = node->GetNodeContent();
            classInfo->SetDescription( description );
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
                ChildInfo childInfo = DoGetChildInfo( childNode );

                if( childInfo )
                    classInfo->AddChildInfo( childInfo );

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

    // Add inherited classes events/properties informations
    for( size_t i = 0; i < classInfo->GetBaseCount(); i++ )
    {
        ClassInfo baseInfo = classInfo->GetBaseInfo(i);
        AddBaseInfos( classInfo, baseInfo );
    }

    // Parse eventually other classes defined inside same XML file
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
    bool found = false;

    for( Strings::iterator it = m_baseNames.begin();
                            it != m_baseNames.end(); ++it )
    {
        if( *it == baseName )
            found = true;
    }

    if( !found )
    {
        for( Strings::iterator it = m_classNames.begin();
                                it != m_classNames.end(); ++it )
        {
            if( *it == baseName )
                found = true;
        }
    }

    if( !found )
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
    if( !node )
        return ChildInfo();

    ClassType childType = CLASS_TYPE_UNKNOWN;
    wxString  childName = wxEmptyString;
    wxString  nodeName  = node->GetName();
    int       max       = -1;
    bool      isOption  = false;

    if( nodeName == "type" )
    {
        wxString type = node->GetNodeContent();
        childType = ClassTypeFromString( type );
    }
    else if( nodeName == "class" )
    {
        childName = node->GetNodeContent();
    }
    else if( nodeName == "option" )
    {
        childName = node->GetNodeContent();
        isOption  = true;
    }

    if( node->HasAttribute("max") )
    {
        max = wxGDConv::IntFromString( node->GetAttribute("max") );
    }

    ChildInfo info( new ChildInfoNode( childType, childName, max, isOption ) );

    return info;
}

EventInfo ClassInfoDB::DoGetEventInfo( wxXmlNode *eventNode )
{
    if( !eventNode )
        return EventInfo();

    wxString evtClsName = eventNode->GetAttribute("name");
    if( evtClsName.empty() )
    {
        wxLogError("Event info without a name in class '%s'",
                    eventNode->GetParent()->GetAttribute("name"));
        return EventInfo();
    }

    EventInfo eventInfo( new EventInfoNode( evtClsName ) );

    wxXmlNode *node = eventNode->GetChildren();
    while( node )
    {
        wxString nodeName = node->GetName();

        if( nodeName == "description" )
        {
            eventInfo->SetDescription( node->GetNodeContent() );
        }
        else if( nodeName == "type" )
        {
            wxString eventTypeDesc = wxEmptyString;

            wxXmlNode *childNode = node->GetChildren();
            while( childNode )
            {
                if( childNode->GetName() == "description" )
                {
                    eventTypeDesc = childNode->GetNodeContent();
                    break;
                }

                childNode = childNode->GetNext();
            }
            // TODO: Add macros
            eventInfo->AddType( node->GetAttribute("name"), eventTypeDesc );
        }
        node = node->GetNext();
    }

    return eventInfo;
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

        propInfo = PropertyInfo( new PropertyInfoNode( type, name, label ) );

        wxXmlNode *childNode = propertyNode->GetChildren();

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
    ClassInfoMap::const_iterator it = m_classInfos.find( name );

    if( it != m_classInfos.end() )
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
    ClassInfoMap::iterator it = m_classInfos.find( name );
    return( it != m_classInfos.end() );
}
