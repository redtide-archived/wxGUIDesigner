///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/props.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/gui/propgrid/props.h"

#include <wx/settings.h>
//=============================================================================
// wxSizeProperty
//=============================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxSizeProperty, wxPGProperty,
                                wxSize, const wxSize&, TextCtrl )

wxSizeProperty::wxSizeProperty( const wxString& label,
                                const wxString& name,
                                const wxSize&   value )
                : wxPGProperty( label, name )
{
    DoSetValue( value );
    AddPrivateChild( new wxIntProperty( _("Width"),  wxPG_LABEL, value.x ) );
    AddPrivateChild( new wxIntProperty( _("Height"), wxPG_LABEL, value.y ) );
}

wxSizeProperty::~wxSizeProperty() {}

void wxSizeProperty::RefreshChildren()
{
    if( !GetChildCount() ) return;

    const wxSize& size = wxSizeRefFromVariant( m_value );

    Item(0)->SetValue( (long)size.x );
    Item(1)->SetValue( (long)size.y );
}

wxVariant wxSizeProperty::ChildChanged( wxVariant& thisValue,
                                        int childIndex,
                                        wxVariant& childValue ) const
{
    wxSize& size = wxSizeRefFromVariant( thisValue );

    int val = childValue.GetLong();
    switch( childIndex )
    {
    case 0:
        size.x = val;
        break;
    case 1:
        size.y = val;
        break;
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

wxPointProperty::wxPointProperty(const wxString& label,
                                const wxString& name,
                                const wxPoint&  value)
                : wxPGProperty( label, name )
{
    DoSetValue( value );
    AddPrivateChild( new wxIntProperty( "X", wxPG_LABEL, value.x ) );
    AddPrivateChild( new wxIntProperty( "Y", wxPG_LABEL, value.y ) );
}

wxPointProperty::~wxPointProperty() { }

void wxPointProperty::RefreshChildren()
{
    if( !GetChildCount() )
        return;

    const wxPoint& point = wxPointRefFromVariant( m_value );

    Item(0)->SetValue( (long)point.x );
    Item(1)->SetValue( (long)point.y );
}

wxVariant wxPointProperty::ChildChanged(wxVariant& thisValue,
                                        int childIndex,
                                        wxVariant& childValue) const
{
    wxPoint& point = wxPointRefFromVariant( thisValue );

    int val = childValue.GetLong();
    switch( childIndex )
    {
    case 0:
        point.x = val;
        break;
    case 1:
        point.y = val;
        break;
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
                               const wxColourPropertyValue&,Choice)

static wxPGChoices gs_wxGDColourProperty_choicesCache;

wxGDColourProperty::wxGDColourProperty( const wxString& label,
                                        const wxString& name,
                                        const wxColourPropertyValue& value )
: wxSystemColourProperty( label, name,
                            gs_cp_labels, gs_cp_values,
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
