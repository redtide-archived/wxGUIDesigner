///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/property/props.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include "wxguidesigner/utils.h"
#include "wxguidesigner/fontcontainer.h"
#include "wxguidesigner/gui/property/props.h"
//=============================================================================
// ColourProperty
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
    0,
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

WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGD::ColourProperty, wxSystemColourProperty,
                                wxColourPropertyValue,
                                const wxColourPropertyValue &, Choice )

static wxPGChoices gs_cp_choicesCache;

wxGD::ColourProperty::ColourProperty  ( const wxString              &label,
                                        const wxString              &name,
                                        const wxColourPropertyValue &value )
:
wxSystemColourProperty( label, name, gs_cp_labels, gs_cp_values,
                        &gs_cp_choicesCache, value )
{
    if( &value )
        Init( value.m_type, value.m_colour );
    else
        SetValueToUnspecified();
}

wxString wxGD::ColourProperty::ColourToString( const wxColour &colour,
                                            int index, int flags ) const
{
    if( index == 0 )
    {
        return wxEmptyString;
    }
    else if( index == -1 )
    {
        return colour.GetAsString();
    }

    return wxGD::Convert::SystemColourToString( gs_cp_values[index] );
}

wxGD::ColourProperty::~ColourProperty()
{
}
//=============================================================================
// FlagsProperty
//=============================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGD::FlagsProperty, wxPGProperty, long, long, TextCtrl )

void wxGD::FlagsProperty::Init()
{
    long value = m_value;

    // Generate children
    size_t prevChildCount = m_children.size();

    int oldSelection = -1;
    if( prevChildCount )
    {
        wxPropertyGridPageState* state = GetParentState();

        // State safety check (it may be NULL in immediate parent)
        wxASSERT( state );

        if( state )
        {
            wxPGProperty* selected = state->GetSelection();
            if( selected )
            {
                if( selected->GetParent() == this )
                    oldSelection = selected->GetIndexInParent();
                else if( selected == this )
                    oldSelection = -2;
            }
        }
        state->DoClearSelection();
    }

    // Delete old children
    for( size_t i = 0; i < prevChildCount; i++ )
        delete m_children[i];

    m_children.clear();

    if( m_choices.IsOk() )
    {
        for( size_t i = 0; i < GetItemCount(); i++ )
        {
            bool childValue = ( value & m_choices.GetValue(i) ) ? true : false;
            wxString label  = GetLabel(i);

            wxPGProperty* boolProp = NULL;
#if wxUSE_INTL
            if( wxPGGlobalVars->m_autoGetTranslation )
            {
                boolProp =
                new wxBoolProperty( ::wxGetTranslation(label), label, childValue );
            }
            else
#endif
            {
                boolProp = new wxBoolProperty( label, label, childValue );
            }

            boolProp->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );

            AddPrivateChild( boolProp );
        }

        m_oldChoicesData = m_choices.GetDataPtr();
    }

    m_oldValue = m_value;

    if( prevChildCount )
        SubPropsChanged( oldSelection );
}

wxGD::FlagsProperty::FlagsProperty( const wxString      &label,
                                    const wxString      &name,
                                    const wxArrayString &labels,
                                    const wxArrayInt    &values,
                                    int                 value )
:
wxPGProperty( label, name )
{
    m_oldChoicesData = NULL;

    if( &labels && labels.size() )
    {
        m_choices.Set( labels, values );

        wxASSERT( GetItemCount() );

        SetValue( (long)value );
    }
    else
    {
        m_value = wxPGGlobalVars->m_vZero;
    }
}

wxGD::FlagsProperty::~FlagsProperty()
{
}

void wxGD::FlagsProperty::OnSetValue()
{
    if( !m_choices.IsOk() || !GetItemCount() )
    {
        m_value = wxPGGlobalVars->m_vZero;
    }
    else
    {
        long val = m_value.GetLong();
        long fullFlags = 0;

        // normalize the value (i.e. remove extra flags)
        for( size_t i = 0; i < GetItemCount(); i++ )
        {
            fullFlags |= m_choices.GetValue(i);
        }

        val &= fullFlags;

        m_value = val;

        // Need to (re)init now?
        if( GetChildCount() != GetItemCount() ||
            m_choices.GetDataPtr() != m_oldChoicesData )
        {
            Init();
        }
    }

    long newFlags = m_value;
    if( newFlags != m_oldValue )
    {
        // Set child modified states
        for( size_t i = 0; i<GetItemCount(); i++ )
        {
            int flag = m_choices.GetValue(i);

            if( (newFlags & flag) != (m_oldValue & flag) )
                Item(i)->ChangeFlag( wxPG_PROP_MODIFIED, true );
        }

        m_oldValue = newFlags;
    }
}

