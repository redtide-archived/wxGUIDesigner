///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/artprovider.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/17
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/config.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/imaglist.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/utils.h"

using namespace wxGDArtProvider;

wxGDIconCategoryMap  wxGDArtProvider::IconCategories;
wxImageList         *wxGDArtProvider::ItemsImageList  = NULL;
wxImageList         *wxGDArtProvider::GroupsImageList = NULL;

void wxGDArtProvider::Load()
{
    wxString dbPath = GetDataBasePath();

    if( !wxDirExists( dbPath ) )
        return;

    wxDir dbDir( dbPath );
    if( !dbDir.IsOpened() )
        return;

    // Init the only image handler needed if not loaded already
    // from an external application
    if(!wxImage::FindHandler( wxBITMAP_TYPE_PNG ))
        wxImage::AddHandler( new wxPNGHandler);

    GroupsImageList = new wxImageList( 16,16 );
    ItemsImageList  = new wxImageList( 22,22 );

    // Add 'no image' replacement
    wxBitmap bmp = wxArtProvider::GetBitmap
                    ( wxART_MISSING_IMAGE, wxART_OTHER, wxSize( 16,16 ) );
    GroupsImageList->Add( bmp );

    bmp = wxArtProvider::GetBitmap
                    ( wxART_MISSING_IMAGE, wxART_OTHER, wxSize( 22,22 ) );
    ItemsImageList->Add( bmp );

    // E.g. 'controls.xml'
    wxString xmlFile;
    bool haveXml = dbDir.GetFirst( &xmlFile, "*.xml",
                                    wxDIR_FILES | wxDIR_HIDDEN );
    while( haveXml )
    {
        wxFileName xmlFileName( dbPath + wxFILE_SEP_PATH + xmlFile );
        if( !xmlFileName.IsAbsolute() )
            xmlFileName.MakeAbsolute();

        LoadXML( xmlFileName );

        haveXml = dbDir.GetNext( &xmlFile );
    }
}

void wxGDArtProvider::Unload()
{
    IconCategories.clear();

    if( ItemsImageList )
        delete ItemsImageList;

    if( GroupsImageList )
        delete GroupsImageList;
}

size_t wxGDArtProvider::GetGroupCount( const wxString &category )
{
    return IconCategories[category].size();
}

bool wxGDArtProvider::LoadXML ( const wxFileName &xmlFileName )
{
    wxXmlDocument doc;
    if( !doc.Load( xmlFileName.GetFullPath() ) )
        return false;

    wxGDIconGroups  groups;
    wxXmlNode       *groupNode = doc.GetRoot()->GetChildren();

    while( groupNode )
    {
        wxString groupName  = groupNode->GetName();
        wxString groupLabel = groupNode->GetAttribute("label");

        if( groupLabel.empty() )
            groupLabel = groupName.Capitalize();

        // TODO: avoid useless capitalized label insert
        wxBitmap bmp = LoadBitmap( xmlFileName.GetName(), groupName );

        int grpsImgIdx = 0;
        if( bmp.IsOk() )
            grpsImgIdx = GroupsImageList->Add( bmp );

        wxGDIconGroupNames  groupNames( groupName, groupLabel );
        wxGDIconGroupInfo   groupInfo( groupNames, grpsImgIdx );
        wxGDIconInfos       iconInfos;

        bool wasOk = false;

        wxXmlNode *itemNode = groupNode->GetChildren();
        while( itemNode )
        {
            wxString itemName  = itemNode->GetNodeContent();
            bool     separator = itemNode->GetName() == "separator";

            if( separator )
            {
                itemName = "-";
            }/*
            else if( xmlFileName.GetName() == "controls" )
            {
                wasOk = ClassInfoDB::Get()->ClassInfoExists( itemName );
                if( !wasOk )
//                  wxLogDebug("Discarding %s", itemName);
            }*/
            else
            {
                wasOk = (itemNode->GetName() == "item") && !itemName.empty();
            }

            if( !wasOk && !separator )
            {
                itemNode = itemNode->GetNext();
                continue;
            }

            bmp = LoadBitmap( xmlFileName.GetName(), groupName, itemName );

            int itmsImgIdx = 0;
            if( bmp.IsOk() )
                itmsImgIdx = ItemsImageList->Add( bmp );

            wxGDIconInfo iconInfo( itemName, itmsImgIdx );

            iconInfos.push_back( iconInfo );

            itemNode = itemNode->GetNext();
        }

        // IconGroups can have no items
        wxGDIconGroup group( groupInfo, iconInfos );
        groups.push_back( group );
        groupNode = groupNode->GetNext();
    }

    if( groups.empty() )
        return false;

    wxGDIconCategory category( xmlFileName.GetName(), groups );

    IconCategories.insert( category );
    return true;
}

