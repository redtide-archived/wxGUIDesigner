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
        int index = bmpProp->GetIndex();

        switch( index )
        {
            case wxPG_BMP_SRC_ART:
            {
                ArtProviderDialog dlg( propGrid->GetPanel() );

                if( dlg.ShowModal() == wxID_OK )
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
//=============================================================================
// wxGDBitmapProperty
//=============================================================================
static const wxChar* const gs_bp_style_labels[] = {
    _("None"),
    _("wxArtProvider"),
    _("File"),
    (const wxChar*) NULL
};

static const long gs_bp_style_values[] = {
    wxPG_BMP_SRC_NONE,
    wxPG_BMP_SRC_ART,
    wxPG_BMP_SRC_FILE
};

WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGDBitmapProperty, wxEnumProperty, wxString,
                                const wxString&, ChoiceAndButton )

wxString wxGDBitmapProperty::ms_lastDir   = wxEmptyString;
int      wxGDBitmapProperty::ms_indFilter = 0;

void wxGDBitmapProperty::Init( int source, const wxString &value )
{
    int           type     = wxPG_BMP_SRC_NONE;
    wxString      newValue = wxEmptyString;
    wxArrayString params   = wxStringTokenize( value, ";" );
    size_t        count    = params.GetCount();

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
                    newValue = artId;

                    if( count > 1 )
                        newValue += ";" + client;

                    type        = source;
                    m_imgThumb  = wxImage( bmp.ConvertToImage() );
                }
                else
                {
                    wxLogDebug("Invalid art:%s %s", artId, client);
                }

                break;
            }
            case wxPG_BMP_SRC_FILE:
            {
                wxFileName fileName = params.Item( 0 );

                if( fileName.FileExists() )
                {
                    wxString path = fileName.GetFullPath();
                    wxBitmap bmp  = wxBitmap( path, wxBITMAP_TYPE_ANY );

                    if( bmp.IsOk() )
                    {
                        newValue    = path;
                        type        = source;
                        m_imgThumb  = wxImage( bmp.ConvertToImage() );
                    }
                }

                break;
            }
            default:
            {
                wxLogDebug("Invalid index:%i", source);
            }
        }
    }
    else
    {
        wxLogDebug("Nothing to load");
    }

    SetIndex( type );
    m_value = WXVARIANT( newValue );
}

wxGDBitmapProperty::wxGDBitmapProperty( int             source,
                                    const wxString &value,
                                    const wxString &label,
                                    const wxString &name )
:
wxEnumProperty( label, name, gs_bp_style_labels, gs_bp_style_values ),
m_bmpThumb( wxNullBitmap ),
m_imgThumb( wxNullImage )
{
    Init( source, value );
}

wxGDBitmapProperty::~wxGDBitmapProperty()
{
}

bool wxGDBitmapProperty::IntToValue( wxVariant& variant, int index, int flags ) const
{
    if( variant.GetType() == "string" )
    {
        variant = WXVARIANT( index );
        return wxEnumProperty::IntToValue( variant, index, flags );
    }

    return wxEnumProperty::IntToValue( variant, index, flags );
}

wxString wxGDBitmapProperty::ValueToString( wxVariant& value, int flags ) const
{
    size_t index = GetIndex();
    size_t count = m_choices.GetCount();
    if( index < count )
        return m_choices.GetLabel( index );

    return wxEmptyString;
}

bool wxGDBitmapProperty::StringToValue( wxVariant &, const wxString &, int ) const
{
    return false;
}

void wxGDBitmapProperty::OnSetValue()
{
    // Avoid to set the long value in wxEnumProperty::OnSetValue()
}

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
