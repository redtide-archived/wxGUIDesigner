///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/property/props.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPERTY_PROPS_H__
#define __WXGUIDESIGNER_GUI_PROPERTY_PROPS_H__

namespace wxGD
{
//=============================================================================
// ColourProperty
//=============================================================================
class ColourProperty : public wxSystemColourProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( ColourProperty )

public:
    ColourProperty( const wxString              &label  = wxPG_LABEL,
                    const wxString              &name   = wxPG_LABEL,
                    const wxColourPropertyValue &value  =
                                                wxColourPropertyValue() );
    virtual ~ColourProperty();

    virtual wxString ColourToString( const wxColour &colour,
                                    int index, int flags = 0 ) const;
};
//=============================================================================
// FlagsProperty
//=============================================================================
class FlagsProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( FlagsProperty )

public:
    FlagsProperty ( const wxString         &label  = wxPG_LABEL,
                    const wxString         &name   = wxPG_LABEL,
                    const wxArrayString    &labels = wxArrayString(),
                    const wxArrayInt       &values = wxArrayInt(),
                    int                    value   = 0 );

    virtual ~FlagsProperty();

    virtual void        OnSetValue();
    virtual wxString    ValueToString ( wxVariant &value, int flags = 0 ) const;
    virtual bool        StringToValue ( wxVariant &variant,
                                        const wxString &text, int flags = 0 ) const;
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
// FontProperty
//=============================================================================
class FontProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( FontProperty )

public:
    FontProperty  ( const wxString          &label = wxPG_LABEL,
                    const wxString          &name  = wxPG_LABEL,
                    const wxFontContainer   &value = *wxNORMAL_FONT );

    virtual ~FontProperty();

    virtual void        OnSetValue();
    virtual wxString    ValueToString ( wxVariant       &value,
                                        int             flags = 0 ) const;

    virtual bool        StringToValue ( wxVariant       &variant,
                                        const wxString  &text,
                                        int             flags = 0 ) const;

    virtual bool        OnEvent       ( wxPropertyGrid *propgrid,
                                        wxWindow *primary, wxEvent &event );

    virtual wxVariant   ChildChanged  ( wxVariant &thisValue, int childIndex,
                                        wxVariant &childValue ) const;
    virtual void        RefreshChildren();
};
//=============================================================================
// PointProperty
//=============================================================================
class PointProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( PointProperty )

public:
    PointProperty ( const wxString &label = wxPG_LABEL,
                    const wxString &name  = wxPG_LABEL,
                    const wxPoint  &value = wxPoint() );

    virtual ~PointProperty();

    virtual wxString    ValueToString ( wxVariant       &value,
                                        int             flags = 0 ) const;

    virtual bool        StringToValue ( wxVariant       &variant,
                                        const wxString  &text,
                                        int             flags = 0 ) const;

    virtual wxVariant   ChildChanged  ( wxVariant       &thisValue,
                                        int             childIndex,
                                        wxVariant       &childValue ) const;

    virtual void        RefreshChildren();

protected:
    void DoSetValue( const wxPoint &value ) { m_value = WXVARIANT( value ); }
};
//=============================================================================
// SizeProperty
//=============================================================================
class SizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( SizeProperty )

public:
    SizeProperty  ( const wxString &label = wxPG_LABEL,
                    const wxString &name  = wxPG_LABEL,
                    const wxSize   &value = wxSize() );

    virtual ~SizeProperty();

    virtual wxString    ValueToString ( wxVariant       &value,
                                        int             flags = 0 ) const;

    virtual bool        StringToValue ( wxVariant       &variant,
                                        const wxString  &text,
                                        int             flags = 0 ) const;

    virtual wxVariant   ChildChanged  ( wxVariant       &thisValue,
                                        int             childIndex,
                                        wxVariant       &childValue ) const;

    virtual void        RefreshChildren();

protected:
    void DoSetValue( const wxSize &value ) { m_value = WXVARIANT( value ); }
};
};

#endif //__WXGUIDESIGNER_GUI_PROPERTY_PROPS_H__
