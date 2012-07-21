/////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/propbmp.cpp
// Purpose:     wxGDBitmapProperty and related support classes implementation
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012-05-08
// RCS-ID:      $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
/////////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>
#include <wx/propgrid/propgrid.h>

#include <wx/log.h>

#include "wxguidesigner/gui/propgrid/propbmp.h"
#include "wxguidesigner/gui/propgrid/artdialog.h"
//=============================================================================
// wxPGBitmapDialogAdapter
//=============================================================================
bool wxPGBitmapDialogAdapter::DoShowDialog( wxPropertyGrid* propGrid,
                                            wxPGProperty*   property )
{
    wxGDBitmapProperty *bmpProp = wxDynamicCast( property, wxGDBitmapProperty );

    if( bmpProp )
    {
        wxString      source = _("wxArtProvider");
        wxString      value  = bmpProp->GetValueAsString();
        wxArrayString params = wxStringTokenize( value, ";" );
        size_t        count  = params.GetCount();

        if( count )
            source = params.Item(0);

        if( source == _("wxArtProvider") )
        {
            BitmapRequesterDialog dlg( propGrid->GetPanel() );

            if( dlg.ShowModal() == wxID_OK )
            {
                wxString artId  = dlg.GetArtId();
                wxString client = dlg.GetArtClient();

                wxBitmap bmp    = wxBitmap
                (
                    wxArtProvider::GetBitmap( artId, client, wxDefaultSize )
                );

                bmpProp->m_imgThumb = wxImage( bmp.ConvertToImage() );

                // Display the string without the "_C suffix"
                client = client.Truncate( client.length() - 2 );
                value = _("wxArtProvider") + ";" + artId + ";" + client;

                SetValue( WXVARIANT( value ) );
                return true;
            }
        }
        else if( source == _("File") )
        {
            wxString wildCard = _("All files");
            wildCard.append(" (*.*)|*.*");

            wxList& handlers = wxImage::GetHandlers();
            wxList::iterator node;

            for( node = handlers.begin(); node != handlers.end(); ++node )
            {
                wxImageHandler *handler = (wxImageHandler*)*node;

                wxString ext_lo = handler->GetExtension();
                wxString ext_up = ext_lo.Upper();

                wildCard.append("|");
                wildCard.append( ext_up );
                wildCard.append(" ");
                wildCard.append(_("files") );
                wildCard.append(" (*.");
                wildCard.append( ext_up );
                wildCard.append(")|*.");
                wildCard.append( ext_lo );
            }

            wxFileDialog dlg( propGrid->GetPanel(), _("Open an image file"),
                              bmpProp->ms_lastDir, wxEmptyString, wildCard,
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST );

            int indFilter = bmpProp->ms_indFilter;

            if( indFilter >= 0 )
                dlg.SetFilterIndex( indFilter );

            if( dlg.ShowModal() == wxID_OK )
            {
                wxString    filePath  = dlg.GetPath();
                wxFileName  fileName  = filePath;

                bmpProp->ms_indFilter = dlg.GetFilterIndex();
                bmpProp->ms_lastDir   = dlg.GetDirectory();

                if( fileName.FileExists() )
                {
                    wxBitmap bmp =
                    wxBitmap( fileName.GetFullPath(), wxBITMAP_TYPE_ANY );

                    if( bmp.IsOk() )
                    {
                        bmpProp->m_imgThumb = wxImage( bmp.ConvertToImage() );

                        value = _("File") + ";" + filePath;

                        SetValue( WXVARIANT( value ) );
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
//=============================================================================
// wxGDBitmapProperty
//=============================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGDBitmapProperty, wxPGProperty, wxString,
                                const wxString &, TextCtrlAndButton )

wxString wxGDBitmapProperty::ms_lastDir   = wxEmptyString;
int      wxGDBitmapProperty::ms_indFilter = 0;

void wxGDBitmapProperty::Init( const wxString &value )
{
    wxString      newValue = wxEmptyString;
    wxArrayString params   = wxStringTokenize( value, ";" );
    size_t        count    = params.GetCount();

    if( count > 1 )
    {
        wxString source = params.Item(0);

        if( source == _("wxArtProvider") )
        {
            wxString artId  = params.Item(1);
            wxString client = wxART_OTHER;

            if( count > 2 )
                client = params.Item(2) + "_C";

            wxBitmap bmp = wxArtProvider::GetBitmap( artId, client );

            if( bmp.IsOk() )
            {
                newValue   = value;
                m_imgThumb = wxImage( bmp.ConvertToImage() );
            }
            else
            {
                wxLogDebug("Invalid art:%s %s", artId, client);
            }
        }
        else if( source == _("File") )
        {
            wxFileName fileName = params.Item(1);

            if( fileName.FileExists() )
            {
                wxString path = fileName.GetFullPath();
                wxBitmap bmp  = wxBitmap( path, wxBITMAP_TYPE_ANY );

                if( bmp.IsOk() )
                {
                    newValue    = value;
                    m_imgThumb  = wxImage( bmp.ConvertToImage() );
                }
            }
        }
    }
    else
    {
        wxLogDebug("Nothing to load");
    }

    m_value = WXVARIANT( newValue );
}

wxGDBitmapProperty::wxGDBitmapProperty( const wxString &label,
                                        const wxString &name,
                                        const wxString &value )
:
wxPGProperty( label, name ),
m_bmpThumb( wxNullBitmap ),
m_imgThumb( wxNullImage )
{
    Init( value );
}

wxGDBitmapProperty::~wxGDBitmapProperty()
{
}

wxString wxGDBitmapProperty::ValueToString( wxVariant &value, int ) const
{
    if ( value.GetType() == wxPG_VARIANT_TYPE_STRING )
        return value.GetString();

    return wxEmptyString;
}
/*
bool wxGDBitmapProperty::StringToValue( wxVariant &, const wxString &, int ) const
{
    return false;
}

void wxGDBitmapProperty::OnSetValue()
{
    // Avoid to set the long value in wxEnumProperty::OnSetValue()
}
*/
wxPGEditorDialogAdapter *wxGDBitmapProperty::GetEditorDialog() const
{
    return new wxPGBitmapDialogAdapter();
}
//=============================================================================
// Thumbnail drawing
//=============================================================================
wxSize wxGDBitmapProperty::OnMeasureImage( int ) const
{
    return wxPG_DEFAULT_IMAGE_SIZE;
}

void wxGDBitmapProperty::OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& )
{
    if( !m_bmpThumb.IsNull() || m_imgThumb.IsOk() )
    {
        if( m_bmpThumb.IsNull() )
        {
            m_imgThumb.Rescale( rect.width, rect.height );
            m_bmpThumb = wxBitmap( m_imgThumb );
        }

        dc.DrawBitmap( m_bmpThumb, rect.x, rect.y, false );
    }
    else
    {
        // No source - just draw a white box
        dc.SetBrush( *wxWHITE_BRUSH );
        dc.DrawRectangle ( rect );
    }
}