wxString wxGD::FlagsProperty::ValueToString( wxVariant &value, int ) const
{
    wxString text;

    if( !m_choices.IsOk() )
        return text;

    long flags = value;

    for( size_t i = 0; i < GetItemCount(); i++ )
    {
        int doAdd = ( flags & m_choices.GetValue(i) );
        if( doAdd )
        {
            text += m_choices.GetLabel(i);
            text += "|";
        }
    }

    // remove last or
    size_t textLength = text.length();
    if( textLength > 1 )
        text.Truncate( textLength - 1 );

    return text;
}
// Translate string into flag tokens
bool wxGD::FlagsProperty::StringToValue( wxVariant &variant,
                                        const wxString &text, int ) const
{
    if( !m_choices.IsOk() )
        return false;

    long newFlags = 0;

    // semicolons are no longer valid delimeters
    WX_PG_TOKENIZER1_BEGIN( text, '|' )
        if( !token.empty() )
        {
            // Determine which one it is
            long bit = IdToBit( token );
            if( bit != -1 )
                newFlags |= bit; // Changed?
            else
                break;
        }
    WX_PG_TOKENIZER1_END()

    if( variant != (long)newFlags )
    {
        variant = (long)newFlags;
        return true;
    }

    return false;
}

// Converts string id to a relevant bit.
long wxGD::FlagsProperty::IdToBit( const wxString &id ) const
{
    for( size_t i = 0; i < GetItemCount(); i++ )
    {
        if( id == GetLabel(i) )
            return m_choices.GetValue(i);
    }

    return -1;
}

void wxGD::FlagsProperty::RefreshChildren()
{
    if( !m_choices.IsOk() || !GetChildCount() )
        return;

    int flags = m_value.GetLong();

    for( size_t i = 0; i < GetItemCount(); i++ )
    {
        long flag   = m_choices.GetValue(i);
        long subVal = flags & flag;

        wxPGProperty* p = Item(i);

        if( subVal != (m_oldValue & flag ) )
            p->ChangeFlag( wxPG_PROP_MODIFIED, true );

        p->SetValue( subVal ? true : false );
    }

    m_oldValue = flags;
}

wxVariant wxGD::FlagsProperty::ChildChanged ( wxVariant &thisValue, int childIndex,
                                            wxVariant &childValue ) const
{
    long oldValue = thisValue.GetLong();
    long value = childValue.GetLong();
    unsigned long vi = m_choices.GetValue(childIndex);

    if( value )
        return (long) (oldValue | vi);

    return (long) (oldValue & ~(vi));
}

bool wxGD::FlagsProperty::DoSetAttribute( const wxString &name, wxVariant &value )
{
    if( name == wxPG_BOOL_USE_CHECKBOX ||
        name == wxPG_BOOL_USE_DOUBLE_CLICK_CYCLING )
    {
        for( size_t i=0; i<GetChildCount(); i++ )
        {
            Item(i)->SetAttribute(name, value);
        }
        // Must return false so that the attribute is stored in
        // flag property's actual property storage
        return false;
    }
    return false;
}
//=============================================================================
// FontProperty - TODO: Implement 'sysfont' and 'relativesize' properties
//=============================================================================
#include <wx/fontenum.h>
#include <wx/fontdata.h>
#include <wx/fontdlg.h>
#include <wx/fontmap.h>

