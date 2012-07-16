///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/props.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/utils.h"
#include "wxguidesigner/gui/propgrid/props.h"

#include <wx/settings.h>
//=============================================================================
// wxSizeProperty
//=============================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxSizeProperty, wxPGProperty,
                                wxSize, const wxSize&, TextCtrl )

wxSizeProperty::wxSizeProperty( const wxString &label,
                                const wxString &name,
                                const wxSize   &value )
:
wxPGProperty( label, name )
{
    DoSetValue( value );
    AddPrivateChild( new wxIntProperty( _("Width"),  wxPG_LABEL, value.x ) );
    AddPrivateChild( new wxIntProperty( _("Height"), wxPG_LABEL, value.y ) );
}

wxSizeProperty::~wxSizeProperty()
{
}

void wxSizeProperty::RefreshChildren()
{
    if( !GetChildCount() ) return;

    const wxSize &size = wxSizeRefFromVariant( m_value );

    Item(0)->SetValue( (long)size.x );
    Item(1)->SetValue( (long)size.y );
}

wxVariant wxSizeProperty::ChildChanged( wxVariant &thisValue,
                                        int       childIndex,
                                        wxVariant &childValue ) const
{
    wxSize &size = wxSizeRefFromVariant( thisValue );

    int val = childValue.GetLong();

    switch( childIndex )
    {
        case 0:
        {
            size.x = val;
            break;
        }
        case 1:
        {
            size.y = val;
            break;
        }
    }

    wxVariant newVariant;
    newVariant << size;
    return newVariant;
}
//=============================================================================
// wxPointProperty
//=============================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxPointProperty, wxPGProperty, wxPoint,
                                const wxPoint&, TextCtrl )

wxPointProperty::wxPointProperty  ( const wxString &label,
                                    const wxString &name,
                                    const wxPoint  &value )
:
wxPGProperty( label, name )
{
    DoSetValue( value );
    AddPrivateChild( new wxIntProperty( "X", wxPG_LABEL, value.x ) );
    AddPrivateChild( new wxIntProperty( "Y", wxPG_LABEL, value.y ) );
}

wxPointProperty::~wxPointProperty()
{
}

void wxPointProperty::RefreshChildren()
{
    if( !GetChildCount() )
        return;

    const wxPoint& point = wxPointRefFromVariant( m_value );

    Item(0)->SetValue( (long)point.x );
    Item(1)->SetValue( (long)point.y );
}

wxVariant wxPointProperty::ChildChanged(wxVariant &thisValue,
                                        int       childIndex,
                                        wxVariant &childValue) const
{
    wxPoint& point = wxPointRefFromVariant( thisValue );

    int val = childValue.GetLong();
    switch( childIndex )
    {
        case 0:
        {
            point.x = val;
            break;
        }
        case 1:
        {
            point.y = val;
            break;
        }
    }

    wxVariant newVariant;
    newVariant << point;
    return newVariant;
}
//=============================================================================
// wxGDColourProperty
//=============================================================================
static const wxChar* const gs_cp_labels[] = {
    _("Default"),
    _("AppWorkspace"),
    _("ActiveBorder"),
    _("ActiveCaption"),
    _("ButtonFace"),
    _("ButtonHighlight"),
    _("ButtonShadow"),
    _("ButtonText"),
    _("CaptionText"),
    _("ControlDark"),
    _("ControlLight"),
    _("Desktop"),
    _("GrayText"),
    _("Highlight"),
    _("HighlightText"),
    _("InactiveBorder"),
    _("InactiveCaption"),
    _("InactiveCaptionText"),
    _("Menu"),
    _("Scrollbar"),
    _("Tooltip"),
    _("TooltipText"),
    _("Window"),
    _("WindowFrame"),
    _("WindowText"),
    _("Custom"),
    (const wxChar*) NULL
};

static const long gs_cp_values[] = {
    wxPG_COLOUR_DEFAULT,
    wxSYS_COLOUR_APPWORKSPACE,
    wxSYS_COLOUR_ACTIVEBORDER,
    wxSYS_COLOUR_ACTIVECAPTION,
    wxSYS_COLOUR_BTNFACE,
    wxSYS_COLOUR_BTNHIGHLIGHT,
    wxSYS_COLOUR_BTNSHADOW,
    wxSYS_COLOUR_BTNTEXT ,
    wxSYS_COLOUR_CAPTIONTEXT,
    wxSYS_COLOUR_3DDKSHADOW,
    wxSYS_COLOUR_3DLIGHT,
    wxSYS_COLOUR_BACKGROUND,
    wxSYS_COLOUR_GRAYTEXT,
    wxSYS_COLOUR_HIGHLIGHT,
    wxSYS_COLOUR_HIGHLIGHTTEXT,
    wxSYS_COLOUR_INACTIVEBORDER,
    wxSYS_COLOUR_INACTIVECAPTION,
    wxSYS_COLOUR_INACTIVECAPTIONTEXT,
    wxSYS_COLOUR_MENU,
    wxSYS_COLOUR_SCROLLBAR,
    wxSYS_COLOUR_INFOBK,
    wxSYS_COLOUR_INFOTEXT,
    wxSYS_COLOUR_WINDOW,
    wxSYS_COLOUR_WINDOWFRAME,
    wxSYS_COLOUR_WINDOWTEXT,
    wxPG_COLOUR_CUSTOM
};

WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGDColourProperty, wxSystemColourProperty,
                                wxColourPropertyValue,
                                const wxColourPropertyValue &, Choice )

static wxPGChoices gs_wxGDColourProperty_choicesCache;

wxGDColourProperty::wxGDColourProperty( const wxString              &label,
                                        const wxString              &name,
                                        const wxColourPropertyValue &value )
:
wxSystemColourProperty( label, name, gs_cp_labels, gs_cp_values,
                        &gs_wxGDColourProperty_choicesCache, value )
{
    if( &value )
        Init( value.m_type, value.m_colour );
    else
        SetValueToUnspecified();
}

wxGDColourProperty::~wxGDColourProperty()
{
}

//=============================================================================
// wxGDFontProperty
//=============================================================================
#include <wx/fontenum.h>
#include <wx/fontdata.h>
#include <wx/fontdlg.h>
#include <wx/fontmap.h>

static const wxChar* const gs_fp_es_family_labels[] = {
    _("Default"),
    _("Decorative"),
    _("Roman"),
    _("Script"),
    _("Swiss"),
    _("Modern"),
    _("Teletype"),
    _("Unknown"),
    (const wxChar*) NULL
};

static const long gs_fp_es_family_values[] = {
    wxFONTFAMILY_DEFAULT,
    wxFONTFAMILY_DECORATIVE,
    wxFONTFAMILY_ROMAN,
    wxFONTFAMILY_SCRIPT,
    wxFONTFAMILY_SWISS,
    wxFONTFAMILY_MODERN,
    wxFONTFAMILY_TELETYPE,
    wxFONTFAMILY_UNKNOWN
};

static const wxChar* const gs_fp_es_style_labels[] = {
    _("Normal"),
    _("Slant"),
    _("Italic"),
    (const wxChar*) NULL
};

static const long gs_fp_es_style_values[] = {
    wxFONTSTYLE_NORMAL,
    wxFONTSTYLE_SLANT,
    wxFONTSTYLE_ITALIC
};

static const wxChar* const gs_fp_es_weight_labels[] = {
    _("Normal"),
    _("Light"),
    _("Bold"),
    (const wxChar*) NULL
};

static const long gs_fp_es_weight_values[] = {
    wxFONTWEIGHT_NORMAL,
    wxFONTWEIGHT_LIGHT,
    wxFONTWEIGHT_BOLD
};

static wxArrayString gs_fp_es_encodings;
static wxArrayInt gs_fp_es_encodings_vals;

WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGDFontProperty, wxPGProperty, wxFont,
                                const wxFont &, TextCtrlAndButton )

wxGDFontProperty::wxGDFontProperty( const wxString          &label,
                                    const wxString          &name,
                                    const wxFontContainer   &value )
:
wxPGProperty( label, name )
{
    SetValue( WXVARIANT( wxGDConv::FontToString( value ) ) );

    // Initialize font family choices list
    if( !wxPGGlobalVars->m_fontFamilyChoices )
    {
        wxArrayString faceNames = wxFontEnumerator::GetFacenames();

        faceNames.Sort();
        faceNames.Insert( wxEmptyString, 0 );

        wxPGGlobalVars->m_fontFamilyChoices = new wxPGChoices( faceNames );
    }

    AddPrivateChild( new wxIntProperty( _("Point Size"), "size",
                                        value.GetPointSize() ) );

    AddPrivateChild   ( new wxEnumProperty( _("Family"), "family",
                                            gs_fp_es_family_labels,
                                            gs_fp_es_family_values,
                                            value.GetFamily()) );

    AddPrivateChild   ( new wxEnumProperty( _("Style"), "style",
                                            gs_fp_es_style_labels,
                                            gs_fp_es_style_values,
                                            value.GetStyle() ) );

    AddPrivateChild   ( new wxEnumProperty( _("Weight"), "weight",
                                            gs_fp_es_weight_labels,
                                            gs_fp_es_weight_values,
                                            value.GetWeight()) );

    wxPGProperty* pgChild = new wxBoolProperty( _("Underlined"), "underlined",
                                                value.GetUnderlined() );

    pgChild->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );

    AddPrivateChild( pgChild );

    wxString faceName = value.GetFaceName();

    if( !faceName.empty() &&
        wxPGGlobalVars->m_fontFamilyChoices->Index(faceName) == wxNOT_FOUND )
        wxPGGlobalVars->m_fontFamilyChoices->AddAsSorted(faceName);

    pgChild = new wxEnumProperty( _("Face Name"), "face",
                                    *wxPGGlobalVars->m_fontFamilyChoices );

    pgChild->SetValueFromString( faceName, wxPG_FULL_VALUE );

    AddPrivateChild( pgChild );

    if( gs_fp_es_encodings.empty() )
    {
        wxFontEncoding fontEncoding;
        size_t count = wxFontMapper::GetSupportedEncodingsCount();

        for( size_t i = 0; i < count; i++ )
        {
            fontEncoding = wxFontMapper::GetEncoding(i);
            if( fontEncoding == wxFONTENCODING_SYSTEM )
                continue;

            gs_fp_es_encodings.Add( wxFontMapper::GetEncodingName(fontEncoding) );
            gs_fp_es_encodings_vals.Add( fontEncoding );
        }
    }

    AddPrivateChild( new wxEnumProperty( _("Encoding"), "encoding", gs_fp_es_encodings,
                                        gs_fp_es_encodings_vals, value.GetEncoding() ) );
}

