///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/iconprovider.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/17
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/utils.h"

#include <wx/artprov.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/xml/xml.h>

#include <wx/log.h>
//=============================================================================
//  IconGroup
//=============================================================================
wxString IconGroup::GetItemLabel( size_t index ) const
{
    if( index < m_items.size() )
        return m_items.at( index ).first;

    return wxEmptyString;
}

wxBitmap IconGroup::GetItemBitmap( size_t index ) const
{
    if( index < m_items.size() )
        return m_items.at( index ).second;

    return wxNullBitmap;
}
//=============================================================================
//  wxGDArtProvider
//=============================================================================
wxGDArtProvider::wxGDArtProvider()
:
m_sel( wxEmptyString )
{
    // Init the only image handler needed if not loaded already from some IDE
    if(!wxImage::FindHandler( wxBITMAP_TYPE_PNG ))
        wxImage::AddHandler( new wxPNGHandler );

    Init();
}

wxGDArtProvider::~wxGDArtProvider()
{
    m_cts.erase( m_cts.begin(), m_cts.end() );
}

size_t wxGDArtProvider::GetGroupCount()
{
    return m_cts.at(m_sel).size();
}


void wxGDArtProvider::Init()
{
    wxString dbPath = GetDataBasePath();

    if( !wxDirExists( dbPath ) )
        return;

    wxDir dbDir( dbPath );
    if( !dbDir.IsOpened() )
        return;

    // E.g. 'controls.xml'
    wxString xmlFile;
    bool haveXml = dbDir.GetFirst( &xmlFile, "*.xml",
                                    wxDIR_FILES | wxDIR_HIDDEN );
    while( haveXml )
    {
        wxFileName xmlFileName( dbPath + wxFILE_SEP_PATH + xmlFile );
        if( !xmlFileName.IsAbsolute() )
            xmlFileName.MakeAbsolute();

        LoadXML( xmlFileName.GetFullPath() );

        haveXml = dbDir.GetNext( &xmlFile );
    }
}

