////////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/palette.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/23
// Revision:    $Hash$
// Licence:     wxWindows licence
////////////////////////////////////////////////////////////////////////////////

#include "core/gui/palette.h"
#include "core/object/tree.h"

#include "interfaces/iobject.h"

#include <wx/notebook.h>

PaletteHandler::PaletteHandler( wxNotebook *owner )
{
    m_palette = owner;
    m_palette->SetImageList( new wxImageList( 16, 16 ) );
}

void PaletteHandler::OnToolClicked( wxCommandEvent &event )
{
    wxToolGroup *tg = wxDynamicCast( event.GetEventObject(), wxToolGroup );

    if ( tg )
    {
        wxWindowID toolId = event.GetId();
        wxString   clsNme = tg->GetToolShortHelp( toolId );
        int        selPge = m_palette->GetSelection();
        wxString   pgeNme = m_palette->GetPageText( selPge );
        ObjectTree::Get()->CreateObject( clsNme, pgeNme );
    }
}

wxToolGroup *PaletteHandler::AddGroup( const wxString &label,
                                       const wxBitmap &bitmap )
{
    wxToolGroup *tg = new wxToolGroup( m_palette, wxID_ANY );

    m_palette->GetImageList()->Add( bitmap );

    m_toolGroups.push_back( tg );
    m_palette->AddPage( tg, label, false, m_palette->GetPageCount() );

    return tg;
}

void PaletteHandler::OnPluginLoaded( Plugin *plugin )
{
    if ( plugin && plugin->GetCategory() == "controls" )
    {
        wxToolGroup *tg = AddGroup( plugin->GetLabel(), plugin->GetBitmap() );

        wxVector< PluginItem * > items = plugin->GetItems();

        for( wxVector< PluginItem * >::iterator it = items.begin();
            it != items.end(); ++it )
        {
            PluginItem *item = (*it);
            wxString    name = item->GetName();

            if ( name == "<separator>" )
            {
                tg->AddSeparator();
            }
            else
            {
                tg->AddTool( wxID_ANY, name, item->GetBitmap(), name );
            }
        }

        tg->Realize();
    }

//  event.Skip();
}