static const wxChar* const gs_fp_family_labels[] = {
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

static const long gs_fp_family_values[] = {
    wxFONTFAMILY_DEFAULT,
    wxFONTFAMILY_DECORATIVE,
    wxFONTFAMILY_ROMAN,
    wxFONTFAMILY_SCRIPT,
    wxFONTFAMILY_SWISS,
    wxFONTFAMILY_MODERN,
    wxFONTFAMILY_TELETYPE,
    wxFONTFAMILY_UNKNOWN
};

static const wxChar* const gs_fp_style_labels[] = {
    _("Normal"),
    _("Slant"),
    _("Italic"),
    (const wxChar*) NULL
};

static const long gs_fp_style_values[] = {
    wxFONTSTYLE_NORMAL,
    wxFONTSTYLE_SLANT,
    wxFONTSTYLE_ITALIC
};

static const wxChar* const gs_fp_weight_labels[] = {
    _("Normal"),
    _("Light"),
    _("Bold"),
    (const wxChar*) NULL
};

static const long gs_fp_weight_values[] = {
    wxFONTWEIGHT_NORMAL,
    wxFONTWEIGHT_LIGHT,
    wxFONTWEIGHT_BOLD
};

static wxArrayString    gs_fp_encodings;
static wxArrayInt       gs_fp_encodings_vals;

WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGD::FontProperty, wxPGProperty, wxFont,
                                const wxFont &, TextCtrlAndButton )

wxGD::FontProperty::FontProperty  ( const wxString          &label,
                                    const wxString          &name,
                                    const wxFontContainer   &value )
:
wxPGProperty( label, name )
{
    SetValue( WXVARIANT( wxGD::Convert::FontToString( value ) ) );

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
                                            gs_fp_family_labels,
                                            gs_fp_family_values,
                                            value.GetFamily()) );

    AddPrivateChild   ( new wxEnumProperty( _("Style"), "style",
                                            gs_fp_style_labels,
                                            gs_fp_style_values,
                                            value.GetStyle() ) );

    AddPrivateChild   ( new wxEnumProperty( _("Weight"), "weight",
                                            gs_fp_weight_labels,
                                            gs_fp_weight_values,
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

    if( gs_fp_encodings.empty() )
    {
        wxFontEncoding fontEncoding;
        size_t count = wxFontMapper::GetSupportedEncodingsCount();

        for( size_t i = 0; i < count; i++ )
        {
            fontEncoding = wxFontMapper::GetEncoding(i);
            if( fontEncoding == wxFONTENCODING_SYSTEM )
                continue;

            gs_fp_encodings.Add( wxFontMapper::GetEncodingName(fontEncoding) );
            gs_fp_encodings_vals.Add( fontEncoding );
        }
    }

    AddPrivateChild( new wxEnumProperty( _("Encoding"), "encoding", gs_fp_encodings,
                                        gs_fp_encodings_vals, value.GetEncoding() ) );
}

wxGD::FontProperty::~FontProperty()
{
}

void wxGD::FontProperty::OnSetValue()
{
}

wxString wxGD::FontProperty::ValueToString( wxVariant &value, int flags ) const
{
    if( value.GetType() == wxPG_VARIANT_TYPE_STRING )
    {
        wxString val = value.GetString();
        value = WXVARIANT( val );
        return val;
    }

    return wxPGProperty::ValueToString( value, flags );
}

bool wxGD::FontProperty::StringToValue  ( wxVariant       &variant,
                                        const wxString  &text, int flags ) const
{
    wxFontContainer font = wxGD::Convert::StringToFont( text );
    variant << font;
    return true;
}

bool wxGD::FontProperty::OnEvent( wxPropertyGrid* propgrid,
                                wxWindow*       WXUNUSED(primary),
                                wxEvent         &event )
{
    if( propgrid->IsMainButtonEvent( event ) )
    {
        wxFontData      data;
        wxFontContainer font = wxGD::Convert::StringToFont( m_value.GetString() );

        data.SetInitialFont( font );
        data.SetColour( *wxBLACK );

        wxFontDialog dlg( propgrid, data );
        if( dlg.ShowModal() == wxID_OK )
        {
            propgrid->EditorsValueWasModified();

            wxFontContainer font    = dlg.GetFontData().GetChosenFont();
            wxVariant       variant = WXVARIANT( wxGD::Convert::FontToString(font) );

            SetValueInEvent( variant );
            return true;
        }
    }

    return false;
}

void wxGD::FontProperty::RefreshChildren()
{
    wxFontContainer font = wxGD::Convert::StringToFont( m_value.GetString() );

    Item(0)->SetValue( font.GetPointSize() );
    Item(1)->SetValue( font.GetFamily() );
    Item(2)->SetValue( font.GetStyle() );
    Item(3)->SetValue( font.GetWeight() );
    Item(4)->SetValue( font.GetUnderlined() );
    Item(5)->SetValueFromString( font.GetFaceName(), wxPG_FULL_VALUE );
    Item(6)->SetValue( font.GetEncoding() );
}

wxVariant wxGD::FontProperty::ChildChanged  ( wxVariant &thisValue,
                                            int       index,
                                            wxVariant &childValue ) const
{
    wxFontContainer font = wxGD::Convert::StringToFont( thisValue.GetString() );

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

    thisValue = WXVARIANT( wxGD::Convert::FontToString(font) );
    return thisValue;
}
//=============================================================================
// PointProperty
//=============================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGD::PointProperty, wxPGProperty, wxPoint,
                                const wxPoint &, TextCtrl )

