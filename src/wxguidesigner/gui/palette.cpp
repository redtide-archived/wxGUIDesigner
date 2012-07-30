////////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/palette.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/23
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
////////////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/config.h>
#include <wx/dc.h>
#include <wx/imaglist.h>
#include <wx/notebook.h>
#include <wx/renderer.h>

#include <wx/aui/auibar.h>
#include <wx/aui/framemanager.h> // wxAUI_BUTTON_STATE_*

#include "wxguidesigner/utils.h"
#include "wxguidesigner/rtti/database.h"

#include "wxguidesigner/gui/aui/tbarart.h"
#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/palette.h"

wxGD::ToolPalette::ToolPalette( Handler *handler, wxWindow* parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler( handler )
{
    wxImageList *imageCtls = m_handler->GetLargeImageList();
    wxImageList *imageList = m_handler->GetSmallImageList();

    if( !imageCtls || !imageList )
        return;

    SetImageList( imageList );
    wxString c = "controls";

    for( size_t g = 0; g < ArtProvider::GetGroupCount( c ); g++ )
    {
        int          idx = ArtProvider::GetGroupImageListIndex( c, g );
        wxString   label = ArtProvider::GetGroupLabel( c, g );
        wxString    name = ArtProvider::GetGroupName( c, g );
        wxAuiToolBar *tb = AddGroup( label, idx );

        for( size_t i = 0; i < ArtProvider::GetItemCount( c, g ); i++ )
        {
            wxString item = ArtProvider::GetItemLabel( c, g, i );

            if( item == "-" )
            {
                tb->AddSeparator();
            }
            else
            {
                idx = ArtProvider::GetItemImageListIndex( c, g, i );
                wxBitmap bmp = imageCtls->GetBitmap(idx);
                tb->AddTool( wxID_ANY, item, bmp, item );
            }
        }

        tb->Realize();
    }

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &ToolPalette::OnToolClicked, this );
}

wxGD::ToolPalette::~ToolPalette()
{
}

void wxGD::ToolPalette::OnToolClicked( wxCommandEvent &event )
{
    wxAuiToolBar *tg = wxDynamicCast( event.GetEventObject(), wxAuiToolBar );

    if( tg )
    {
        wxWindowID toolId = event.GetId();
        if( !toolId )
            return; // overflow tool item

        wxString   clsNme = tg->GetToolShortHelp( toolId );
        int        selPge = GetSelection();
        wxString   pgeNme = GetPageText( selPge );

        m_handler->CreateObject( clsNme, GetId() );
    }
}

wxAuiToolBar *wxGD::ToolPalette::AddGroup( const wxString &label, int imageIndex )
{
    wxAuiToolBar *group = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxAUI_TB_OVERFLOW );

    group->SetArtProvider( new AUI::ToolBarArt() );

    AddPage( group, label, false, imageIndex );

    return group;
}
