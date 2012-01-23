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
//  WidgetInfoBase
//-----------------------------------------------------------------------------

WidgetInfoBase::~WidgetInfoBase()
{
    m_evtInfos.erase( m_evtInfos.begin(), m_evtInfos.end() );
    m_propInfos.erase( m_propInfos.begin(), m_propInfos.end() );
}

void WidgetInfoBase::AddEventInfo( EventInfo info )
{
    if ( info.get() )
    {
        m_evtInfos.insert
        ( std::map< wxString, EventInfo >::value_type( info->GetClassName(), info ) );
    }
}

void WidgetInfoBase::AddPropertyInfo( PropertyInfo info )
{
    if ( info.get() )
    {
        m_propInfos.insert( std::map< wxString, PropertyInfo >::value_type( info->GetName(), info ) );
    }
}

bool WidgetInfoBase::CanBeChildOf( const wxString &name )
{
    return m_parents.Index( name ) != wxNOT_FOUND;
}

bool WidgetInfoBase::IsKindOf( const wxString &name )
{
    return m_bases.Index( name ) != wxNOT_FOUND;
}
//-----------------------------------------------------------------------------
//  WidgetInfoDB
//-----------------------------------------------------------------------------

WidgetInfoDB *WidgetInfoDB::ms_instance = NULL;

WidgetInfoDB *WidgetInfoDB::Get()
{
    if ( !ms_instance )
        ms_instance = new WidgetInfoDB;

    return ms_instance;
}