bool wxGDArtProvider::LoadXML( const wxString &path )
{
    wxXmlDocument doc;
    if( !doc.Load( path ) )
        return false;

    wxXmlNode *rootNode = doc.GetRoot();
    wxString   catName  = rootNode->GetAttribute("name");

    if( rootNode->GetName() != "category" || catName.empty() )
        return false;

    m_sel = catName;

    wxXmlNode *groupNode = rootNode->GetChildren();
    while( groupNode )
    {
        wxString nodeName   = groupNode->GetName();
        wxString groupName  = wxEmptyString;
        wxString groupLabel = wxEmptyString;

        if( nodeName == "group" )
        {
            groupName = groupNode->GetAttribute("name");

            // Skip unnamed groups
            if( groupName.empty() )
            {
                groupNode = groupNode->GetNext();
                continue;
            }

            groupLabel = groupNode->GetAttribute("label");
            if( groupLabel.empty() )
                groupLabel = groupName.Capitalize();

            wxBitmap bmp = LoadBitmap( catName, groupName );
            IconGroupPtr group( new IconGroup( groupName, groupLabel, bmp ) );

            bool wasOk = false;

            wxXmlNode *itemNode = groupNode->GetChildren();
            while( itemNode )
            {
                wxString itemName  = itemNode->GetNodeContent();
                bool     separator = itemNode->GetName() == "separator";

                if( separator )
                {
                    if( wasOk )
                    {
                        itemName = "-";
                    }
                    else
                    {
                        separator = false; // Avoid unneeded separator
                    }
                }
                else if( catName == "controls" )
                {
                    wasOk = ClassInfoDB::Get()->ClassInfoExists( itemName );
                    if( !wasOk )
                    {
//                      wxLogDebug("Discarding %s", itemName);
                    }
                }
                else
                {
                    wasOk = (itemNode->GetName() == "item") && !itemName.empty();
                }

                if( !wasOk && !separator )
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

    if( m_cts[m_sel].empty() )
        return false;

    return true;
}

wxBitmap wxGDArtProvider::LoadBitmap( const wxString &category,
                                   const wxString &group,
                                   const wxString &item )
{
    if( item == "-" )
        return wxNullBitmap;

    wxString path = GetDataBasePath() + wxFILE_SEP_PATH + category +
        wxFILE_SEP_PATH + group + wxFILE_SEP_PATH + "icons" + wxFILE_SEP_PATH;

    wxString bmpPath = path + group + ".png"; // ICONTYPE_GROUP

    if( !item.empty() ) // ICONTYPE_ITEM
    {
        // 'wxFrame' => 'frame.png'
        wxString bmpName  = item.Lower().Append(".png");
        bmpName.Replace("wx", "");
        bmpPath = path + bmpName;
    }

    if( wxFileExists( bmpPath ) )
    {
        wxBitmap bmp = wxBitmap( bmpPath, wxBITMAP_TYPE_PNG );
        if( bmp.IsOk() )
            return bmp;
    }

    return wxNullBitmap;
}

bool wxGDArtProvider::CheckIconDB()
{
    if( m_cts.empty() )
    {
        wxLogError("Empty icon database, check the\n%s\n database directory",
                    GetDataBasePath() );
        return false;
    }

    return true;
}
//=============================================================================
//  wxGDArtProvider public functions
//=============================================================================
bool wxGDArtProvider::SelectCategory( const wxString &category )
{
    IconCategories::iterator it = m_cts.find( category );

    if( it != m_cts.end() )
    {
        m_sel = category;
        return true;
    }

    return false;
}

wxBitmap wxGDArtProvider::GetGroupBitmap( size_t index )
{
    if( CheckIconDB() && ( index < m_cts.at(m_sel).size() ) )
    {
        wxBitmap bmp = m_cts.at(m_sel)[index]->GetBitmap();
        if( bmp.IsOk() )
            return bmp;
    }

    return wxArtProvider::GetBitmap
                    ( wxART_MISSING_IMAGE, wxART_OTHER, wxSize( 16, 16 ) );
}

wxString wxGDArtProvider::GetGroupLabel( size_t index )
{
    if( CheckIconDB() && ( index < m_cts.at(m_sel).size() ) )
        return m_cts.at(m_sel)[index]->GetLabel();

    return wxEmptyString;
}

wxString wxGDArtProvider::GetGroupName( size_t index )
{
    if( CheckIconDB() && ( index < m_cts.at(m_sel).size() ) )
        return m_cts.at(m_sel)[index]->GetName();

    return wxEmptyString;
}

size_t wxGDArtProvider::GetItemCount( size_t groupIndex )
{
    if( CheckIconDB() && groupIndex < m_cts.at(m_sel).size() )
    {
        return m_cts.at(m_sel)[groupIndex]->GetItemCount();
    }

    return wxNOT_FOUND;
}

wxString wxGDArtProvider::GetItemLabel( size_t groupIndex, size_t itemIndex )
{
    if
    (
        CheckIconDB() && (groupIndex < m_cts.at(m_sel).size())
        && (itemIndex < m_cts.at(m_sel)[groupIndex]->GetItemCount())
    )
        return m_cts.at(m_sel)[groupIndex]->GetItemLabel( itemIndex );

    return wxEmptyString;
}

wxBitmap wxGDArtProvider::GetItemBitmap( size_t groupIndex, size_t itemIndex )
{
    if
    (
        CheckIconDB() && ( groupIndex < m_cts.at(m_sel).size() ) &&
        ( itemIndex < m_cts.at(m_sel)[groupIndex]->GetItemCount() )
    )
    {
        wxBitmap bmp = m_cts.at(m_sel)[groupIndex]->GetItemBitmap( itemIndex );
        if( bmp.IsOk() )
            return bmp;
    }

    return wxArtProvider::GetBitmap
                    ( wxART_MISSING_IMAGE, wxART_OTHER, wxSize( 22, 22 ) );
}
