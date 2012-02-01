///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/database.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/29
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/object/database.h"
#include "core/utils.h"

#include <wx/app.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>

#include <wx/log.h>
//-----------------------------------------------------------------------------
//  EventInfoNode
//-----------------------------------------------------------------------------
EventInfoNode::EventInfoNode( const wxString &name, const wxString &description )
                            : m_name( name ),
                              m_desc( description )
{
}

EventInfoNode::~EventInfoNode()
{
    m_types.clear();
}

wxString EventInfoNode::GetTypeName( size_t index )
{
    if ( index < m_types.size() )
    {
        return m_types.at( index ).first;
    }

    return wxEmptyString;
}

wxString EventInfoNode::GetTypeDescription( size_t index )
{
    if ( index < m_types.size() )
        return m_types.at( index ).second;

    return wxEmptyString;
}

void EventInfoNode::AddType( const wxString &name, const wxString &description )
{
    EventType info = std::make_pair( name, description );
    m_types.push_back( info );
}
//-----------------------------------------------------------------------------
//  PropertyInfoNode
//-----------------------------------------------------------------------------

PropertyInfoNode::PropertyInfoNode( PropertyType type,
                                    const wxString &name,
                                    const wxString &label )
                                  : m_type( type ),
                                    m_name( name ),
                                    m_label( label ),
                                    m_value( wxEmptyString ),
                                    m_desc( wxEmptyString )
{
}

PropertyInfoNode::~PropertyInfoNode()
{
    m_children.erase( m_children.begin(), m_children.end() );
}

void PropertyInfoNode::AddChild( const wxString &name, PropertyInfo info )
{
    m_children.insert( PropertyInfoMap::value_type( name, info ) );
}

//-----------------------------------------------------------------------------
//  ClassNode
//-----------------------------------------------------------------------------

ClassNode::ClassNode( const wxString &className, ClassType type )
                            : m_name( className ),
                              m_type( type )
{
}

ClassNode::~ClassNode()
{
    m_events.clear();
    m_props.clear();
    m_childTypes.clear();
}

int ClassNode::GetMaxAllowedBy( const wxString &parentClsName ) const
{
    ClassInfo parentInfo = ClassInfoDB::Get()->GetClassInfo( parentClsName );
    if ( parentInfo.get() )
    {
        // Check by type
        for ( size_t i = 0; i < parentInfo->GetChildTypeCount(); i++ )
        {
            if ( m_type == parentInfo->GetAllowedChildType( i ).first )
            {
                return parentInfo->GetAllowedChildType( i ).second;
            }
        }
        // Check by name
        for ( size_t i = 0; i < parentInfo->GetChildNameCount(); i++ )
        {
            if ( m_name == parentInfo->GetAllowedChildName( i ).first )
            {
                return parentInfo->GetAllowedChildName( i ).second;
            }
        }
    }

    return 0;
}

bool ClassNode::IsKindOf( const wxString &name )
{
    return m_bases.Index( name ) != wxNOT_FOUND;
}

bool ClassNode::IsTypeOf( ClassType type )
{
    return ( m_type == type );
}

wxString ClassNode::GetBaseName( size_t index )
{
    if ( index < m_bases.GetCount() )
        return m_bases.Item( index );

    return wxEmptyString;
}

AllowedChildType ClassNode::GetAllowedChildType( size_t index ) const
{
    if ( index < m_childTypes.size() )
        return m_childTypes.at( index );

    return std::make_pair( CLASS_TYPE_UNKNOWN, 0 );
}

AllowedChildName ClassNode::GetAllowedChildName( size_t index ) const
{
    if ( index < m_childNames.size() )
        return m_childNames.at( index );

    return std::make_pair( wxEmptyString, 0 );
}

EventInfo ClassNode::GetEventInfo( size_t index )
{
    if ( index < m_events.size() )
        return m_events.at( index );

    return EventInfo();
}

PropertyInfo ClassNode::GetPropertyInfo( size_t index )
{
    if ( index < m_props.size() )
        return m_props.at( index );

    return PropertyInfo();
}