wxGDFontProperty::~wxGDFontProperty()
{
}

void wxGDFontProperty::OnSetValue()
{
}

wxString wxGDFontProperty::ValueToString( wxVariant &value, int argFlags ) const
{
    return wxPGProperty::ValueToString( value, argFlags );
}

bool wxGDFontProperty::OnEvent( wxPropertyGrid* propgrid,
                                wxWindow*       WXUNUSED(primary),
                                wxEvent         &event )
{
    if( propgrid->IsMainButtonEvent( event ) )
    {
        wxFontData      data;
        wxFontContainer font = wxGDConv::StringToFont( m_value.GetString() );

        data.SetInitialFont( font );
        data.SetColour( *wxBLACK );

        wxFontDialog dlg( propgrid, data );
        if( dlg.ShowModal() == wxID_OK )
        {
            propgrid->EditorsValueWasModified();

            wxFontContainer font    = dlg.GetFontData().GetChosenFont();
            wxVariant       variant = WXVARIANT( wxGDConv::FontToString(font) );

            SetValueInEvent( variant );
            return true;
        }
    }

    return false;
}

void wxGDFontProperty::RefreshChildren()
{
    wxFontContainer font = wxGDConv::StringToFont( m_value.GetString() );

    Item(0)->SetValue( font.GetPointSize() );
    Item(1)->SetValue( font.GetFamily() );
    Item(2)->SetValue( font.GetStyle() );
    Item(3)->SetValue( font.GetWeight() );
    Item(4)->SetValue( font.GetUnderlined() );
    Item(5)->SetValueFromString( font.GetFaceName(), wxPG_FULL_VALUE );
    Item(6)->SetValue( font.GetEncoding() );
}

wxVariant wxGDFontProperty::ChildChanged  ( wxVariant &thisValue,
                                            int       index,
                                            wxVariant &childValue ) const
{
    wxFontContainer font = wxGDConv::StringToFont( thisValue.GetString() );

    switch( index )
    {
        case 0:
        {
            font.SetPointSize( childValue.GetLong() );
            break;
        }
        case 1:
        {
            int family = childValue.GetLong();
            if( family < wxFONTFAMILY_DEFAULT || family > wxFONTFAMILY_TELETYPE )
                family = wxFONTFAMILY_DEFAULT;

            font.SetFamily( family );
            break;
        }
        case 2:
        {
            int style = childValue.GetLong();
            if( style != wxFONTSTYLE_NORMAL && style != wxFONTSTYLE_SLANT &&
                style != wxFONTSTYLE_ITALIC )
                style = wxFONTSTYLE_NORMAL;

            font.SetStyle( style );
            break;
        }
        case 3:
        {
            int weight = childValue.GetLong();
            if( weight != wxFONTWEIGHT_NORMAL && weight != wxFONTWEIGHT_LIGHT &&
                weight != wxFONTWEIGHT_BOLD )
                weight = wxFONTWEIGHT_NORMAL;

            font.SetWeight( weight );
            break;
        }
        case 4:
        {
            font.SetUnderlined( childValue.GetBool() );
            break;
        }
        case 5:
        {
            wxString faceName = wxEmptyString;
            int faceIndex = childValue.GetLong();

            if( faceIndex >= 0 )
                faceName = wxPGGlobalVars->m_fontFamilyChoices->GetLabel(faceIndex);

            font.SetFaceName( faceName );
            break;
        }
        case 6:
        {
            int encoding = childValue.GetLong();
            if( encoding < wxFONTENCODING_DEFAULT || encoding > wxFONTENCODING_MAX )
                encoding = wxFONTENCODING_DEFAULT;
            
            font.SetEncoding( encoding );
            break;
        }
    }

    thisValue = WXVARIANT( wxGDConv::FontToString(font) );
    return thisValue;
}
