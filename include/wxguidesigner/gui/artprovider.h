///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/artprovider.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/17
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_ARTPROVIDER_H__
#define __WXGUIDESIGNER_GUI_ARTPROVIDER_H__

#include <map>
#include <utility>
#include <vector>

class wxBitmap;
class wxString;
class wxFileName;
class wxImageList;

using namespace std;

// item label (e.g. 'wxFrame') and imagelist index
typedef pair< wxString, int >                       wxGDIconInfo;
typedef vector< wxGDIconInfo >                      wxGDIconInfos;

// group name and label e.g. 'toplevel' 'Top Level'
typedef pair< wxString, wxString >                  wxGDIconGroupNames;
typedef pair< wxGDIconGroupNames, int >             wxGDIconGroupInfo;
typedef pair< wxGDIconGroupInfo, wxGDIconInfos >    wxGDIconGroup;
typedef vector< wxGDIconGroup >                     wxGDIconGroups;

// category name e.g. "controls"
typedef pair< wxString, wxGDIconGroups >            wxGDIconCategory;
typedef map< wxString, wxGDIconGroups >             wxGDIconCategoryMap;

namespace wxGDArtProvider
{
    void Load();
    void Unload();

    size_t   GetGroupCount        ( const wxString &categoryName );
    wxString GetGroupName         ( const wxString &categoryName,
                                    size_t groupIndex );

    wxString GetGroupLabel        ( const wxString &categoryName,
                                    size_t groupIndex );

    int      GetGroupImageListIndex( const wxString &categoryName,
                                    size_t groupIndex );

    size_t   GetItemCount         ( const wxString &categoryName,
                                    size_t groupIndex );

    wxString GetItemLabel         ( const wxString &categoryName,
                                    size_t groupIndex, size_t itemIndex );

    int      GetItemImageListIndex( const wxString &categoryName,
                                    const wxString &itemName );
    int      GetItemImageListIndex( const wxString &categoryName,
                                    size_t groupIndex, size_t itemIndex );
    int      GetItemImageListSize();

    bool     CategoryExists       ( const wxString &categoryName );

    bool     GroupExists          ( const wxString &categoryName,
                                    size_t groupIndex );

    bool     ItemExists           ( const wxString &categoryName,
                                    size_t groupIndex, size_t itemIndex );

    bool     LoadXML              ( const wxFileName &xmlFileName );
    wxBitmap LoadBitmap           ( const wxString   &categoryName,
                                    const wxString   &groupName,
                                    const wxString   &itemLabel = wxEmptyString );

    extern wxGDIconCategoryMap  IconCategories;
    extern wxImageList          *ItemsImageList;
    extern wxImageList          *GroupsImageList;
};

#endif //__WXGUIDESIGNER_GUI_ARTPROVIDER_H__
