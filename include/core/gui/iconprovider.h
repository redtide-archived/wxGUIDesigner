///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/artprovider.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/17
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_GUI_ARTPROVIDER_H__
#define __CORE_GUI_ARTPROVIDER_H__

#include "core/dllimpexp.h"

#include <boost/tr1/memory.hpp>
#include <map>
#include <utility>
#include <vector>

#include <wx/bitmap.h>
#include <wx/filename.h>

enum IconType
{
    ICONTYPE_ITEM,
    ICONTYPE_GROUP
};
//-----------------------------------------------------------------------------
//  IconGroup
//-----------------------------------------------------------------------------

typedef std::pair< wxString, wxBitmap > IconInfo;
typedef std::vector< IconInfo >         Items;

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
//  IconProvider
//-----------------------------------------------------------------------------

class DLLIMPEXP_CORE IconProvider
{
public:
    static IconProvider *Get();
    void Free();

    bool SelectCategory( const wxString &category );

    size_t   GetGroupCount() { return m_cts.at(m_sel).size(); }
    wxString GetGroupName( size_t index ) const;
    wxString GetGroupLabel( size_t index ) const;
    wxBitmap GetGroupBitmap( size_t index ) const;

    size_t   GetItemCount( size_t groupIndex );
    wxString GetItemLabel( size_t groupIndex, size_t itemIndex ) const;
    wxBitmap GetItemBitmap( size_t groupIndex, size_t itemIndex ) const;

private:
    IconProvider() : m_sel( wxEmptyString ) { Init(); }
    ~IconProvider() { m_cts.erase( m_cts.begin(), m_cts.end() ); }

    IconProvider( const IconProvider& );
    IconProvider& operator=( IconProvider const& );

    void     Init();
    bool     CheckIconDB();
    bool     LoadXML( const wxString &path );
    wxBitmap LoadBitmap( const wxString &category, const wxString &group,
                         const wxString &item = wxEmptyString );

    typedef class std::tr1::shared_ptr< IconGroup > IconGroupPtr;
    typedef std::vector< IconGroupPtr >             IconGroups;
    typedef std::map< wxString, IconGroups >        IconCategories;

    wxString       m_sel;
    IconCategories m_cts;

    static IconProvider *ms_instance;
};

#endif //__CORE_GUI_ARTPROVIDER_H__
