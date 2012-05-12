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

#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/tokenzr.h>

// -----------------------------------------------------------------------
// wxBitmapPropertyValue
// -----------------------------------------------------------------------

// Implement dynamic class for type value.
IMPLEMENT_DYNAMIC_CLASS( wxBitmapPropertyValue, wxObject )

bool operator == ( const wxBitmapPropertyValue& a, const wxBitmapPropertyValue& b )
{
    return ( ( a.m_source == b.m_source ) &&
             ( a.m_bitmap.IsSameAs( b.m_bitmap ) ) &&
             ( a.m_vals   == b.m_vals) );
}
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
            case wxPG_BMP_SRC_NONE:
            {
                wxBitmapPropertyValue val = wxBitmapPropertyValue();

                wxVariant var;
                var << val;
                SetValue( var );
                return false;
            }
            case wxPG_BMP_SRC_ART:
            {
                ArtProviderDialog dlg( propGrid->GetPanel() );

                if ( dlg.ShowModal() == wxID_OK )
                {
                    wxArrayString arr;
                    arr.Add( dlg.GetArtId() );
                    arr.Add( dlg.GetArtClient() );

                    bmpProp->DoSetThumbNail( dlg.GetArtId(),
                                             dlg.GetArtClient() );
                    wxBitmapPropertyValue val
                    (
                        wxPG_BMP_SRC_ART,
                        wxArtProvider::GetBitmap( dlg.GetArtId(),
                                                  dlg.GetArtClient(),
                                                  wxDefaultSize ),
                        arr
                    );

                    wxVariant var;
                    var << val;
                    SetValue( var );
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
                    bmpProp->ms_indFilter = dlg.GetFilterIndex();
                    bmpProp->ms_lastDir   = dlg.GetDirectory();
                    bmpProp->DoSetThumbNail( dlg.GetPath() );

                    wxArrayString arr;
                    arr.Add( dlg.GetPath() );

                    wxBitmapPropertyValue val
                    (
                        wxPG_BMP_SRC_FILE,
                        wxBitmap( dlg.GetPath() ),
                        arr
                    );

                    wxVariant var;
                    var << val;
                    SetValue( var );
                    return true;
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

IMPLEMENT_VARIANT_OBJECT_SHALLOWCMP( wxBitmapPropertyValue )

WX_PG_IMPLEMENT_PROPERTY_CLASS( wxBitmapProperty, wxEnumProperty,
                                wxBitmapPropertyValue,
                                const wxBitmapPropertyValue&,
                                ChoiceAndButton )

wxString wxBitmapProperty::ms_lastDir   = wxEmptyString;
int      wxBitmapProperty::ms_indFilter = 0;

void wxBitmapProperty::Init( int source,
                            const wxBitmap& bitmap,
                            const wxArrayString& values )
{
    wxBitmapPropertyValue cpv;

    if ( bitmap.IsOk() )
    {
        cpv.Init( source, bitmap, values );

        switch( source )
        {
            case wxPG_BMP_SRC_ART:
            {
                DoSetThumbNail( values.Item( 0 ), values.Item( 1 ) );
                break;
            }
            case wxPG_BMP_SRC_FILE:
            {
                DoSetThumbNail( values.Item( 0 ) );
                break;
            }
        }
    }
    else
        cpv.Init( wxPG_BMP_SRC_NONE, wxNullBitmap, wxArrayString() );

//  m_flags |= wxPG_PROP_STATIC_CHOICES;

    wxEnumProperty::SetIndex( source );

    m_value << cpv;
}

wxBitmapProperty::wxBitmapProperty( const wxString& label,
                                    const wxString& name,
                                    const wxBitmapPropertyValue& value )
:
wxEnumProperty( label, name, gs_bp_style_labels, gs_bp_style_values ),
m_bmpThumb( wxNullBitmap ),
m_imgThumb( wxNullImage )
{
    if ( &value )
        Init( value.m_source, value.m_bitmap, value.m_vals );
    else
        Init( wxPG_BMP_SRC_NONE, wxNullBitmap, wxArrayString() );
}

wxBitmapProperty::~wxBitmapProperty()
{
}

bool wxBitmapProperty::IntToValue( wxVariant& variant,
                                     int intVal,
                                     int argFlags ) const
{
    if( variant.GetType() == "wxBitmapPropertyValue" )
    {
        wxVariant v = WXVARIANT( intVal );
        return wxEnumProperty::IntToValue( v, intVal, argFlags );
    }

    return wxEnumProperty::IntToValue( variant, intVal, argFlags );
}

wxString wxBitmapProperty::ValueToString( wxVariant& WXUNUSED( value ),
                                            int argFlags ) const
{
    switch( GetIndex() )
    {
        case wxPG_BMP_SRC_ART:
        {
            return _("wxArtProvider");
        }
        case wxPG_BMP_SRC_FILE:
        {
            return _("File");
        }
        default:
        {
            return _("Default");
        }
    }

    return wxEmptyString;
}

bool wxBitmapProperty::StringToValue( wxVariant&, const wxString&,
                                        int argFlags ) const
{
    return false;
}

void wxBitmapProperty::OnSetValue()
{
    // Avoid to set the long value in wxEnumProperty::OnSetValue()
}

wxBitmapPropertyValue wxBitmapProperty::GetVal( const wxVariant* pVariant ) const
{
    if( !pVariant )
        pVariant = &m_value;

    if( pVariant->IsNull() )
        return wxBitmapPropertyValue();

    if( pVariant->GetType() == wxS("wxBitmapPropertyValue") )
    {
        wxBitmapPropertyValue v;
        v << *pVariant;
        return v;
    }

    return wxBitmapPropertyValue();
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

void wxBitmapProperty::DoSetThumbNail( const wxString& path )
{
    wxFileName filename = path;

    // Create the image thumbnail
    if ( filename.FileExists() )
    {
        m_imgThumb = wxImage( filename.GetFullPath() );
    }
}

void wxBitmapProperty::DoSetThumbNail( const wxArtID& id,
                                       const wxArtClient& client )
{
    wxBitmap bmp = wxArtProvider::GetBitmap( id, client, wxDefaultSize );
    m_imgThumb   = wxImage( bmp.ConvertToImage() );
}
