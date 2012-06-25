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
                            : m_name( className ), m_type( type )
{
}

ClassNode::~ClassNode()
{
    m_events.clear();
    m_props.clear();
    m_children.clear();
}

bool ClassNode::IsKindOf( const wxString &name )
{
    return m_bases.Index( name ) != wxNOT_FOUND;
}

bool ClassNode::IsTypeOf( ClassType type )
{
    return( m_type == type );
}

wxString ClassNode::GetBaseName( size_t index )
{
    if( index < m_bases.GetCount() )
        return m_bases.Item( index );

    return wxEmptyString;
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
    if( !childClsInfo.get() )
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

bool ClassInfoDB::InitClassList( const wxString &path )
{
    // Check document
    wxXmlDocument doc;
    if( !doc.Load( path ) )
        return false;

    wxXmlNode *node = doc.GetRoot()->GetChildren();
    while( node )
    {
        wxXmlNode *itemNode = node->GetChildren();
        while( itemNode )
        {
            if( itemNode->GetName() == "item" )
             {
                 m_classList.Add( itemNode->GetNodeContent() );
                 //wxLogDebug("Loaded %s", itemNode->GetNodeContent() );
             }

            itemNode = itemNode->GetNext();
        }

        node = node->GetNext();
    }

    if( m_classList.empty() )
        return false;

    return true;
}

void ClassInfoDB::Init()
{
    // /path/to/db/controls
    wxString dbPath = GetDataBasePath() + wxFILE_SEP_PATH + "controls";

    if( !wxDirExists( dbPath ) )
        return;

    wxDir dbDir( dbPath );
    if( !dbDir.IsOpened() )
        return;

    if( !InitClassList( dbPath + ".xml" ) )
        return;

    InitPropertyTypes();

    wxString currentDir;
    bool haveDir = dbDir.GetFirst( &currentDir, wxEmptyString,
                                    wxDIR_DIRS | wxDIR_HIDDEN );
    while( haveDir )
    {
        wxString categoryPath = dbPath + wxFILE_SEP_PATH + currentDir;
        wxDir categoryDir( categoryPath );
        if( categoryDir.IsOpened() )
        {
            wxString xmlFile;
            bool haveXml = categoryDir.GetFirst( &xmlFile, "*.xml",
                                                wxDIR_FILES | wxDIR_HIDDEN );
            while( haveXml )
            {
                wxFileName xmlFileName( categoryPath + wxFILE_SEP_PATH + xmlFile );
                if( !xmlFileName.IsAbsolute() )
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
    if( !doc.Load( path ) )
        return false;

    // If the 'class' element isn't the root node, then we could have
    // other classes defined in the xml: tell it to the parser.
    wxXmlNode *classNode = doc.GetRoot();
    if( !( classNode->GetName() == "class" ) )
    {
        classNode = classNode->GetChildren();
        if( !classNode || ( classNode->GetName() != "class" ) )
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
        (type != CLASS_TYPE_ABSTRACT) && (type != CLASS_TYPE_ITEM) &&
        (type != CLASS_TYPE_CUSTOM)   && (type != CLASS_TYPE_ROOT)
    )
    {
        if( m_classList.Index( name ) == wxNOT_FOUND )
        {
            wxLogError( "Class '%s' was not found in registered list.", name );
            return;
        }
        else if( !CheckClass( name ) )
        {
//          wxLogError( "Can't register class '%s'.", name );
            return;
        }
    }


//  wxLogDebug( "Loading class %s", name );

    ClassInfo clsInfo( new ClassNode( name, type ) );
    m_classes.insert( ClassInfoMap::value_type( name, clsInfo ) );

    wxXmlNode *node = classNode->GetChildren();
    while( node )
    {
        wxString nodeName = node->GetName();
        if( nodeName == "description" )
        {
            clsInfo->m_desc = node->GetNodeContent();
        }
        else if( nodeName == "inherits" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while( childNode && childNode->GetName() == "class" )
            {
                clsInfo->AddBaseName( childNode->GetNodeContent() );

                childNode = childNode->GetNext();
            }
        }
        else if( nodeName == "children" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while( childNode )
            {
                ChildInfo info = DoGetChildInfo( childNode );

                if( info.get() )
                    clsInfo->AddChildInfo( info );

                childNode = childNode->GetNext();
            }
        }
        else if( nodeName == "event" )
        {
            EventInfo info = DoGetEventInfo( node );

            if( info.get() )
                clsInfo->AddEventInfo( info );
        }
        else if( node->GetType() != wxXML_COMMENT_NODE )
        {
            PropertyInfo info = DoGetPropertyInfo( node );

            if( info.get() )
                clsInfo->AddPropertyInfo( info );
        }

        node = node->GetNext();
    }

    if( recursively )
    {
        classNode = classNode->GetNext();

        if( classNode )
            Parse( classNode, true );
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

    bool opt  = node->HasAttribute("option");

    ChildInfo info( new ChildInfoNode( type, name, max, opt ) );

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

        propInfo.reset( new PropertyInfoNode( type, name, label ) );

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
                if( childInfo.get() )
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

ClassInfo ClassInfoDB::GetClassInfo( const wxString &className ) const
{
    ClassInfoMap::const_iterator it = m_classes.find( className );

    if( it != m_classes.end() )
        return it->second;

    return ClassInfo();
}

PropertyType ClassInfoDB::GetPropertyType( const wxString &tagname ) const
{
    PropertyTypeMap::const_iterator it = m_propTypes.find( tagname );

    if( it != m_propTypes.end() )
        return it->second;

    return PROPERTY_TYPE_UNKNOWN;
}

bool ClassInfoDB::ClassInfoExists( const wxString &name )
{
    ClassInfoMap::iterator it = m_classes.find( name );
    if( it != m_classes.end() )
        return true;

    return false;
}
