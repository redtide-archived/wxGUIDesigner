///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/palette.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/23
// RCS-ID:      $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "handlers/palette.h"
#include "manager.h"

PaletteHandler::PaletteHandler( wxNotebook *owner )
{
    m_palette = owner;
    m_palette->SetImageList( new wxImageList( 16, 16 ) );
}

void PaletteHandler::OnToolClicked( wxCommandEvent &event )
{
#ifndef USE_AUIBAR
    wxToolBar *tb = wxDynamicCast( event.GetEventObject(), wxToolBar );
#else
    wxAuiToolBar *tb = wxDynamicCast( event.GetEventObject(), wxAuiToolBar );
#endif
    if ( tb )
    {
        wxWindowID toolId = event.GetId();
        wxString   clsNme = tb->GetToolShortHelp( toolId );

        wxGUIDesigner::Get()->CreateObject( clsNme );
    }
}

#ifndef USE_AUIBAR
    wxToolBar *
#else
    wxAuiToolBar *
#endif
PaletteHandler::AddGroup( const wxString &name, const wxBitmap &bitmap )
{
#ifndef USE_AUIBAR
    wxToolBar *tb = new wxToolBar( m_palette, wxID_ANY );
#else
    wxAuiToolBar *tb = new wxAuiToolBar( m_palette, wxID_ANY );
#endif

    m_palette->GetImageList()->Add( bitmap );

    m_toolbars.push_back( tb );
    m_palette->AddPage( tb, name, false, m_palette->GetPageCount() );

    return tb;
}

void PaletteHandler::OnPluginLoaded( PluginEvent &event )
{
    Plugin *plugin = wxDynamicCast( event.GetEventObject(), Plugin );

    if ( plugin )
    {
    #ifndef USE_AUIBAR
        wxToolBar *
    #else
        wxAuiToolBar *
    #endif
        tb = AddGroup( plugin->GetName(), plugin->GetBitmap() );

        wxVector< PluginItem * > items = plugin->GetItems();

        for( wxVector< PluginItem * >::iterator it = items.begin();
            it != items.end(); ++it )
        {
            PluginItem *item = (*it);

            tb->AddTool(wxID_ANY, item->GetName(), item->GetBitmap(), item->GetName());
        }
    }
}
