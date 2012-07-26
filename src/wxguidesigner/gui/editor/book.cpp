///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/editor/book.cpp
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
#include <wx/sstream.h>
#include <wx/stc/stc.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/events.h"
#include "wxguidesigner/utils.h"

#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/tree.h"

#include "wxguidesigner/gui/artprovider.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/gui/debugwindow.h"
#include "wxguidesigner/gui/editor/designer.h"
#include "wxguidesigner/gui/editor/book.h"
//=============================================================================
// wxGDEditorBook
//=============================================================================
wxGDEditorBook::wxGDEditorBook( wxGDHandler *handler, wxWindow *parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler ( handler ),
m_editor  ( NULL )
{
    SetOwnBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

    m_editor = new wxGDEditor( this );

    wxImageList *imageList = m_handler->GetSmallImageList();
    if( imageList )
        SetImageList( imageList );

    int imgIndex = wxGDArtProvider::GetGroupImageListIndex( "controls", "toplevel" );

    AddPage( m_editor, _("Designer"), true, imgIndex );
//=============================================================================
// wxGDCodeEditors
//=============================================================================
    LoadCodeEditorPages();

    Bind( wxGD_EVT_OBJECT_CREATED,   &wxGDEditorBook::OnObjectCreated,   this );
    Bind( wxGD_EVT_OBJECT_SELECTED,  &wxGDEditorBook::OnObjectSelected,  this );
    Bind( wxGD_EVT_EVENT_CHANGED,    &wxGDEditorBook::OnEventChanged,    this );
    Bind( wxGD_EVT_PROPERTY_CHANGED, &wxGDEditorBook::OnPropertyChanged, this );
}

wxGDEditorBook::~wxGDEditorBook()
{
    m_objects.clear();
}

wxGDEditor *wxGDEditorBook::GetGUIEditor() const
{
    return m_editor;
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
            wxImageList *imageList = GetImageList();

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

void wxGDEditorBook::OnObjectCreated( wxGDObjectEvent &event )
{
    Reload();
}

void wxGDEditorBook::OnObjectDeleted( wxGDObjectEvent &event )
{
    Reload();
}

void wxGDEditorBook::OnObjectSelected( wxGDObjectEvent &event )
{
    Reload();
}

void wxGDEditorBook::OnEventChanged( wxGDEvent &event )
{
    Reload();
}

void wxGDEditorBook::OnPropertyChanged( wxGDPropertyEvent &event )
{
    Reload();
}

void wxGDEditorBook::Reload()
{
    Object object = m_handler->GetSelectedObject();
    if( !object )
        return;

    Object   toplevel  = object->GetTopLevelParent();
    wxString className = toplevel->GetClassName();
    wxString name      = toplevel->GetName();

    m_handler->Serialize();
    m_editor->UpdateDesigner( className, name );
}

void wxGDEditorBook::SetupWindow( wxWindow *window )
{
    Object object = m_handler->GetSelectedObject();
    if( !window || !object )
        return;

    Property property = object->GetProperty("size");
    wxSize   size     = property->GetAsSize();

    if( size != wxDefaultSize )
        window->SetSize( size );
}
