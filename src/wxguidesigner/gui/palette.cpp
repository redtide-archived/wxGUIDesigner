////////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/palette.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/23
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
////////////////////////////////////////////////////////////////////////////////
#include <wx/notebook.h>

#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/palette.h"

wxGDToolPalette::wxGDToolPalette( wxGDHandler *handler, wxWindow* parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler( handler )
{
    SetImageList( new wxImageList(16,16) );

    Bind( wxEVT_COMMAND_TOOL_CLICKED, &wxGDToolPalette::OnToolClicked, this );
}

wxGDToolPalette::~wxGDToolPalette()
{
}

void wxGDToolPalette::OnToolClicked( wxCommandEvent &event )
{
    wxToolGroup *tg = wxDynamicCast( event.GetEventObject(), wxToolGroup );
    if( tg )
    {
        wxWindowID toolId = event.GetId();
        wxString   clsNme = tg->GetToolShortHelp( toolId );
        int        selPge = GetSelection();
        wxString   pgeNme = GetPageText( selPge );
        m_handler->CreateObject( clsNme, GetId() ); // TODO: Assign winid
    }
}

wxToolGroup *wxGDToolPalette::AddGroup( const wxString &label,
                                        const wxBitmap &bitmap )
{
    wxToolGroup *tg = new wxToolGroup( this, wxID_ANY );

    GetImageList()->Add( bitmap );

    m_toolGroups.push_back( tg );
    AddPage( tg, label, false, GetPageCount() );

    return tg;
}
