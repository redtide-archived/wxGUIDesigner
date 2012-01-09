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

#include <wx/app.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>

#include <wx/log.h>
//-----------------------------------------------------------------------------
//  XMLDataBase
//-----------------------------------------------------------------------------

XMLDataBase::~XMLDataBase()
{
    m_classes.erase( m_classes.begin(), m_classes.end() );
}

bool XMLDataBase::Load()
{
    wxString basePath = ObjectDBManager::Get()->GetBasePath();
    wxString dbPath   = basePath + m_category + wxFILE_SEP_PATH;

    if ( !wxDirExists( dbPath ) )
        return false;

    wxDir dbDir( dbPath );
    if ( !dbDir.IsOpened() )
        return false;

    wxString currentDirName;
    bool haveDir = dbDir.GetFirst( &currentDirName, wxEmptyString,
                                    wxDIR_DIRS | wxDIR_HIDDEN );
    while ( haveDir )
    {
        wxString pluginPath = dbPath + currentDirName;
        wxDir pluginDir( pluginPath );
        if ( pluginDir.IsOpened() )
        {
            wxString xmlFilePath;
            bool haveXml = pluginDir.GetFirst( &xmlFilePath, "*.xml",
                                                wxDIR_FILES | wxDIR_HIDDEN );
            while ( haveXml )
            {
                wxFileName xmlFileName( pluginPath + wxFILE_SEP_PATH + xmlFilePath );
                if ( !xmlFileName.IsAbsolute() )
                    xmlFileName.MakeAbsolute();

                ReadXML( xmlFileName.GetFullPath() );

                haveXml = pluginDir.GetNext( &xmlFilePath );
            }
        }

        haveDir = dbDir.GetNext( &currentDirName );
    }

    return true;
}

bool XMLDataBase::ReadXML( const wxString &xmlpath )
{
    // Check document
    wxXmlDocument doc;
    if ( !doc.Load( xmlpath ) )
        return false;

    // If the 'class' element isn't the root, then we could have
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

void XMLDataBase::Parse( wxXmlNode *classNode, bool recursively )
{
    // 'class' element must have a non-empty 'name' attribute
    wxString name = classNode->GetAttribute("name");
    if ( name.empty() )
    {
        wxLogError( "Class '%s' without a name", classNode->GetName() );
        return;
    }

    wxArrayString   parents;
    wxArrayString   baseClasses;
    EventInfoMap    evtInfoMap;
    PropertyInfoMap propInfoMap;

    wxXmlNode *node = classNode->GetChildren();
    while ( node )
    {
        // Parse the base class data recursively to add into the ClassInfo
/*      if ( node->GetName() == "include" )
        {
            wxString includePath = node->GetNodeContent();
            if ( !includePath.empty() )
            {
                includePath.Replace( "/", wxFILE_SEP_PATH );
                wxString path = ObjectDBManager::Get()->GetBasePath() + includePath;
                Parse( path, true );
            }
        }
        else */
        if ( node->GetName() == "inherits" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while ( childNode && childNode->GetName() == "class" )
            {
                baseClasses.Add( childNode->GetNodeContent() );

                childNode = childNode->GetNext();
            }
        }
        else if ( node->GetName() == "parents" )
        {
            wxXmlNode *childNode = node->GetChildren();
            while ( childNode && childNode->GetName() == "class" )
            {
                parents.Add( childNode->GetNodeContent() );

                childNode = childNode->GetNext();
            }
        }
        else if ( node->GetName() == "event" )
        {
            EventInfoBase *evtInfoBase = DoGetEventInfo( node );
            if ( evtInfoBase )
            {
                EventInfo evtInfo( evtInfoBase );
                evtInfoMap.insert( EventInfoMap::value_type
                                    ( evtInfoBase->GetClassName(), evtInfo ) );
            }
        }
        else
        {
            PropertyInfoBase *propInfoBase = DoGetPropertyInfo( node );
            if ( propInfoBase )
            {
                PropertyInfo propInfo( propInfoBase );
                propInfoMap.insert( PropertyInfoMap::value_type
                                    ( propInfoBase->GetName(), propInfo ) );
            }
        }

        node = node->GetNext();
    }

    ClassInfoBase *clsInfoBase = new ClassInfoBase( name, baseClasses, parents,
                                                    evtInfoMap, propInfoMap );
    ClassInfo clsInfo( clsInfoBase );
    m_classes.insert( ClassInfoMap::value_type( name, clsInfo ) );

    if ( recursively )
    {
        classNode = classNode->GetNext();

        if ( classNode )
            Parse( classNode, true );
    }
}

EventInfoBase *XMLDataBase::DoGetEventInfo( wxXmlNode *eventNode )
{
    wxString evtClsName = eventNode->GetAttribute("name");

    if ( evtClsName.empty() )
    {
        wxLogError("Event info without a name in class '%s'",
                    eventNode->GetParent()->GetAttribute("name"));
        return NULL;
    }

    EventInfoBase *evtInfoBase = new EventInfoBase( evtClsName );

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

        evtInfoBase->AddEventType( evtTypeName, evtTypeDesc );

        evtTypeNode = evtTypeNode->GetNext();
    }

    return evtInfoBase;
}

