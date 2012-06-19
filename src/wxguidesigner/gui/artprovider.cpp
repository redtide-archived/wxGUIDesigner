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
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/utils.h"

using namespace wxGDArtProvider;

wxGDIconCategoryMap wxGDArtProvider::categories;

void wxGDArtProvider::Load()
{
    // Init the only image handler needed if not loaded already
    // from an external application
    if(!wxImage::FindHandler( wxBITMAP_TYPE_PNG ))
        wxImage::AddHandler( new wxPNGHandler);

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

        LoadXML( xmlFileName );

        haveXml = dbDir.GetNext( &xmlFile );
    }
}

void wxGDArtProvider::Unload()
{
    categories.clear();
}

size_t wxGDArtProvider::GetGroupCount( const wxString &category )
{
    return categories[category].size();
}

bool wxGDArtProvider::LoadXML ( const wxFileName &xmlFileName )
{
    wxXmlDocument doc;
    if( !doc.Load( xmlFileName.GetFullPath() ) )
        return false;

    wxXmlNode *groupNode = doc.GetRoot()->GetChildren();

    wxGDIconGroups   groups;

    while( groupNode )
    {
        wxString groupName  = groupNode->GetName();
        wxString groupLabel = groupNode->GetAttribute("label");

        if( groupLabel.empty() )
            groupLabel = groupName.Capitalize();

        // TODO: avoid useless capitalized label insert
        wxBitmap bmp = LoadBitmap( xmlFileName.GetName(), groupName );
        wxGDIconGroupNames  groupNames( groupName, groupLabel );
        wxGDIconGroupInfo   groupInfo( groupNames, bmp );
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
            wxGDIconInfo iconInfo( itemName, bmp );

            iconInfos.push_back( iconInfo );

            itemNode = itemNode->GetNext();
        }

        if( !iconInfos.empty() )
        {
            wxGDIconGroup group( groupInfo, iconInfos );
            groups.push_back( group );
        }

        groupNode = groupNode->GetNext();
    }

    if( groups.empty() )
        return false;

    wxGDIconCategory category( xmlFileName.GetName(), groups );

    categories.insert( category );
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
    wxGDIconCategoryMap::iterator category = categories.find( categoryName );
    if( category != categories.end() )
        return true;

    return false;
}

bool wxGDArtProvider::GroupExists ( const wxString &categoryName,
                                    size_t groupIndex )
{
    return
    (
        CategoryExists( categoryName ) &&
        (groupIndex < categories[categoryName].size())
    );
}

bool wxGDArtProvider::ItemExists  ( const wxString &categoryName,
                                    size_t groupIndex, size_t itemIndex )
{
    return
    (
        GroupExists( categoryName, groupIndex ) &&
        (itemIndex < categories[categoryName].at(groupIndex).second.size())
    );
}

wxString wxGDArtProvider::GetGroupName( const wxString &category, size_t index )
{
    if( GroupExists(category, index) )
        return categories[category].at(index).first.first.first;

    return wxEmptyString;
}

wxString wxGDArtProvider::GetGroupLabel( const wxString &category, size_t index )
{
    if( GroupExists(category, index) )
        return categories[category].at(index).first.first.second;

    return wxEmptyString;
}

wxBitmap wxGDArtProvider::GetGroupBitmap( const wxString &category, size_t index )
{
    if( GroupExists( category, index )  )
    {
        wxBitmap bmp = categories[category].at(index).first.second;
        if( bmp.IsOk() )
            return bmp;
    }

    return wxArtProvider::GetBitmap
                    ( wxART_MISSING_IMAGE, wxART_OTHER, wxSize( 16, 16 ) );
}

size_t wxGDArtProvider::GetItemCount( const wxString &category, size_t index )
{
    if( GroupExists( category, index ) )
        return categories[category].at(index).second.size();

    return 0;
}

wxString wxGDArtProvider::GetItemLabel( const wxString &categoryName,
                                        size_t groupIndex, size_t itemIndex )
{
    if( ItemExists( categoryName, groupIndex, itemIndex ) )
        return
        categories[categoryName].at(groupIndex).second.at(itemIndex).first;

    return wxEmptyString;
}

wxBitmap wxGDArtProvider::GetItemBitmap( const wxString &categoryName,
                                        size_t groupIndex, size_t itemIndex )
{
    if( ItemExists( categoryName, groupIndex, itemIndex ) )
    {
        wxBitmap bmp =
        categories[categoryName].at(groupIndex).second.at(itemIndex).second;

        if( bmp.IsOk() )
            return bmp;
    }

    return wxArtProvider::GetBitmap
                    ( wxART_MISSING_IMAGE, wxART_OTHER, wxSize( 22, 22 ) );
}
