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

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
//=============================================================================
// wxSizeProperty
//=============================================================================
class wxSizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxSizeProperty )
public:
    wxSizeProperty( const wxString& label = wxPG_LABEL,
                      const wxString& name  = wxPG_LABEL,
                      const wxSize&   value = wxSize() );

    virtual ~wxSizeProperty();

    virtual wxVariant ChildChanged( wxVariant& thisValue,
                                    int childIndex,
                                    wxVariant& childValue ) const;
    virtual void RefreshChildren();

protected:
    void DoSetValue( const wxSize& value ) { m_value = WXVARIANT( value ); }
};
//=============================================================================
// wxPointProperty
//=============================================================================
class wxPointProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxPointProperty )
public:
    wxPointProperty( const wxString& label = wxPG_LABEL,
                       const wxString& name  = wxPG_LABEL,
                       const wxPoint&  value = wxPoint() );

    virtual ~wxPointProperty();

    virtual wxVariant ChildChanged( wxVariant& thisValue,
                                    int childIndex,
                                    wxVariant& childValue ) const;
    virtual void RefreshChildren();

protected:
    void DoSetValue( const wxPoint& value ) { m_value = WXVARIANT( value ); }
};
//=============================================================================
// wxGDColourProperty
//=============================================================================
enum
{
    wxPG_COLOUR_DEFAULT = 0
};

class wxGDColourProperty : public wxSystemColourProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxGDColourProperty )

public:
    wxGDColourProperty( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
                        const wxColourPropertyValue& value =
                                    wxColourPropertyValue() );
    virtual ~wxGDColourProperty();

private:
    
};

#endif //__WXGUIDESIGNER_GUI_PROPGRID_PROPS_H__