wxGD::PointProperty::PointProperty( const wxString &label,
                                    const wxString &name,
                                    const wxPoint  &value )
:
wxPGProperty( label, name )
{
    DoSetValue( value );
    AddPrivateChild( new wxIntProperty( "X", wxPG_LABEL, value.x ) );
    AddPrivateChild( new wxIntProperty( "Y", wxPG_LABEL, value.y ) );
}

wxGD::PointProperty::~PointProperty()
{
}

void wxGD::PointProperty::RefreshChildren()
{
    if( !GetChildCount() )
        return;

    const wxPoint &point = wxPointRefFromVariant( m_value );

    Item(0)->SetValue( (long)point.x );
    Item(1)->SetValue( (long)point.y );
}

wxVariant wxGD::PointProperty::ChildChanged ( wxVariant   &thisValue,
                                            int         childIndex,
                                            wxVariant   &childValue) const
{
    wxPoint &point = wxPointRefFromVariant( thisValue );

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

wxString wxGD::PointProperty::ValueToString( wxVariant &value, int flags ) const
{
    if( value.GetType() == "wxPoint" )
    {
        wxPoint &point = wxPointRefFromVariant( value );
        wxString val   = wxGD::Convert::PointToString( point );
        value = WXVARIANT( val );
        return val;
    }

    return wxPGProperty::ValueToString( value, flags );
}

bool wxGD::PointProperty::StringToValue( wxVariant       &variant,
                                         const wxString  &text, int flags ) const
{
    wxPoint point = wxGD::Convert::StringToPoint( text );
    variant << point;
    return true;
}
//=============================================================================
// SizeProperty
//=============================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGD::SizeProperty, wxPGProperty,
                                wxSize, const wxSize &, TextCtrl )

wxGD::SizeProperty::SizeProperty  ( const wxString &label,
                                    const wxString &name,
                                    const wxSize   &value )
:
wxPGProperty( label, name )
{
    DoSetValue( value );
    AddPrivateChild( new wxIntProperty( _("Width"),  wxPG_LABEL, value.x ) );
    AddPrivateChild( new wxIntProperty( _("Height"), wxPG_LABEL, value.y ) );
}

wxGD::SizeProperty::~SizeProperty()
{
}

void wxGD::SizeProperty::RefreshChildren()
{
    if( !GetChildCount() ) return;

    const wxSize &size = wxSizeRefFromVariant( m_value );

    Item(0)->SetValue( (long)size.x );
    Item(1)->SetValue( (long)size.y );
}

wxVariant wxGD::SizeProperty::ChildChanged( wxVariant &thisValue,
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

wxString wxGD::SizeProperty::ValueToString( wxVariant &value, int flags ) const
{
    if( value.GetType() == "wxSize" )
    {
        wxSize &size = wxSizeRefFromVariant( value );
        wxString val   = wxGD::Convert::SizeToString( size );
        value = WXVARIANT( val );
        return val;
    }

    return wxPGProperty::ValueToString( value, flags );
}

bool wxGD::SizeProperty::StringToValue ( wxVariant       &variant,
                                        const wxString  &text, int flags ) const
{
    wxSize size = wxGD::Convert::StringToSize( text );
    variant << size;
    return true;
}
