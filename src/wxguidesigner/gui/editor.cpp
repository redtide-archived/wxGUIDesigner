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
#include <wx/sstream.h>
#include <wx/stc/stc.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/events.h"
#include "wxguidesigner/utils.h"

#include "wxguidesigner/defs.h"
#include "wxguidesigner/interfaces/iobject.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/tree.h"

#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/debugwindow.h"
#include "wxguidesigner/gui/designer.h"
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

    wxBoxSizer* reSizer = new wxBoxSizer( wxVERTICAL );
    m_resizer->SetSizer( reSizer );
    m_resizer->Layout();
    reSizer->Fit( m_resizer );
/*-----------------------------------------------------------------------------
// The client area, this is our 'GetClientSize()' except when has scrollbars
//-----------------------------------------------------------------------------
    m_client = new wxPanel( m_designer );

    designerSizer->Add( m_client, 1, wxEXPAND );
    m_designer->SetSizer( designerSizer );
    m_designer->Layout();
    designerSizer->Fit( m_designer );
*/
    // Editor imagelist
    wxImageList *imageList = wxGDArtProvider::SmallImageList;
    if( !imageList )
        return;

    SetImageList( imageList );

    int imgIndex = wxGDArtProvider::GetGroupImageListIndex( "controls", "toplevel" );

    AddPage( m_scrolled, _("Designer"), true, imgIndex );
//=============================================================================
// wxGDCodeEditors
//=============================================================================
    LoadCodeEditorPages();

    Bind( wxGD_EVT_OBJECT_CREATED,   &wxGDEditorBook::OnObjectCreated,   this );
    Bind( wxGD_EVT_OBJECT_SELECTED,  &wxGDEditorBook::OnObjectSelected,  this );
    Bind( wxGD_EVT_PROPERTY_CHANGED, &wxGDEditorBook::OnPropertyChanged, this );
}

wxGDEditorBook::~wxGDEditorBook()
{
    m_objects.clear();
}

void wxGDEditorBook::LoadCodeEditorPages()
{
    wxString c = "languages";

    for( size_t g = 0; g < wxGDArtProvider::GetGroupCount( c ); g++ )
    {
        wxString name        = wxGDArtProvider::GetGroupName( c, g );
        wxString xrcFilePath = GetDataBasePath() + wxFILE_SEP_PATH + c +
                                wxFILE_SEP_PATH + name + wxFILE_SEP_PATH +
                                name + ".xrc";

        if( !wxXmlResource::Get()->Load( xrcFilePath ) )
            continue;

        int      index = wxGDArtProvider::GetGroupImageListIndex( c, g );
        wxString label = wxGDArtProvider::GetGroupLabel( c, g );
        size_t   count = wxGDArtProvider::GetItemCount( c, g );

        if( count )
        {
            wxNotebook  *nb        = new wxNotebook( this, wxID_ANY );
            wxImageList *imageList = wxGDArtProvider::SmallImageList;

            if( !imageList )
                continue;

            nb->SetImageList( imageList );
            AddPage( nb, label, false, index );

            for( size_t i = 0; i < count; i++ )
            {
                index = wxGDArtProvider::GetItemImageListIndex( c, g, i );
                label = wxGDArtProvider::GetItemLabel( c, g, i );

                wxObject *obj =
                wxXmlResource::Get()->LoadObject( nb, label, "wxStyledTextCtrl" );
                wxStyledTextCtrl *stc = wxDynamicCast( obj, wxStyledTextCtrl );
                if( stc )
                    nb->AddPage( stc, label, false, index );
            }
        }
        else
        {
            wxObject *obj =
            wxXmlResource::Get()->LoadObject( this, name, "wxStyledTextCtrl" );
            wxStyledTextCtrl *stc = wxDynamicCast( obj, wxStyledTextCtrl );
            if( stc )
                AddPage( stc, label, false, index );
        }
    }
}

void wxGDEditorBook::OnDesignerResize( wxSizeEvent &event )
{
    // Update the scrolled window virtual size to show scroll bars
    wxSize size = m_resizer->GetSize();
    m_scrolled->SetVirtualSize( size.GetX() + 15, size.GetY() + 15 );
    event.Skip();
}

void wxGDEditorBook::OnObjectCreated( wxGDObjectEvent & )
{
    UpdateControls();
}

void wxGDEditorBook::OnObjectDeleted( wxGDObjectEvent & )
{
    UpdateControls();
}

void wxGDEditorBook::OnObjectSelected( wxGDObjectEvent & )
{
    UpdateControls();
    // TODO: Draw the coloured rectangle
}

void wxGDEditorBook::OnPropertyChanged( wxGDPropertyEvent & )
{
    UpdateControls();
}

void wxGDEditorBook::UpdateControls()
{
    Object object = m_handler->GetSelectedObject();
    if( !object )
        return;

    Object toplevel = m_handler->GetTopLevelObject( object );
    if( !toplevel )
        return;

    // Load the xrcText in memory so access it and load the object
    wxXmlDocument *doc = m_handler->GetXRCProject();
    wxStringOutputStream sout;
    doc->Save( sout, 4 );
    wxString xrcText = sout.GetString();

    wxMemoryFSHandler::AddFile("xrc.xrc", xrcText );
    wxXmlResource::Get()->Load("memory:xrc.xrc");

    wxStyledTextCtrl *xrcEditor = wxDynamicCast( GetPage(1), wxStyledTextCtrl );
    if( xrcEditor )
        xrcEditor->SetText( xrcText );

    wxString className = toplevel->GetClassName();
    wxString name      = toplevel->GetName();

    m_resizer->DestroyChildren();

    wxObject *wxobject = wxXmlResource::Get()->LoadObject( m_resizer, name, className );
    wxWindow *wxwindow = wxDynamicCast( wxobject, wxWindow );

    wxMemoryFSHandler::RemoveFile("xrc.xrc");

    if( !wxwindow )
        return;

    wxSizer *reSizer = m_resizer->GetSizer();
    if( reSizer )
        reSizer->Add( wxwindow, 1, wxBOTTOM | wxRIGHT | wxEXPAND, 3 );

    wxwindow->Reparent( m_resizer );
    m_resizer->SetSize( wxSize( 399,339 ) );
    m_resizer->Layout();

    wxwindow->Bind( wxEVT_SIZE, &wxGDEditorBook::OnDesignerResize, this );
}
