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

#include <wx/log.h>

#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/utils.h"

wxGD::ArtProvider::IconCategoryMap wxGD::ArtProvider::Categories;

void wxGD::ArtProvider::Load  ( const wxString &categoryName,
                                wxImageList *smallImageList,
                                wxImageList *largeImageList, bool useSmallIcons )
{
    // No duplicates
    if( CategoryExists( categoryName ) )
        return;

    wxString dbPath = GetDataBasePath();

    if( !wxDirExists( dbPath ) )
        return;

    wxDir dbDir( dbPath );
    if( !dbDir.IsOpened() )
        return;

    // E.g. 'controls.xml'
    wxString   xmlFilePath = dbPath  + wxFILE_SEP_PATH + categoryName + ".xml";
    wxFileName xmlFileName( xmlFilePath ); 

    if( xmlFileName.FileExists() )
    {
        if(!xmlFileName.IsAbsolute())
            xmlFileName.MakeAbsolute();

        LoadXML( xmlFileName, smallImageList, largeImageList, useSmallIcons );
    }
}

void wxGD::ArtProvider::Unload()
{
    Categories.clear();
}

size_t wxGD::ArtProvider::GetGroupCount( const wxString &category )
{
    return Categories[category].size();
}

bool wxGD::ArtProvider::LoadXML(const wxFileName &xmlFileName,
                                wxImageList *smallImageList,
                                wxImageList *largeImageList,
                                bool useSmallIcons)
{
    wxXmlDocument doc;
    if( !doc.Load( xmlFileName.GetFullPath() ) )
        return false;

    IconGroups  groups;
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
            grpsImgIdx = smallImageList->Add( bmp );

        IconGroupNames  groupNames( groupName, groupLabel );
        IconGroupInfo   groupInfo( groupNames, grpsImgIdx );
        IconInfos       iconInfos;

        bool wasOk = false;

        wxXmlNode *itemNode = groupNode->GetChildren();
        while( itemNode )
        {
            wxString itemName  = itemNode->GetNodeContent();
            bool     separator = itemNode->GetName() == "separator";

            if( separator )
            {
                itemName = "-";
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

            int itmsImgIdx = 0;
            bmp = LoadBitmap( xmlFileName.GetName(), groupName, itemName );

            if( bmp.IsOk() )
            {
                if( useSmallIcons )
                    itmsImgIdx = smallImageList->Add( bmp );
                else
                    itmsImgIdx = largeImageList->Add( bmp );
            }

            IconInfo iconInfo( itemName, itmsImgIdx );
            iconInfos.push_back( iconInfo );

            itemNode = itemNode->GetNext();
        }

        // IconGroups can have no items
        IconGroup group( groupInfo, iconInfos );
        groups.push_back( group );
        groupNode = groupNode->GetNext();
    }

    if( groups.empty() )
        return false;

    IconCategory category( xmlFileName.GetName(), groups );

    Categories.insert( category );
    return true;
}

wxBitmap wxGD::ArtProvider::LoadBitmap( const wxString &category,
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

bool wxGD::ArtProvider::CategoryExists( const wxString &categoryName )
{
    IconCategoryMap::iterator category = Categories.find( categoryName );
    if( category != Categories.end() )
        return true;

    return false;
}

bool wxGD::ArtProvider::GroupExists(const wxString &categoryName,
                                    size_t groupIndex)
{
    return
    (
        CategoryExists( categoryName ) &&
        (groupIndex < Categories[categoryName].size())
    );
}

bool wxGD::ArtProvider::ItemExists( const wxString &categoryName,
                                    size_t groupIndex, size_t itemIndex )
{
    return
    (
        GroupExists( categoryName, groupIndex ) &&
        (itemIndex < Categories[categoryName].at(groupIndex).second.size())
    );
}

wxString wxGD::ArtProvider::GetGroupName  ( const wxString &categoryName,
                                            size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex ) )
        return Categories[categoryName].at(groupIndex).first.first.first;

    return wxEmptyString;
}

wxString wxGD::ArtProvider::GetGroupLabel ( const wxString &categoryName,
                                            size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex ) )
        return Categories[categoryName].at(groupIndex).first.first.second;

    return wxEmptyString;
}

int wxGD::ArtProvider::GetGroupImageListIndex ( const wxString &categoryName,
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

    return 0; // wxART_MISSING_IMAGE
}

int wxGD::ArtProvider::GetGroupImageListIndex ( const wxString &categoryName,
                                                size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex )  )
        return Categories[categoryName].at(groupIndex).first.second;

    return 0; // wxART_MISSING_IMAGE
}

size_t wxGD::ArtProvider::GetItemCount( const wxString &categoryName,
                                        size_t groupIndex )
{
    if( GroupExists( categoryName, groupIndex ) )
        return Categories[categoryName].at(groupIndex).second.size();

    return 0; // wxART_MISSING_IMAGE
}

wxString wxGD::ArtProvider::GetItemLabel  ( const wxString &categoryName,
                                            size_t groupIndex, size_t itemIndex )
{
    if( ItemExists( categoryName, groupIndex, itemIndex ) )
        return
        Categories[categoryName].at(groupIndex).second.at(itemIndex).first;

    return wxEmptyString;
}

int wxGD::ArtProvider::GetItemImageListIndex  ( const wxString &categoryName,
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

    return 0; // wxART_MISSING_IMAGE
}

int wxGD::ArtProvider::GetItemImageListIndex  ( const wxString &categoryName,
                                                size_t          groupIndex,
                                                size_t          itemIndex )
{
    if( ItemExists( categoryName, groupIndex, itemIndex ) )
        return Categories[categoryName].at(groupIndex).second.at(itemIndex).second;

    return 0; // wxART_MISSING_IMAGE
}

int wxGD::ArtProvider::GetItemImageListSize()
{
    // Load icons preferences
    bool smallIcons = false;
    int  size       = 22;
    wxConfigBase::Get()->Read( "gui/smallicons", &smallIcons );

    if( smallIcons )
        size = 16;

    return size;
}