PropertyInfoBase *XMLDataBase::DoGetPropertyInfo( wxXmlNode *propertyNode )
{
    PropertyInfoBase *propInfoBase = NULL;
    PropertyType propType = GetPropertyType( propertyNode->GetName() );

    if ( propType != PROPERTY_TYPE_UNKNOWN )
    {
        // Some properties like 'style' gets the name by the element node name
        wxString propName = propertyNode->GetAttribute("name");
        if ( propName.empty() )
            propName = propertyNode->GetName();

        wxString propDesc = wxEmptyString;
        wxString propVal  = wxEmptyString;
        PropertyInfoMap propInfoMap;
        PropertyInfoBase *childInfoBase = NULL;

        wxXmlNode *childNode = propertyNode->GetChildren();
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
                childInfoBase = DoGetPropertyInfo( childNode );
                if ( childInfoBase )
                {
                    PropertyInfo childInfo( childInfoBase );
                    propInfoMap.insert( PropertyInfoMap::value_type
                                        ( childInfoBase->GetName(), childInfo ) );
                }
            }

            childNode = childNode->GetNext();
        }

        propInfoBase = new PropertyInfoBase( propType, propName, propVal, propDesc );

        if ( !propInfoMap.empty() )
            propInfoBase->SetChildren( propInfoMap );
    }
    else
    {
        wxLogError( "Unknown property '%s'", propertyNode->GetName() );
    }

    return propInfoBase;
}

ClassInfo XMLDataBase::GetClassInfo( const wxString &className )
{
    ClassInfoMap::const_iterator it = m_classes.find( className );

    if ( it != m_classes.end() )
        return it->second;

    return ClassInfo();
}

/*
int XMLDataBase::GetBaseClassInfoIndex( const wxString &name )
{
    for ( ClassInfoMap::iterator it = m_baseClasses.begin();
            it != m_baseClasses.end(); ++it )
    {
        ClassInfo baseClass = (*it);
        if ( baseClass.get()->GetClassName() == name )
            return baseClass;
    }
}
*/
//-----------------------------------------------------------------------------
//  ObjectDBManager
//-----------------------------------------------------------------------------

ObjectDBManager *ObjectDBManager::ms_instance = NULL;

ObjectDBManager *ObjectDBManager::Get()
{
    if ( !ms_instance )
        ms_instance = new ObjectDBManager;

    return ms_instance;
}

void ObjectDBManager::Free()
{
    m_databases.erase( m_databases.begin(), m_databases.end() );

    if ( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void ObjectDBManager::InitTypes()
{
    m_types.insert( PropertyTypeMap::value_type( "bitmap",   PROPERTY_TYPE_BITMAP ) );
    m_types.insert( PropertyTypeMap::value_type( "bool",     PROPERTY_TYPE_BOOL ) );
    m_types.insert( PropertyTypeMap::value_type( "category", PROPERTY_TYPE_CATEGORY ) );
    m_types.insert( PropertyTypeMap::value_type( "colour",   PROPERTY_TYPE_COLOUR ) );
    m_types.insert( PropertyTypeMap::value_type( "dimension",PROPERTY_TYPE_DIMENSION ) );
    m_types.insert( PropertyTypeMap::value_type( "double",   PROPERTY_TYPE_DOUBLE ) );
    m_types.insert( PropertyTypeMap::value_type( "event",    PROPERTY_TYPE_EVENT ) );
    m_types.insert( PropertyTypeMap::value_type( "enum",     PROPERTY_TYPE_ENUM ) );
    m_types.insert( PropertyTypeMap::value_type( "float",    PROPERTY_TYPE_FLOAT ) );
    m_types.insert( PropertyTypeMap::value_type( "flag",     PROPERTY_TYPE_FLAG ) );
    m_types.insert( PropertyTypeMap::value_type( "font",     PROPERTY_TYPE_FONT ) );
    m_types.insert( PropertyTypeMap::value_type( "int",      PROPERTY_TYPE_INT ) );
    m_types.insert( PropertyTypeMap::value_type( "name",     PROPERTY_TYPE_NAME ) );
    m_types.insert( PropertyTypeMap::value_type( "point",    PROPERTY_TYPE_POINT ) );
    m_types.insert( PropertyTypeMap::value_type( "size",     PROPERTY_TYPE_SIZE ) );
    m_types.insert( PropertyTypeMap::value_type( "string",   PROPERTY_TYPE_STRING ) );
    m_types.insert( PropertyTypeMap::value_type( "style",    PROPERTY_TYPE_STYLE ) );
    m_types.insert( PropertyTypeMap::value_type( "exstyle",  PROPERTY_TYPE_STYLE ) );
    m_types.insert( PropertyTypeMap::value_type( "text",     PROPERTY_TYPE_TEXT ) );
    m_types.insert( PropertyTypeMap::value_type( "url",      PROPERTY_TYPE_URL ) );
}

PropertyType ObjectDBManager::GetPropertyType( const wxString &tagname ) const
{
    PropertyTypeMap::const_iterator it = m_types.find( tagname );

    if ( it != m_types.end() )
        return it->second;

    return PROPERTY_TYPE_UNKNOWN;
}

bool ObjectDBManager::AddDatabase( const wxString &category )
{
    ObjectDB db( new XMLDataBase( category ) );

    if ( !db->Load() )
    {
        db.reset();
        return false;
    }

    m_databases.insert( ObjectDBMap::value_type( category, db ) );
    return true;
}

const wxString ObjectDBManager::GetBasePath() const
{
#ifdef __WXMSW__
    return wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') +
                wxFILE_SEP_PATH + "plugins" + wxFILE_SEP_PATH;
#else
    return wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') +
                wxFILE_SEP_PATH + wxTheApp->GetAppName().Lower().Trim( false ) +
                wxFILE_SEP_PATH + "plugins" + wxFILE_SEP_PATH;
#endif
}

ClassInfo ObjectDBManager::GetClassInfo( const wxString &className,
                                        const wxString &dataBaseName )
{
    ObjectDBMap::const_iterator it = m_databases.find( dataBaseName );

    if ( it != m_databases.end() )
        return it->second->GetClassInfo( className );

    return ClassInfo();
}