bool ClassNode::PropertyInfoExists( const wxString &name )
{
    for ( PropertyInfos::iterator it = m_props.begin(); it != m_props.end(); ++it )
    {
        if ( (*it)->GetName() == name )
            return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
//  ClassInfoDB
//-----------------------------------------------------------------------------
ClassInfoDB::ClassInfoDB()
{
    Init();
}

ClassInfoDB::~ClassInfoDB()
{
    m_classes.clear();
}

ClassInfoDB *ClassInfoDB::ms_instance = NULL;

ClassInfoDB *ClassInfoDB::Get()
{
    if ( !ms_instance )
        ms_instance = new ClassInfoDB;

    return ms_instance;
}

void ClassInfoDB::Free()
{
    if ( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void ClassInfoDB::InitPropertyTypes()
{
    m_types.insert( PropertyTypeMap::value_type( "arraystring", PROPERTY_TYPE_ARRAYSTRING ) );
    m_types.insert( PropertyTypeMap::value_type( "bitmap",      PROPERTY_TYPE_BITMAP ) );
    m_types.insert( PropertyTypeMap::value_type( "bool",        PROPERTY_TYPE_BOOL ) );
    m_types.insert( PropertyTypeMap::value_type( "category",    PROPERTY_TYPE_CATEGORY ) );
    m_types.insert( PropertyTypeMap::value_type( "colour",      PROPERTY_TYPE_COLOUR ) );
    m_types.insert( PropertyTypeMap::value_type( "dimension",   PROPERTY_TYPE_DIMENSION ) );
    m_types.insert( PropertyTypeMap::value_type( "double",      PROPERTY_TYPE_DOUBLE ) );
    m_types.insert( PropertyTypeMap::value_type( "event",       PROPERTY_TYPE_EVENT ) );
    m_types.insert( PropertyTypeMap::value_type( "enum",        PROPERTY_TYPE_ENUM ) );
    m_types.insert( PropertyTypeMap::value_type( "float",       PROPERTY_TYPE_FLOAT ) );
    m_types.insert( PropertyTypeMap::value_type( "flag",        PROPERTY_TYPE_FLAG ) );
    m_types.insert( PropertyTypeMap::value_type( "font",        PROPERTY_TYPE_FONT ) );
    m_types.insert( PropertyTypeMap::value_type( "listcolfmt",  PROPERTY_TYPE_LISTCOLFMT ) );
    m_types.insert( PropertyTypeMap::value_type( "items",       PROPERTY_TYPE_ITEMS ) );
    m_types.insert( PropertyTypeMap::value_type( "int",         PROPERTY_TYPE_INT ) );
    m_types.insert( PropertyTypeMap::value_type( "name",        PROPERTY_TYPE_NAME ) );
    m_types.insert( PropertyTypeMap::value_type( "point",       PROPERTY_TYPE_POINT ) );
    m_types.insert( PropertyTypeMap::value_type( "size",        PROPERTY_TYPE_SIZE ) );
    m_types.insert( PropertyTypeMap::value_type( "string",      PROPERTY_TYPE_STRING ) );
    m_types.insert( PropertyTypeMap::value_type( "style",       PROPERTY_TYPE_STYLE ) );
    m_types.insert( PropertyTypeMap::value_type( "exstyle",     PROPERTY_TYPE_STYLE ) );
    m_types.insert( PropertyTypeMap::value_type( "text",        PROPERTY_TYPE_TEXT ) );
    m_types.insert( PropertyTypeMap::value_type( "url",         PROPERTY_TYPE_URL ) );
}

bool ClassInfoDB::InitClassList( const wxString &path )
{
    // Check document
    wxXmlDocument doc;
    if ( !doc.Load( path ) )
        return false;

    wxXmlNode *node = doc.GetRoot();
    if ( !node || node->GetName() != "category" )
        return false;

    node = node->GetChildren();
    while( node )
    {
        wxXmlNode *itemNode = node->GetChildren();
        while ( itemNode )
        {
            if ( itemNode->GetName() == "item" )
             {
                 m_classList.Add( itemNode->GetNodeContent() );
                 wxLogDebug("Loaded %s", itemNode->GetNodeContent() );
             }

            itemNode = itemNode->GetNext();
        }

        node = node->GetNext();
    }

    if ( m_classList.empty() )
        return false;

    return true;
}

void ClassInfoDB::Init()
{
    // /path/to/db/controls
    wxString dbPath = GetDataBasePath() + wxFILE_SEP_PATH + "controls";

    if ( !wxDirExists( dbPath ) )
        return;

    wxDir dbDir( dbPath );
    if ( !dbDir.IsOpened() )
        return;

    if ( !InitClassList( dbPath + ".xml" ) )
        return;

    InitPropertyTypes();

    wxString currentDir;
    bool haveDir = dbDir.GetFirst( &currentDir, wxEmptyString,
                                    wxDIR_DIRS | wxDIR_HIDDEN );
    while ( haveDir )
    {
        wxString categoryPath = dbPath + wxFILE_SEP_PATH + currentDir;
        wxDir categoryDir( categoryPath );
        if ( categoryDir.IsOpened() )
        {
            wxString xmlFile;
            bool haveXml = categoryDir.GetFirst( &xmlFile, "*.xml",
                                                wxDIR_FILES | wxDIR_HIDDEN );
            while ( haveXml )
            {
                wxFileName xmlFileName( categoryPath + wxFILE_SEP_PATH + xmlFile );
                if ( !xmlFileName.IsAbsolute() )
                    xmlFileName.MakeAbsolute();

                LoadXML( xmlFileName.GetFullPath() );

                haveXml = categoryDir.GetNext( &xmlFile );
            }
        }

        haveDir = dbDir.GetNext( &currentDir );
    }
}

bool ClassInfoDB::LoadXML( const wxString &path )
{
    // Check document
    wxXmlDocument doc;
    if ( !doc.Load( path ) )
        return false;

    // If the 'class' element isn't the root node, then we could have
    // other classes defined in the xml: tell it to the parser.
    wxXmlNode *classNode = doc.GetRoot();
    if ( !( classNode->GetName() == "class" ) )
    {
        classNode = classNode->GetChildren();
        if ( !classNode || ( classNode->GetName() != "class" ) )
            return false;

        Parse( classNode, true );
        return true;
    }

    Parse( classNode );
    return true;
}

bool ClassInfoDB::CheckClass( const wxString &name )
{
    // Test for existance in wxRTTI system
    wxClassInfo *info = wxClassInfo::FindClass( name );

    if ( !info )
        return false;

    return true;
}

void ClassInfoDB::Parse( wxXmlNode *classNode, bool recursively )
{
    // 'class' element must have a non-empty 'name' attribute
    wxString name = classNode->GetAttribute("name");
    if ( name.empty() )
    {
        wxLogError( "Unnamed class was found." );
        return;
    }

    bool listed = m_classList.Index( name ) != wxNOT_FOUND;
    ClassType type = wxGDConv::ClassTypeFromString
                    ( classNode->GetAttribute("type") );

    bool isBase   = ( type == CLASS_TYPE_ABSTRACT );
    bool isItem   = ( type == CLASS_TYPE_ITEM );
    bool isCustom = ( type == CLASS_TYPE_CUSTOM );
    bool isRoot   = ( type == CLASS_TYPE_ROOT );
    // Can't check for:
    // - Base   classes: wxRTTI doesn't support them.
    // - Item   classes: pseudo classes, no need to check them here.
    // - Custom classes: CustomCtrl will be managed somewhere else.
    // - Root class is application specific.
    //
    // Disabled classes in XML (wxXML_COMMENT_NODEs) will be skipped.

    if ( !isBase && !isItem && !isCustom && !isRoot )
    {
        if ( !listed )
        {
            wxLogError( "Class '%s' was not found in registered list.", name );
            return;
        }
        else if ( !CheckClass( name ) )
        {
            wxLogError( "Can't register class '%s'.", name );
            return;
        }
    }

    wxLogDebug( "Loading class %s", name );

    ClassInfo clsInfo( new ClassNode( name, type ) );
    m_classes.insert( ClassInfoMap::value_type( name, clsInfo ) );

    wxXmlNode *node = classNode->GetChildren();
    while ( node )
    {
        if ( node->GetName() == "inherits" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while ( childNode && childNode->GetName() == "class" )
            {
                clsInfo->AddBaseName( childNode->GetNodeContent() );

                childNode = childNode->GetNext();
            }
        }
        else if ( node->GetName() == "children" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while ( childNode )
            {
                if ( childNode->GetName() == "child" )
                {
                    ClassType childType = wxGDConv::ClassTypeFromString
                                            ( childNode->GetAttribute("type") );
                    int max = wxGDConv::IntFromString
                                            ( childNode->GetAttribute("max") );
                    clsInfo->AddAllowedChildType
                                            ( std::make_pair( childType, max ) );
                }
                else if ( childNode->GetName() == "class" )
                {
                    wxString childName = childNode->GetAttribute("name");

                    int max = wxGDConv::IntFromString
                                            ( childNode->GetAttribute("max") );
                    clsInfo->AddAllowedChildName
                                            ( std::make_pair( childName, max ) );
                }

                childNode = childNode->GetNext();
            }
        }
        else if ( node->GetName() == "event" )
        {
            EventInfo evtInfo( DoGetEventInfo( node ) );

            if ( evtInfo.get() )
                clsInfo->AddEventInfo( evtInfo );
        }
        else if ( node->GetType() != wxXML_COMMENT_NODE )
        {
            PropertyInfo propInfo( DoGetPropertyInfo( node ) );

            if ( propInfo.get() )
                clsInfo->AddPropertyInfo( propInfo );
        }

        node = node->GetNext();
    }

    if ( recursively )
    {
        classNode = classNode->GetNext();

        if ( classNode )
            Parse( classNode, true );
    }
}

EventInfo ClassInfoDB::DoGetEventInfo( wxXmlNode *eventNode )
{
    wxString evtClsName = eventNode->GetAttribute("name");

    if ( evtClsName.empty() )
    {
        wxLogError("Event info without a name in class '%s'",
                    eventNode->GetParent()->GetAttribute("name"));
        return EventInfo();
    }

    EventInfo evtInfo( new EventInfoNode( evtClsName ) );

    wxXmlNode *evtTypeNode = eventNode->GetChildren();
    while ( evtTypeNode && evtTypeNode->GetName() == "type" )
    {
        wxString evtTypeName = evtTypeNode->GetAttribute("name");
        wxString evtTypeDesc = wxEmptyString;

        wxXmlNode *descNode = evtTypeNode->GetChildren();
        if ( descNode && descNode->GetName() == "description" )
        {
            evtTypeDesc = descNode->GetNodeContent();
        }

        evtInfo->AddType( evtTypeName, evtTypeDesc );

        evtTypeNode = evtTypeNode->GetNext();
    }

    return evtInfo;
}

PropertyInfo ClassInfoDB::DoGetPropertyInfo( wxXmlNode *propertyNode )
{
    PropertyType type = GetPropertyType( propertyNode->GetName() );
    PropertyInfo propInfo;

    if ( type != PROPERTY_TYPE_UNKNOWN )
    {
        wxString name = propertyNode->GetAttribute("name");
        if ( name.empty() )
            name = propertyNode->GetName();

        wxString label = propertyNode->GetAttribute("label");
        if ( label.empty() )
            label = name.Capitalize();

        wxXmlNode *childNode = propertyNode->GetChildren();

        propInfo.reset( new PropertyInfoNode( type, name, label ) );

        while ( childNode )
        {
            if ( childNode->GetName() == "description" )
            {
                propInfo->SetDescription( childNode->GetNodeContent() );
            }
            else if ( childNode->GetName() == "value" )
            {
                propInfo->SetDefaultValue( childNode->GetNodeContent() );
            }
            else
            {
                PropertyInfo childInfo( DoGetPropertyInfo( childNode ) );
                if ( childInfo.get() )
                    propInfo->AddChild( childInfo->GetName(), childInfo );
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
//-----------------------------------------------------------------------------
//  ClassInfoDB public functions
//-----------------------------------------------------------------------------

ClassInfo ClassInfoDB::GetClassInfo( const wxString &className ) const
{
    ClassInfoMap::const_iterator it = m_classes.find( className );

    if ( it != m_classes.end() )
        return it->second;

    return ClassInfo();
}

PropertyType ClassInfoDB::GetPropertyType( const wxString &tagname ) const
{
    PropertyTypeMap::const_iterator it = m_types.find( tagname );

    if ( it != m_types.end() )
        return it->second;

    return PROPERTY_TYPE_UNKNOWN;
}

bool ClassInfoDB::ClassInfoExists( const wxString &name )
{
    ClassInfoMap::iterator it = m_classes.find( name );
    if ( it != m_classes.end() )
        return true;

    return false;
}
