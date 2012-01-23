///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/iconprovider.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/17
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/gui/iconprovider.h"
#include "core/object/database.h"
#include "core/utils.h"

#include <wx/artprov.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/xml/xml.h>

#include <wx/log.h>

IconProvider *IconProvider::ms_instance = NULL;

IconProvider *IconProvider::Get()
{
    if ( !ms_instance )
        ms_instance = new IconProvider;

    return ms_instance;
}

void IconProvider::Free()
{
    if ( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void IconProvider::Init()
{
    wxString dbPath = GetDataBasePath();

    if ( !wxDirExists( dbPath ) )
        return;

    wxDir dbDir( dbPath );
    if ( !dbDir.IsOpened() )
        return;

    // E.g. 'controls.xml'
    wxString xmlFile;
    bool haveXml = dbDir.GetFirst( &xmlFile, "*.xml",
                                    wxDIR_FILES | wxDIR_HIDDEN );
    while ( haveXml )
    {
        wxFileName xmlFileName( dbPath + wxFILE_SEP_PATH + xmlFile );
        if ( !xmlFileName.IsAbsolute() )
            xmlFileName.MakeAbsolute();

        LoadXML( xmlFileName.GetFullPath() );

        haveXml = dbDir.GetNext( &xmlFile );
    }
}

wxBitmap IconProvider::GetBitmap( const wxString &label, IconType type ) const
{
    if ( ms_instance->CheckIconDB() )
    {
        for ( IconGroups::const_iterator it = m_cts.at(m_sel).begin();
                                        it != m_cts.at(m_sel).end(); it++ )
        {
            if ( type == ICONTYPE_GROUP && (*it)->GetLabel() == label )
            {
                return (*it)->GetBitmap();
            }
            else // if type == ICONTYPE_ITEM
            {
                for ( size_t i = 0; i < (*it)->GetItemCount(); i++ )
                {
                    if ( (*it)->GetItemLabel( i ) == label )
                        return (*it)->GetItemBitmap( i );
                }
            }
        }
    }

    wxSize size = wxSize( 22, 22 ); // ICONTYPE_ITEM

    if ( type == ICONTYPE_GROUP )
        size = wxSize( 16, 16 );

    return wxArtProvider::GetBitmap( wxART_MISSING_IMAGE, wxART_OTHER, size );
}

bool IconProvider::LoadXML( const wxString &path )
{
    wxXmlDocument doc;
    if ( !doc.Load( path ) )
        return false;

    wxXmlNode *rootNode = doc.GetRoot();
    wxString   catName  = rootNode->GetAttribute("name");

    if ( rootNode->GetName() != "category" || catName.empty() )
        return false;

    m_sel = catName;

    wxXmlNode *groupNode = rootNode->GetChildren();
    while ( groupNode )
    {
        wxString nodeName   = groupNode->GetName();
        wxString groupName  = wxEmptyString;
        wxString groupLabel = wxEmptyString;

        if ( nodeName == "group" )
        {
            groupName = groupNode->GetAttribute("name");

            // Skip unnamed groups
            if ( groupName.empty() )
            {
                groupNode = groupNode->GetNext();
                continue;
            }

            groupLabel = groupNode->GetAttribute("label");
            if ( groupLabel.empty() )
                groupLabel = groupName.Capitalize();

            wxBitmap bmp = LoadBitmap( catName, groupName );
            IconGroupPtr group( new IconGroup( groupName, groupLabel, bmp ) );

            wxXmlNode *itemNode = groupNode->GetChildren();
            while ( itemNode )
            {
                wxString itemName = itemNode->GetNodeContent();

                if ( itemNode->GetName() == "separator" )
                {
                    itemName = "-";
                }
                else if ( (itemNode->GetName() != "item") || itemName.empty() )
                {
                    itemNode = itemNode->GetNext();
                    continue;
                }

                bmp = LoadBitmap( catName, groupName, itemName );
                IconInfo item = std::make_pair( itemName, bmp );

                group->AddItem( item );

                itemNode = itemNode->GetNext();
            }

            m_cts[m_sel].push_back( group );
        }

        groupNode = groupNode->GetNext();
    }

    if ( m_cts[m_sel].empty() )
        return false;

    return true;
}

wxBitmap IconProvider::LoadBitmap( const wxString &category,
                                   const wxString &group,
                                   const wxString &item )
{
    if ( item == "-" )
        return wxNullBitmap;

    wxString path = GetDataBasePath() + wxFILE_SEP_PATH + category +
        wxFILE_SEP_PATH + group + wxFILE_SEP_PATH + "icons" + wxFILE_SEP_PATH;

    wxString bmpPath = path + group + ".png"; // ICONTYPE_GROUP

    if ( !item.empty() ) // ICONTYPE_ITEM
    {
        // 'wxFrame' => 'frame.png'
        wxString bmpName  = item.Lower().Append(".png");
        bmpName.Replace( "wx", "" );

        bmpPath = path + wxFILE_SEP_PATH + bmpName;
    }

    if ( wxFileExists( bmpPath ) )
    {
        wxBitmap bmp = wxBitmap( bmpPath, wxBITMAP_TYPE_PNG );
        if ( bmp.IsOk() )
            return bmp;
    }

    return wxNullBitmap;
}

bool IconProvider::CheckIconDB()
{
    if ( m_cts.empty() )
    {
        wxLogError("Empty icon database, check the\n%s\n database directory",
                    GetDataBasePath() );
        return false;
    }

    return true;
}
