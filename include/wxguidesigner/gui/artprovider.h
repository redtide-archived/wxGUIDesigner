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

#include <boost/tr1/memory.hpp>
#include <map>
#include <utility>
#include <vector>

#include <wx/bitmap.h>
#include <wx/string.h>

using namespace std;
//-----------------------------------------------------------------------------
//  IconGroup
//-----------------------------------------------------------------------------
typedef pair< wxString, wxBitmap > IconInfo;
typedef vector< IconInfo >         Items;

class IconGroup
{
public:
    IconGroup( const wxString &name, const wxString &label, const wxBitmap &bitmap )
            : m_name( name ), m_label( label ), m_bitmap( bitmap ) {}

    ~IconGroup() { m_items.clear(); }

    wxString GetName()   const { return m_name; }
    wxString GetLabel()  const { return m_label; }
    wxBitmap GetBitmap() const { return m_bitmap; }

    size_t   GetItemCount()    { return m_items.size(); }
    wxString GetItemLabel( size_t index ) const;
    wxBitmap GetItemBitmap( size_t index ) const;

    void AddItem( IconInfo item ) { m_items.push_back( item ); }

private:
    wxString m_name;
    wxString m_label;
    wxBitmap m_bitmap;
    Items    m_items;
};
//-----------------------------------------------------------------------------
//  wxGDArtProvider
//-----------------------------------------------------------------------------

class wxGDArtProvider
{
public:
    ~wxGDArtProvider();

    static void Load();

    static bool SelectCategory( const wxString &category );

    static size_t   GetGroupCount();
    static wxString GetGroupName   ( size_t index );
    static wxString GetGroupLabel  ( size_t index );
    static wxBitmap GetGroupBitmap ( size_t index );

    static size_t   GetItemCount   ( size_t groupIndex );
    static wxString GetItemLabel   ( size_t groupIndex, size_t itemIndex );
    static wxBitmap GetItemBitmap  ( size_t groupIndex, size_t itemIndex );

private:
    wxGDArtProvider( const wxGDArtProvider& );
    wxGDArtProvider& operator=( wxGDArtProvider const& );

    static bool     CheckIconDB();
    static bool     LoadXML( const wxString &path );
    static wxBitmap LoadBitmap( const wxString &category, const wxString &group,
                         const wxString &item = wxEmptyString );

    typedef class tr1::shared_ptr< IconGroup > IconGroupPtr;
    typedef vector< IconGroupPtr >             IconGroups;
    typedef map< wxString, IconGroups >        IconCategories;

    static wxString       m_sel;
    static IconCategories m_cts;
};

#endif //__WXGUIDESIGNER_GUI_ARTPROVIDER_H__
