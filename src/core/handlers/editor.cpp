///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/editor.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "handlers/editor.h"
#include "manager.h"

EditorHandler::EditorHandler( wxNotebook *owner )
{
    m_editor = owner;
}

void EditorHandler::OnPluginLoaded( PluginEvent &event )
{
    Plugin  *plugin     = wxDynamicCast( event.GetEventObject(), Plugin );
    wxString category   = plugin->GetCategory();
    wxString plugDir    = wxGD_PLUGIN_DIR + category + wxFILE_SEP_PATH;

    if ( plugin && category == "codegens" )
    {
        wxImageList *imgLst  = m_editor->GetImageList();
        wxImageList *itmLst  = new wxImageList( 16, 16 );
        wxImage     img      = plugin->GetBitmap().ConvertToImage().Scale( 16, 16 );
        int         imgIdx   = imgLst->Add( img );
        wxString    plugName = plugin->GetName();

        wxVector< PluginItem * > items = plugin->GetItems();

        wxString xrcFileName = plugDir + plugName + wxFILE_SEP_PATH + plugName + ".xml";

        if ( !wxXmlResource::Get()->Load(xrcFileName) ) return;

        if ( items.size() )
        {
            wxNotebook *nb = new wxNotebook( m_editor, wxID_ANY );

            nb->SetImageList( itmLst );

            m_editor->AddPage( nb, plugin->GetLabel(), false, imgIdx );

            for( wxVector< PluginItem * >::iterator it = items.begin();
                it != items.end(); ++it )
            {
                PluginItem  *item    = (*it);
                wxString    itemName = item->GetName();
                wxObject    *obj     =
                    wxXmlResource::Get()->LoadObject( nb, itemName, "wxStyledTextCtrl" );

                wxStyledTextCtrl *edit = wxDynamicCast( obj, wxStyledTextCtrl );

                if ( edit )
                {
                    nb->AddPage
                        (
                            edit, itemName, false, itmLst->Add( item->GetBitmap() )
                        );
                }
            }
        }
        else
        {
            wxObject *obj = wxXmlResource::Get()->LoadObject( m_editor, plugName,
                                                             "wxStyledTextCtrl");
            wxStyledTextCtrl *edit = wxDynamicCast( obj, wxStyledTextCtrl );

            if ( edit )
            {
                m_editor->AddPage( edit, plugin->GetLabel(), false, imgIdx );
            }
        }
    }

    event.Skip();
}
