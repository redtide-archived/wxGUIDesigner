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

namespace wxGD
{
// item label (e.g. 'wxFrame') and imagelist index
typedef std::pair< wxString, int >              IconInfo;
typedef std::vector< IconInfo >                 IconInfos;

// group name and label e.g. 'toplevel' 'Top Level'
typedef std::pair< wxString, wxString >         IconGroupNames;
typedef std::pair< IconGroupNames, int >        IconGroupInfo;
typedef std::pair< IconGroupInfo, IconInfos >   IconGroup;
typedef std::vector< IconGroup >                IconGroups;

// category name e.g. "controls"
typedef std::pair< wxString, IconGroups >       IconCategory;
typedef std::map< wxString, IconGroups >        IconCategoryMap;

namespace ArtProvider
{
    void Load ( const wxString &categoryName, wxImageList *smallImageList,
                wxImageList *largeImageList, bool useSmallIcons = false );
    void Unload();

    size_t   GetGroupCount        ( const wxString &categoryName );
    wxString GetGroupName         ( const wxString &categoryName,
                                    size_t groupIndex );

    wxString GetGroupLabel        ( const wxString &categoryName,
                                    size_t groupIndex );

    int      GetGroupImageListIndex( const wxString &categoryName,
                                     const wxString &groupName );
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

    bool     LoadXML              ( const wxFileName &xmlFileName,
                                    wxImageList *smallImageList,
                                    wxImageList *largeImageList,
                                    bool useSmallIcons );

    wxBitmap LoadBitmap           ( const wxString &categoryName,
                                    const wxString &groupName,
                                    const wxString &itemLabel = wxEmptyString );

    extern IconCategoryMap Categories;
};
};

#endif //__WXGUIDESIGNER_GUI_ARTPROVIDER_H__
