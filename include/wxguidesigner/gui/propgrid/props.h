///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/props.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPGRID_PROPS_H__
#define __WXGUIDESIGNER_GUI_PROPGRID_PROPS_H__
//=============================================================================
// wxGDColourProperty
//=============================================================================
enum{ wxPG_COLOUR_DEFAULT = 0 };

class wxGDColourProperty : public wxSystemColourProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxGDColourProperty )

public:
    wxGDColourProperty( const wxString              &label  = wxPG_LABEL,
                        const wxString              &name   = wxPG_LABEL,
                        const wxColourPropertyValue &value  =
                                                    wxColourPropertyValue() );
    virtual ~wxGDColourProperty();
};
//=============================================================================
// wxGDFlagsProperty
//=============================================================================
class WXDLLIMPEXP_PROPGRID wxGDFlagsProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxGDFlagsProperty )

public:
    wxGDFlagsProperty ( const wxString         &label  = wxPG_LABEL,
                        const wxString         &name   = wxPG_LABEL,
                        const wxArrayString    &labels = wxArrayString(),
                        const wxArrayInt       &values = wxArrayInt(),
                        int                    value   = 0 );

    virtual ~wxGDFlagsProperty();

    virtual void        OnSetValue();
    virtual wxString    ValueToString ( wxVariant &value, int argFlags = 0 ) const;
    virtual bool        StringToValue ( wxVariant &variant,
                                        const wxString &text, int flags ) const;
    virtual wxVariant   ChildChanged  ( wxVariant &thisValue, int childIndex,
                                        wxVariant &childValue ) const;
    virtual void        RefreshChildren();
    virtual bool        DoSetAttribute( const wxString &name, wxVariant &value );

    // GetChoiceSelection needs to overridden since m_choices is
    // used and value is integer, but it is not index.
    virtual int         GetChoiceSelection() const { return wxNOT_FOUND; }

    // helpers
    size_t              GetItemCount() const { return m_choices.GetCount(); }
    const wxString      &GetLabel( size_t index ) const
                        { return m_choices.GetLabel( static_cast<int>(index) ); }
protected:
    // Creates children and sets value.
    void                Init();

    // Converts string id to a relevant bit.
    long                IdToBit( const wxString &id ) const;

    // Used to detect if choices have been changed
    wxPGChoicesData     *m_oldChoicesData;

    // Needed to properly mark changed sub-properties
    long                m_oldValue;
};
//=============================================================================
// wxGDFontProperty
//=============================================================================
class wxGDFontProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxGDFontProperty )

public:
    wxGDFontProperty  ( const wxString          &label = wxPG_LABEL,
                        const wxString          &name  = wxPG_LABEL,
                        const wxFontContainer   &value = *wxNORMAL_FONT );

    virtual ~wxGDFontProperty();

    virtual void        OnSetValue();
    virtual wxString    ValueToString ( wxVariant &value, int flags = 0 ) const;

    virtual bool        OnEvent       ( wxPropertyGrid *propgrid,
                                        wxWindow *primary, wxEvent &event );

    virtual wxVariant   ChildChanged  ( wxVariant &thisValue, int childIndex,
                                        wxVariant &childValue ) const;
    virtual void        RefreshChildren();
};
//=============================================================================
// wxGDPointProperty
//=============================================================================
class wxGDPointProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxGDPointProperty )

public:
    wxGDPointProperty ( const wxString &label = wxPG_LABEL,
                        const wxString &name  = wxPG_LABEL,
                        const wxPoint  &value = wxPoint() );

    virtual ~wxGDPointProperty();

    virtual wxVariant   ChildChanged  ( wxVariant &thisValue,
                                        int       childIndex,
                                        wxVariant &childValue ) const;
    virtual void        RefreshChildren();

protected:
    void DoSetValue( const wxPoint &value ) { m_value = WXVARIANT( value ); }
};
//=============================================================================
// wxGDSizeProperty
//=============================================================================
class wxGDSizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxGDSizeProperty )

public:
    wxGDSizeProperty  ( const wxString &label = wxPG_LABEL,
                        const wxString &name  = wxPG_LABEL,
                        const wxSize   &value = wxSize() );

    virtual ~wxGDSizeProperty();

    virtual wxVariant   ChildChanged  ( wxVariant &thisValue,
                                        int       childIndex,
                                        wxVariant &childValue ) const;
    virtual void        RefreshChildren();

protected:
    void DoSetValue( const wxSize &value ) { m_value = WXVARIANT( value ); }
};

#endif //__WXGUIDESIGNER_GUI_PROPGRID_PROPS_H__
