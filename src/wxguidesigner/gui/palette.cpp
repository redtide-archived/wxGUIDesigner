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

#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/palette.h"

//#if wxGD_AUI_TOOLBAR
    class wxGDAUIToolBarArt : public wxAuiDefaultToolBarArt
    {
    public:
        wxGDAUIToolBarArt() {}
        virtual ~wxGDAUIToolBarArt() {}

        virtual void DrawBackground( wxDC &dc, wxWindow *, const wxRect &rect )
        {
            wxColour topEnd      = wxColour( 225,225,225 );
            wxColour topStart    = *wxWHITE;
            wxColour bottomStart = wxColour( 210,210,210 );
            wxColour bottomEnd   = wxColour( 240,240,240 );

            wxRect gradientRect = rect;

            gradientRect.SetHeight( gradientRect.GetHeight() / 2 );
            dc.GradientFillLinear( gradientRect, topStart, topEnd, wxSOUTH );
            gradientRect.Offset( 0, gradientRect.GetHeight() );
            dc.GradientFillLinear( gradientRect, bottomStart, bottomEnd, wxSOUTH );
        }

        virtual void DrawOverflowButton( wxDC &dc, wxWindow *wnd,
                                         const wxRect &rect, int state )
        {
            // TODO: Looks like wxAUI has some bug about keeping the overflow
            // button pressed state, also tooltips should appears under the
            // mouse cursor/current tool item instead to be displayed above
            // the tool item where the tooltip was shown in the first time.

            wxAuiToolBar *tb = wxDynamicCast( wnd, wxAuiToolBar );

            if( tb && !tb->GetToolBarFits() )
            {
                int flags = 0;

                if( state & wxAUI_BUTTON_STATE_HOVER )
                    flags = wxCONTROL_CURRENT;

                if( state & wxAUI_BUTTON_STATE_PRESSED )
                    flags = flags | wxCONTROL_PRESSED;

                if( flags )
                    wxRendererNative::GetDefault().
                        DrawPushButton( wnd, dc, rect, flags );

                wxRendererNative::GetDefault().
                    DrawDropArrow( wnd, dc, rect, flags );
            }
        }
    };
//#endif

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
    wxToolGroup *tg = wxDynamicCast( event.GetEventObject(), wxToolGroup );

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

wxToolGroup *wxGDToolPalette::AddGroup( const wxString &label,
                                        const wxBitmap &bitmap )
{
    wxToolGroup *group =

//#if wxGD_AUI_TOOLBAR
    new wxToolGroup( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxAUI_TB_OVERFLOW );

    group->SetArtProvider( new wxGDAUIToolBarArt() );
//#else
//    new wxToolGroup( this, wxID_ANY );
//#endif

    GetImageList()->Add( bitmap );

    AddPage( group, label, false, GetPageCount() );

    return group;
}
