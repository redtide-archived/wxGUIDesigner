///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/artprovider.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/17
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_ARTPROVIDER_H__
#define __WXGUIDESIGNER_GUI_ARTPROVIDER_H__

#include <map>

#include <wx/bitmap.h>
#include <wx/string.h>

using namespace std;

class wxGDArtProvider
{
public:
    wxGDArtProvider();
    ~wxGDArtProvider();

    static size_t   GetGroupCount ( const wxString &categoryName );

    static wxString GetGroupLabel ( const wxString &categoryName,
                                    const wxString &groupName );

    static wxBitmap GetGroupBitmap( const wxString &categoryName,
                                    const wxString &groupName );

    static size_t   GetItemCount  ( const wxString &categoryName,
                                    const wxString &groupName );

    static wxString GetItemLabel  ( const wxString &categoryName,
                                    const wxString &groupName,
                                    const wxString &itemName );

    static wxBitmap GetItemBitmap ( const wxString &categoryName,
                                    const wxString &groupName,
                                    const wxString &itemName );
private:
    wxGDArtProvider( const wxGDArtProvider& );
    wxGDArtProvider& operator=( wxGDArtProvider const& );

    static bool     CheckIconDB();
    static bool     LoadXML   ( const wxString &filePath );
    static wxBitmap LoadBitmap( const wxString &categoryName,
                                const wxString &groupName,
                                const wxString &itemName );
    // icon name and bitmap
    typedef pair< wxString, wxBitmap > wxGDIcon;

    // group name, icon name and bitmap
    typedef map< wxString, wxGDIcon > wxGDIconGroups;
};

#endif //__WXGUIDESIGNER_GUI_ARTPROVIDER_H__
