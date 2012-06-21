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
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/palette.h"

wxGDToolPalette::wxGDToolPalette( wxGDHandler *handler, wxWindow* parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler( handler )
{
    AssignImageList( new wxImageList( 16,16 ) );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDToolPalette::OnToolClicked, this );
}

wxGDToolPalette::~wxGDToolPalette()
{
}

void wxGDToolPalette::OnToolClicked( wxCommandEvent &event )
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
        m_handler->CreateObject( clsNme, GetId() ); // TODO: Assign winid
    }
}

wxAuiToolBar *wxGDToolPalette::AddGroup( const wxString &label,
                                        const wxBitmap &bitmap )
{
    wxAuiToolBar *group = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxAUI_TB_OVERFLOW );

    group->SetArtProvider( new wxGDAUIToolBarArt() );

    GetImageList()->Add( bitmap );

    AddPage( group, label, false, GetPageCount() );

    return group;
}