wxBitmap wxGDArtProvider::LoadBitmap  ( const wxString &category,
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

//=============================================================================
//  wxGDArtProvider public functions
//=============================================================================
bool wxGDArtProvider::CategoryExists( const wxString &categoryName )
{
    wxGDIconCategoryMap::iterator category = IconCategories.find( categoryName );
    if( category != IconCategories.end() )
        return true;

    return false;
}

bool wxGDArtProvider::GroupExists ( const wxString &categoryName,
                                    size_t groupIndex )
{
    return
    (
        CategoryExists( categoryName ) &&
        (groupIndex < IconCategories[categoryName].size())
    );
}

bool wxGDArtProvider::ItemExists  ( const wxString &categoryName,
                                    size_t groupIndex, size_t itemIndex )
{
    return
    (
        GroupExists( categoryName, groupIndex ) &&
        (itemIndex < IconCategories[categoryName].at(groupIndex).second.size())
    );
}

wxString wxGDArtProvider::GetGroupName( const wxString &categoryName,
                                        size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex ) )
        return IconCategories[categoryName].at(groupIndex).first.first.first;

    return wxEmptyString;
}

wxString wxGDArtProvider::GetGroupLabel( const wxString &categoryName,
                                         size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex ) )
        return IconCategories[categoryName].at(groupIndex).first.first.second;

    return wxEmptyString;
}

int wxGDArtProvider::GetGroupImageListIndex( const wxString &categoryName,
                                             const wxString &groupName )
{
    if( CategoryExists( categoryName ) )
    {
        for( size_t g = 0; g < GetGroupCount( categoryName ); g++ )
        {
            if( GetGroupName( categoryName, g ) == groupName )
                return GetGroupImageListIndex( categoryName, g );
        }
    }

    return 0;
}

int wxGDArtProvider::GetGroupImageListIndex( const wxString &categoryName,
                                             size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex )  )
        return IconCategories[categoryName].at(groupIndex).first.second;

    return 0; // wxART_MISSING_IMAGE
}

size_t wxGDArtProvider::GetItemCount  ( const wxString &categoryName,
                                        size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex ) )
        return IconCategories[categoryName].at(groupIndex).second.size();

    return 0;
}

wxString wxGDArtProvider::GetItemLabel( const wxString &categoryName,
                                        size_t groupIndex, size_t itemIndex )
{
    if( ItemExists( categoryName, groupIndex, itemIndex ) )
        return
        IconCategories[categoryName].at(groupIndex).second.at(itemIndex).first;

    return wxEmptyString;
}

int wxGDArtProvider::GetItemImageListIndex( const wxString &categoryName,
                                            const wxString &itemName )
{
    if( CategoryExists( categoryName ) )
    {
        for( size_t g = 0; g < GetGroupCount( categoryName ); g++ )
        {
            for( size_t i = 0; i < GetItemCount( categoryName, g ); i++ )
            {
                if( GetItemLabel( categoryName, g, i ) == itemName )
                    return GetItemImageListIndex( categoryName, g, i );
            }
        }
    }

    return 0;
}

int wxGDArtProvider::GetItemImageListIndex( const wxString &categoryName,
                                            size_t groupIndex, size_t itemIndex )
{
    if( ItemExists( categoryName, groupIndex, itemIndex ) )
        return IconCategories[categoryName].at(groupIndex).second.at(itemIndex).second;

    return 0; // wxART_MISSING_IMAGE
}

int wxGDArtProvider::GetItemImageListSize()
{
    // Load icons preferences
    bool smallIcons = false;
    int  size       = 22;
    wxConfigBase::Get()->Read( "gui/smallicons", &smallIcons );

    if( smallIcons )
        size = 16;

    return size;
}
