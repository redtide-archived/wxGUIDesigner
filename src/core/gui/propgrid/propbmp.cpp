/////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propgrid/propbmp.cpp
// Purpose:     wxBitmapProperty and related support classes implementation
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012-05-08
// RCS-ID:      $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "core/gui/propgrid/propbmp.h"
#include "core/gui/propgrid/artdialog.h"

#include <wx/propgrid/propgrid.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>
#include <wx/log.h>
// ----------------------------------------------------------------------------
// wxPGBitmapDialogAdapter
// ----------------------------------------------------------------------------

bool wxPGBitmapDialogAdapter::DoShowDialog( wxPropertyGrid* propGrid,
                                            wxPGProperty*   property )
{
    wxBitmapProperty *bmpProp = wxDynamicCast( property, wxBitmapProperty );

    if ( bmpProp )
    {
        int index = bmpProp->GetIndex();

        switch( index )
        {
            case wxPG_BMP_SRC_ART:
            {
                ArtProviderDialog dlg( propGrid->GetPanel() );

                if ( dlg.ShowModal() == wxID_OK )
                {
                    wxString artId  = dlg.GetArtId();
                    wxString client = dlg.GetArtClient();
                    wxBitmap bmp    = wxBitmap
                    (
                        wxArtProvider::GetBitmap( artId, client, wxDefaultSize )
                    );

                    bmpProp->m_imgThumb = wxImage( bmp.ConvertToImage() );

                    wxArrayString params;
                    params.Add( artId );
                    params.Add( client );

                    SetValue( WXVARIANT(params) );
                    return true;
                }
                return false;
            }
            case wxPG_BMP_SRC_FILE:
            {
                wxString wildCard = _("All files");
                wildCard.append(" (*.*)|*.*");

                wxList& handlers = wxImage::GetHandlers();
                wxList::iterator node;

                for ( node = handlers.begin(); node != handlers.end(); ++node )
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

                if ( indFilter >= 0 )
                    dlg.SetFilterIndex( indFilter );

                if ( dlg.ShowModal() == wxID_OK )
                {
                    wxString    filePath  = dlg.GetPath();
                    wxFileName  fileName  = filePath;

                    bmpProp->ms_indFilter = dlg.GetFilterIndex();
                    bmpProp->ms_lastDir   = dlg.GetDirectory();

                    if ( fileName.FileExists() )
                    {
                        wxBitmap bmp =
                        wxBitmap( fileName.GetFullPath(), wxBITMAP_TYPE_ANY );

                        if ( bmp.IsOk() )
                        {
                            bmpProp->m_imgThumb = wxImage( bmp.ConvertToImage() );

                            wxArrayString params;
                            params.Add( filePath );

                            SetValue( WXVARIANT(params) );
                            return true;
                        }
                    }
                }
                return false;
            }
        }
    }
    return false;
}
// ----------------------------------------------------------------------------
// wxBitmapProperty
// ----------------------------------------------------------------------------

static const wxChar* const gs_bp_style_labels[] = {
    _("Default"),
    _("wxArtProvider"),
    _("File"),
    (const wxChar*) NULL
};

static const long gs_bp_style_values[] = {
    wxPG_BMP_SRC_NONE,
    wxPG_BMP_SRC_ART,
    wxPG_BMP_SRC_FILE
};

WX_PG_IMPLEMENT_PROPERTY_CLASS( wxBitmapProperty, wxEnumProperty, wxArrayString,
                                const wxArrayString&, ChoiceAndButton )

wxString wxBitmapProperty::ms_lastDir   = wxEmptyString;
int      wxBitmapProperty::ms_indFilter = 0;

void wxBitmapProperty::Init( int source, const wxArrayString& params )
{
    wxArrayString newParams;
    int           bmpType = wxPG_BMP_SRC_NONE;
    size_t        count   = params.GetCount();

    if( count )
    {
        switch( source )
        {
            case wxPG_BMP_SRC_ART:
            {
                wxString artId  = params.Item(0);
                wxString client = wxART_OTHER;

                if( count > 1 )
                    client = params.Item(1);

                wxBitmap bmp = wxArtProvider::GetBitmap( artId, client );

                if( bmp.IsOk() )
                {
                    //wxLogDebug("Loading stock_id:%s", artId );
                    newParams.Add( artId );

                    if( count > 1 )
                    {
                        //wxLogDebug("Loading stock_client:%s", client );
                        newParams.Add( client );
                    }

                    bmpType = source;
                    m_imgThumb = wxImage( bmp.ConvertToImage() );
                }
                else
                {
                    //wxLogDebug("Invalid art:%s %s", artId, client);
                }

                break;
            }
            case wxPG_BMP_SRC_FILE:
            {
                wxFileName fn = params.Item( 0 );

                if( fn.Exists() )
                {
                    wxString path = fn.GetFullPath();
                    wxBitmap bmp  = wxBitmap( path, wxBITMAP_TYPE_ANY );

                    if( bmp.IsOk() )
                    {
                        newParams.Add( path );
                        bmpType    = source;
                        m_imgThumb = wxImage( bmp.ConvertToImage() );
                    }
                }

                break;
            }
            default:
            {
                //wxLogDebug("Invalid index:%i", source);
            }
        }
    }
    else
    {
        //wxLogDebug("Nothing to load");
    }

    wxEnumProperty::SetIndex( bmpType );

    m_value = WXVARIANT( newParams );
}

wxBitmapProperty::wxBitmapProperty( int                  source,
                                    const wxArrayString& params,
                                    const wxString&      label,
                                    const wxString&      name )
:
wxEnumProperty( label, name, gs_bp_style_labels, gs_bp_style_values ),
m_bmpThumb( wxNullBitmap ),
m_imgThumb( wxNullImage )
{
    Init( source, params );
}

wxBitmapProperty::~wxBitmapProperty()
{
}

bool wxBitmapProperty::IntToValue( wxVariant& variant,
                                     int intVal,
                                     int argFlags ) const
{
    if( variant.GetType() == "arrstring" )
    {
        wxVariant v = WXVARIANT( intVal );
        return wxEnumProperty::IntToValue( v, intVal, argFlags );
    }

    return wxEnumProperty::IntToValue( variant, intVal, argFlags );
}

wxString wxBitmapProperty::ValueToString( wxVariant& value, int argFlags ) const
{
    //wxLogDebug( "ValueToString variant=%s", value.GetType() ); TODO
    return m_choices.GetLabel( GetIndex() );
}

bool wxBitmapProperty::StringToValue( wxVariant& variant, const wxString& text,
                                      int argFlags ) const
{
    //wxLogDebug( "StringToValue variant=%s text=%s", variant.GetType(), text ); TODO
    return false;
}

void wxBitmapProperty::OnSetValue()
{
    // Avoid to set the long value in wxEnumProperty::OnSetValue()
}

wxPGEditorDialogAdapter *wxBitmapProperty::GetEditorDialog() const
{
    return new wxPGBitmapDialogAdapter();
}
// ----------------------------------------------------------------------------
// Thumbnail drawing
// ----------------------------------------------------------------------------

wxSize wxBitmapProperty::OnMeasureImage( int ) const
{
    return wxPG_DEFAULT_IMAGE_SIZE;
}

void wxBitmapProperty::OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& )
{
    if ( !m_bmpThumb.IsNull() || m_imgThumb.IsOk() )
    {
        if ( m_bmpThumb.IsNull() )
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
