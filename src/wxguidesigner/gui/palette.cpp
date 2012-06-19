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
#include <wx/imaglist.h>
#include <wx/notebook.h>

#include "wxguidesigner/utils.h"
#include "wxguidesigner/rtti/database.h"

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
    wxToolGroup *group = new wxToolGroup( this, wxID_ANY );

    GetImageList()->Add( bitmap );

    AddPage( group, label, false, GetPageCount() );

    return group;
}

/*
void wxGDToolPalette::LoadPalette( const wxSize &iconSize )
{
    wxString ctrlDir     = GetDataBasePath() + wxFILE_SEP_PATH + "controls";
    wxString xmlCtrlList = ctrlDir + ".xml";

    wxXmlDocument doc;
    if( !doc.Load( xmlCtrlList ) )
        return;

    if( doc.GetRoot()->GetName() != "controls" )
        return;

    int imgIndex = 1; // index 0 is 'project' icon
    wxXmlNode *groupNode = doc.GetRoot()->GetChildren();

    while( groupNode )
    {
        wxString name  = groupNode->GetName();
        wxString label = groupNode->GetAttribute("label");

        if( label.empty() )
            label = name.Capitalize();

        wxToolGroup *toolGroup = AddGroup( name, label );
        wxImageList *imageList = m_handler->GetControlsImageList();

        wxXmlNode *itemNode = groupNode->GetChildren();
        while( itemNode )
        {
            wxString itemName  = itemNode->GetNodeContent();
            bool     separator = itemNode->GetName() == "group";
            if( separator )
            {
                toolGroup->AddSeparator();
            }
            else //if( m_handler->ClassExists( itemName ) ) TODO
            {
                wxBitmap bmp = wxNullBitmap;
                if( imageList && (imgIndex < imageList->GetImageCount()) )
                    bmp = imageList->GetBitmap( imgIndex );

                if( !bmp.IsOk() )
                    bmp = wxArtProvider::GetBitmap( wxART_MISSING_IMAGE,
                                                    wxART_OTHER, iconSize );
                imgIndex++;
                toolGroup->AddTool( wxID_ANY, itemName, bmp, itemName );
            }

            itemNode = itemNode->GetNext();
        }

        toolGroup->Realize();

        groupNode = groupNode->GetNext();
    }
}

wxBitmap wxGDToolPalette::LoadBitmap( const wxString &categoryName,
                                      const wxString &ctrlName,
                                      const wxSize   &size )
{
    wxString iconPath = GetDataBasePath() + wxFILE_SEP_PATH + "controls" +
                        wxFILE_SEP_PATH + categoryName + wxFILE_SEP_PATH +
                        "icons" + wxFILE_SEP_PATH;

    wxBitmap bmp = wxArtProvider::GetBitmap( wxART_MISSING_IMAGE,
                                             wxART_OTHER, size );
    if( ctrlName.empty() )
    {
        iconPath += categoryName + ".png";
    }
    else
    {
        wxString bmpName = ctrlName.Lower();
        bmpName.Replace("wx", "");
        iconPath += bmpName + ".png";
    }

    if( !wxFileExists( iconPath ) )
        return bmp;

    wxBitmap bitmap = wxBitmap( iconPath, wxBITMAP_TYPE_PNG );
    if( bitmap.IsOk() && (bitmap.GetSize() == size) )
        return bitmap;

    return bmp;
}
*/