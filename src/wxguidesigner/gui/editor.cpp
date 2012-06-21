///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/editor.cpp
// Purpose:     wxGDEditorBook: Visual and code editor wxNotebook impl.
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/30
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/fs_mem.h>
#include <wx/imaglist.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/stc/stc.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/events.h"
#include "wxguidesigner/utils.h"
#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/gui/utils/draw.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/designer.h"
#include "wxguidesigner/gui/glossybutton.h"
#include "wxguidesigner/gui/editor.h"
//=============================================================================
// wxGDEditorBook
//=============================================================================
wxGDEditorBook::wxGDEditorBook( wxGDHandler *handler, wxWindow *parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler( handler )
{
//=============================================================================
// wxGDEditor
//=============================================================================
    SetOwnBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
//-----------------------------------------------------------------------------
// Scrolled window, the main visual editor page
//-----------------------------------------------------------------------------
    // Working area, block events propagation to our application
    m_scrolled = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxHSCROLL | wxVSCROLL |
                                                        wxWS_EX_BLOCK_EVENTS );
    m_scrolled->SetScrollRate( 5, 5 );
    m_scrolled->SetBackgroundColour( wxColour( 192, 192, 192 ) );
//-----------------------------------------------------------------------------
// 'Invisible' resize area, where to catch and handle user mouse events
//-----------------------------------------------------------------------------
    m_resizer = new wxGDResizingPanel( m_scrolled );
    m_resizer->SetBackgroundColour( wxColour( 192, 192, 192 ) );
//-----------------------------------------------------------------------------
// The main designer
//-----------------------------------------------------------------------------
    m_designer = new wxPanel( m_resizer, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, wxBORDER_NONE );
//-----------------------------------------------------------------------------
// The 'fake TopLevelWindow' titlebar
//-----------------------------------------------------------------------------
    m_title = new wxGDTitleBarPanel( m_designer );
    wxBoxSizer* titleSizer = new wxBoxSizer( wxHORIZONTAL );

    // Init the only image handler needed if not loaded already
    if(!wxImage::FindHandler( wxBITMAP_TYPE_PNG ))
        wxImage::AddHandler( new wxPNGHandler );

    // Titlebar bitmap
    wxBitmap bmp = wxXmlResource::Get()->LoadBitmap("icon_msw");
    m_titleBmp   = new wxStaticBitmap( m_title, wxID_ANY, bmp );
    titleSizer->Add( m_titleBmp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );

    // Titlebar label
    m_titleLbl = new wxStaticText( m_title, wxID_ANY, wxT("Title") );
    m_titleLbl->Wrap( -1 );

    // Titlebar appearance
    wxColour colour = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION );
    int r = colour.Red()   - 75; if( r < 0 ) r = 15;
    int g = colour.Green() - 75; if( g < 0 ) g = 15;
    int b = colour.Blue()  - 75; if( b < 0 ) b = 15;
    wxColour caption = wxColour( (unsigned char)r, (unsigned char)g, (unsigned char)b );
    // TODO: wxRendererNative for wxMAC and wxMSW

    // Titlebar label color, depending if background dark or bright
    wxColour label = wxGDDraw::IsDark( caption ) ? *wxWHITE : *wxBLACK;
    m_titleLbl->SetFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
    m_titleLbl->SetForegroundColour( label );
    titleSizer->Add( m_titleLbl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 3 );

    // Titlebar buttons
    wxBoxSizer* titleBtnSizer = new wxBoxSizer( wxHORIZONTAL );

    // Minimize button
    bmp = wxXmlResource::Get()->LoadBitmap("minimize");
    m_titleBmpMin = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMin );

    // Maximize button
    bmp = wxXmlResource::Get()->LoadBitmap("maximize");
    m_titleBmpMax = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpMax );

    // Close button
    bmp = wxArtProvider::GetBitmap( wxART_CLOSE );
    m_titleBmpClose = new wxGlossyButton( m_title, wxID_ANY, bmp, wxEmptyString,
                                        wxDefaultPosition, wxSize( 26,-1 ) );
    titleBtnSizer->Add( m_titleBmpClose );

    titleSizer->Add( titleBtnSizer, 0, wxALL, 3 );
    m_title->SetSizer( titleSizer );
    m_title->Layout();
    titleSizer->Fit( m_title );

    wxBoxSizer* designerSizer = new wxBoxSizer( wxVERTICAL );
    designerSizer->Add( m_title, 0, wxEXPAND );
//-----------------------------------------------------------------------------
// The client area, this is our 'GetClientSize()' except when has scrollbars
//-----------------------------------------------------------------------------
    m_client = new wxPanel( m_designer );

    designerSizer->Add( m_client, 1, wxEXPAND );
    m_designer->SetSizer( designerSizer );
    m_designer->Layout();
    designerSizer->Fit( m_designer );

    wxBoxSizer* reSizer = new wxBoxSizer( wxVERTICAL );
    reSizer->Add( m_designer, 1, wxBOTTOM | wxRIGHT | wxEXPAND, 3 );
    m_resizer->SetSizer( reSizer );
    m_resizer->Layout();
    reSizer->Fit( m_resizer );

    // Editor imagelist
    wxImageList *imageList = new wxImageList( 16,16 );
    AssignImageList( imageList );

    bmp = wxXmlResource::Get()->LoadBitmap("designer");
    int imgIndex = -1;
    if( bmp.IsOk() )
        imgIndex = imageList->Add( bmp );

    AddPage( m_scrolled, _("Designer"), true, imgIndex );

    m_designer->Bind( wxEVT_SIZE, &wxGDEditorBook::OnDesignerResize, this );