void WidgetInfoDB::Free()
{
    m_classes.erase( m_classes.begin(), m_classes.end() );

    if ( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void WidgetInfoDB::InitPropertyTypes()
{
    m_types.insert( std::map< wxString, PropertyType >::value_type( "arraystring", PROPERTY_TYPE_ARRAYSTRING ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "bitmap",      PROPERTY_TYPE_BITMAP ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "bool",        PROPERTY_TYPE_BOOL ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "category",    PROPERTY_TYPE_CATEGORY ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "colour",      PROPERTY_TYPE_COLOUR ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "dimension",   PROPERTY_TYPE_DIMENSION ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "double",      PROPERTY_TYPE_DOUBLE ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "event",       PROPERTY_TYPE_EVENT ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "enum",        PROPERTY_TYPE_ENUM ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "float",       PROPERTY_TYPE_FLOAT ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "flag",        PROPERTY_TYPE_FLAG ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "font",        PROPERTY_TYPE_FONT ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "listcolfmt",  PROPERTY_TYPE_LISTCOLFMT ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "items",       PROPERTY_TYPE_ITEMS ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "int",         PROPERTY_TYPE_INT ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "name",        PROPERTY_TYPE_NAME ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "point",       PROPERTY_TYPE_POINT ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "size",        PROPERTY_TYPE_SIZE ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "string",      PROPERTY_TYPE_STRING ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "style",       PROPERTY_TYPE_STYLE ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "exstyle",     PROPERTY_TYPE_STYLE ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "text",        PROPERTY_TYPE_TEXT ) );
    m_types.insert( std::map< wxString, PropertyType >::value_type( "url",         PROPERTY_TYPE_URL ) );
}

bool WidgetInfoDB::InitClassList( const wxString &path )
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

void WidgetInfoDB::Init()
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

bool WidgetInfoDB::LoadXML( const wxString &path )
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

bool WidgetInfoDB::CheckClass( const wxString &name )
{
    // Test for existance in wxRTTI system
    wxClassInfo *info = wxClassInfo::FindClass( name );

    if ( !info )
        return false;

    return true;
}

void WidgetInfoDB::Parse( wxXmlNode *classNode, bool recursively )
{
    // 'class' element must have a non-empty 'name' attribute
    wxString name = classNode->GetAttribute("name");
    if ( name.empty() )
    {
        wxLogError( "Unnamed class was found." );
        return;
    }

    bool listed = m_classList.Index( name ) != wxNOT_FOUND;
    bool isBase = (classNode->GetAttribute("type") == "abstract");
    bool isItem = (classNode->GetAttribute("type") == "item");
    bool isTop  = (classNode->GetAttribute("type") == "toplevel");
    bool isSizer= (classNode->GetAttribute("type") == "sizer");

    WidgetType type = WIDGET_TYPE_DEFAULT;
    if ( isBase )
    {
        type = WIDGET_TYPE_ABSTRACT;
    }
    else if ( isItem )
    {
        type = WIDGET_TYPE_ITEM;
    }
    else if ( isSizer )
    {
        type = WIDGET_TYPE_SIZER;
    }
    else if ( isTop )
    {
        type = WIDGET_TYPE_TOPLEVEL;
    }

    // Can't check for baseclasses: wxRTTI doesn't support them,
    // item types are pseudo classes, they will be used and tested through XRC,
    // disabled classes in XML will be skipped, the rest will be ignored
    // if not supported by wxRTTI
    if ( !isBase && !isItem )
    {
        if ( !listed )
        {
            wxLogError( "Class '%s' was not found in registred list.", name );
            return;
        }
        else if ( !CheckClass( name ) )
        {
            wxLogError( "Can't register class '%s'.", name );
            return;
        }
    }

    wxLogDebug( "Loading class %s", name );

    WidgetInfo clsInfo( new WidgetInfoBase( name, type ) );
    m_classes.insert( std::map< wxString, WidgetInfo >::value_type( name, clsInfo ) );

    wxXmlNode *node = classNode->GetChildren();
    while ( node && (node->GetType() != wxXML_COMMENT_NODE) )
    {
        // Parse the base class data recursively to add into the WidgetInfo
        if ( node->GetName() == "inherits" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while ( childNode && childNode->GetName() == "class" )
            {
                clsInfo->AddBaseName( childNode->GetNodeContent() );

                childNode = childNode->GetNext();
            }
        }
        else if ( node->GetName() == "parents" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while ( childNode && childNode->GetName() == "class" )
            {
                clsInfo->AddParentName( childNode->GetNodeContent() );

                childNode = childNode->GetNext();
            }
        }
        else if ( node->GetName() == "event" )
        {
            EventInfo evtInfo( DoGetEventInfo( node ) );

            if ( evtInfo.get() )
                clsInfo->AddEventInfo( evtInfo );
        }
        else
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

EventInfo WidgetInfoDB::DoGetEventInfo( wxXmlNode *eventNode )
{
    wxString evtClsName = eventNode->GetAttribute("name");

    if ( evtClsName.empty() )
    {
        wxLogError("Event info without a name in class '%s'",
                    eventNode->GetParent()->GetAttribute("name"));
        return EventInfo();
    }

    EventInfo evtInfo( new EventInfoBase( evtClsName ) );

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

PropertyInfo WidgetInfoDB::DoGetPropertyInfo( wxXmlNode *propertyNode )
{
    PropertyType propType = GetPropertyType( propertyNode->GetName() );
    PropertyInfo propInfo;

    if ( propType != PROPERTY_TYPE_UNKNOWN )
    {
        // Some properties like 'style' gets the name by the element node name
        wxString propName = propertyNode->GetAttribute("name");
        if ( propName.empty() )
            propName = propertyNode->GetName();

        wxString propDesc = wxEmptyString;
        wxString propVal  = wxEmptyString;
        std::map< wxString, PropertyInfo > propInfoMap;

        wxXmlNode *childNode = propertyNode->GetChildren();

        propInfo.reset
         ( new PropertyInfoBase( propType, propName, propVal, propDesc ) );

        while ( childNode )
        {
            if ( childNode->GetName() == "description" )
            {
                propDesc = childNode->GetNodeContent();
            }
            else if ( childNode->GetName() == "value" )
            {
                propVal = childNode->GetNodeContent();
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
//  WidgetInfoDB public functions
//-----------------------------------------------------------------------------

WidgetInfo WidgetInfoDB::GetClassInfo( const wxString &className )
{
    std::map< wxString, WidgetInfo >::const_iterator it = m_classes.find( className );

    if ( it != m_classes.end() )
        return it->second;

    return WidgetInfo();
}

PropertyType WidgetInfoDB::GetPropertyType( const wxString &tagname ) const
{
    std::map< wxString, PropertyType >::const_iterator it = m_types.find( tagname );

    if ( it != m_types.end() )
        return it->second;

    return PROPERTY_TYPE_UNKNOWN;
}
