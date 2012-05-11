/////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propgrid/propbmp.h
// Purpose:     wxBitmapProperty and related support classes
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012-05-08
// RCS-ID:      $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PROPGRID_BMP_PROPERTY_H_
#define _WX_PROPGRID_BMP_PROPERTY_H_

#include "artdialog.h"

#include <wx/propgrid/propgrid.h>

enum
{
    wxPG_BMP_SRC_NONE = 0,
    wxPG_BMP_SRC_ART = 1,
    wxPG_BMP_SRC_FILE = 2
};
// ----------------------------------------------------------------------------
// wxBitmapPropertyValue
// ----------------------------------------------------------------------------

class wxBitmapPropertyValue : public wxObject
{
public:
    int             m_source;
    wxBitmap        m_bitmap;
    wxArrayString   m_vals;

    wxBitmapPropertyValue() : wxObject()
    {
        m_source = wxPG_BMP_SRC_NONE;
        m_bitmap = wxNullBitmap;
        m_vals   = wxArrayString();
    }

    virtual ~wxBitmapPropertyValue()
    {
    }

    wxBitmapPropertyValue( const wxBitmapPropertyValue& v ) : wxObject()
    {
        m_source = v.m_source;
        m_bitmap = v.m_bitmap;
        m_vals   = v.m_vals;
    }

    void Init( int source, const wxBitmap& bitmap, const wxArrayString& values )
    {
        m_source = source;
        m_bitmap = bitmap;
        m_vals   = values;
    }

    wxBitmapPropertyValue( int source, const wxBitmap& bitmap,
                            const wxArrayString& values ) : wxObject()
    {
        Init( source, bitmap, values );
    }

    void operator=(const wxBitmapPropertyValue& cpv)
    {
        if( this != &cpv )
            Init( cpv.m_source, cpv.m_bitmap, cpv.m_vals );
    }

private:
    DECLARE_DYNAMIC_CLASS( wxBitmapPropertyValue )
};

bool operator==(const wxBitmapPropertyValue&, const wxBitmapPropertyValue&);

DECLARE_VARIANT_OBJECT( wxBitmapPropertyValue )

// ----------------------------------------------------------------------------
// wxPGBitmapDialogAdapter
// ----------------------------------------------------------------------------

class wxPGBitmapDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    virtual bool DoShowDialog( wxPropertyGrid* propGrid,
                               wxPGProperty*   property );
};
// ----------------------------------------------------------------------------
// wxBitmapProperty
// ----------------------------------------------------------------------------

class wxBitmapProperty : public wxEnumProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxBitmapProperty )

    friend class wxPGBitmapDialogAdapter;

public:
    wxBitmapProperty( const wxString&               label = wxPG_LABEL,
                      const wxString&               name  = wxPG_LABEL,
                      const wxBitmapPropertyValue&  value =
                                                    wxBitmapPropertyValue() );
    virtual ~wxBitmapProperty();

    virtual bool IntToValue( wxVariant& variant,
                             int number,
                             int argFlags = 0 ) const;

    virtual bool StringToValue( wxVariant& variant, const wxString& text,
                                int argFlags = 0 ) const;

    virtual wxString ValueToString( wxVariant& value, int argFlags = 0 ) const;

    virtual void OnSetValue();

    virtual wxPGEditorDialogAdapter *GetEditorDialog() const;

    // Thumbnail
    virtual wxSize OnMeasureImage( int item ) const;
    virtual void OnCustomPaint( wxDC& dc, const wxRect& rect,
                                wxPGPaintData& paintdata );

    wxBitmapPropertyValue GetVal( const wxVariant* pVariant = NULL ) const;

protected:
    void Init( int source, const wxBitmap& bitmap, const wxArrayString& values );

    void DoSetThumbNail( const wxString& path );
    void DoSetThumbNail( const wxArtID& id, const wxArtClient& client );

    wxBitmap*   m_pBitmap; // final thumbnail area
    wxImage*    m_pImage;  // intermediate thumbnail area

    static int      ms_indFilter; // index to the selected filter
    static wxString ms_lastDir;   // last used path
};

#endif //_WX_PROPGRID_BMP_PROPERTY_H_