//=============================================================================
// wxGDCodeEditors
//=============================================================================
    LoadCodeEditorPages();

    Bind( wxGD_EVT_OBJECT_CREATED,  &wxGDEditorBook::OnObjectCreated,  this );
    Bind( wxGD_EVT_OBJECT_SELECTED, &wxGDEditorBook::OnObjectSelected, this );
}

wxGDEditorBook::~wxGDEditorBook()
{
}

void wxGDEditorBook::LoadCodeEditorPages()
{
    wxString dbPath      = GetDataBasePath() + wxFILE_SEP_PATH;
    wxString langDir     = dbPath  + "languages";
    wxString xmlLangList = langDir + ".xml";

    wxXmlDocument doc;
    if( !doc.Load( xmlLangList ) )
        return;

    if( doc.GetRoot()->GetName() != "languages" )
        return;

    wxXmlNode *langNode = doc.GetRoot()->GetChildren();
    while( langNode )
    {
        wxString name  = langNode->GetName();
        wxString label = langNode->GetAttribute("label");
        if( label.empty() )
            label = name.Capitalize();

        // e.g.: db/languages/cpp/cpp.xrc
        wxString xrcFilePath = langDir + wxFILE_SEP_PATH + name +
                                wxFILE_SEP_PATH + name + ".xrc";

        if( !wxXmlResource::Get()->Load( xrcFilePath ) )
            continue;

        wxArrayString pageNames;
        wxXmlNode *itemNode = langNode->GetChildren();
        while( itemNode && (itemNode->GetName() == "item") )
        {
            pageNames.Add( itemNode->GetNodeContent() );
            itemNode = itemNode->GetNext();
        }

        // e.g. db/languages/cpp/icons/cpp.png
        wxString iconPath = langDir + wxFILE_SEP_PATH + name + wxFILE_SEP_PATH +
                            "icons" + wxFILE_SEP_PATH + name + ".png";
        int imageIndex = -1;
        wxBitmap bmp   = wxBitmap( iconPath, wxBITMAP_TYPE_PNG );
        if( bmp.IsOk() )
            imageIndex = GetImageList()->Add( bmp );

        size_t count = pageNames.GetCount();
        if( count )
        {
            wxNotebook  *nb     = new wxNotebook( this, wxID_ANY );
            wxImageList *itmLst = new wxImageList( 16,16 );

            nb->AssignImageList( itmLst );
            AddPage( nb, label, false, imageIndex );
            imageIndex = -1;

            for(  size_t i = 0; i < count; i++ )
            {
                // e.g. db/languages/cpp/icons/h.png
                label    = pageNames.Item(i);
                iconPath = langDir + wxFILE_SEP_PATH + name + wxFILE_SEP_PATH +
                            "icons" + wxFILE_SEP_PATH + label + ".png";
                bmp      = wxBitmap( iconPath, wxBITMAP_TYPE_PNG );

                if( bmp.IsOk() )
                    imageIndex = itmLst->Add( bmp );

                wxObject *obj =
                wxXmlResource::Get()->LoadObject( nb, label, "wxStyledTextCtrl" );
                wxStyledTextCtrl *stc = wxDynamicCast( obj, wxStyledTextCtrl );
                if( stc )
                    nb->AddPage( stc, label, false, imageIndex );
            }
        }
        else
        {
            wxObject *obj =
                wxXmlResource::Get()->LoadObject( this, name, "wxStyledTextCtrl" );

            wxStyledTextCtrl *stc = wxDynamicCast( obj, wxStyledTextCtrl );
            if( stc )
                AddPage( stc, label, false, imageIndex );
        }

        langNode = langNode->GetNext();
    }
}

void wxGDEditorBook::OnDesignerResize( wxSizeEvent &event )
{
    // Update the scrolled window virtual size to show scroll bars
    wxSize size = m_resizer->GetSize();
    m_scrolled->SetVirtualSize( size.GetX() + 15, size.GetY() + 15 );
    event.Skip();
}

void wxGDEditorBook::OnObjectCreated( wxGDObjectEvent &event )
{
    wxObject *wxobject = event.GetEventObject();
    Object   object    = event.GetObject();
//  wxString xrcText   = event.GetString();

    if( !object || !wxobject )
        return;

    wxString name      = object->GetName();
    wxString className = object->GetClassName();
/*
    // Load the xrcText in memory so access it and load the object
    wxMemoryFSHandler::AddFile("xrc.xrc", xrcText );
    wxXmlResource::Get()->Load("memory:xrc.xrc");

    if( wxXmlResource::Get()->LoadObject( wxobject, m_client, name, className ) )
    {
        wxWindow *window = wxDynamicCast( object, wxWindow );
        if( window )
        {
            wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
            sizer->Add( window, 1, wxEXPAND );
            m_client->SetSizer( sizer );
            m_resizer->SetSize( window->GetBestSize() );
        }
    }

    // Delete the temporary file
    wxMemoryFSHandler::RemoveFile("xrc.xrc");
*/
}

void wxGDEditorBook::OnObjectDeleted( wxGDObjectEvent &event )
{
    // TODO: Update the xrcText
}

void wxGDEditorBook::OnObjectSelected( wxGDObjectEvent &event )
{
    // TODO: Draw the coloured rectangle
}
