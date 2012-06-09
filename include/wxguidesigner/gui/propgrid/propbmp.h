/////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/propbmp.h
// Purpose:     wxBitmapProperty and related support classes
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012-05-08
// RCS-ID:      $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPGRID_PROPBMP_H__
#define __WXGUIDESIGNER_GUI_PROPGRID_PROPBMP_H__

#include <wx/propgrid/propgrid.h>

enum
{
    wxPG_BMP_SRC_NONE = 0,
    wxPG_BMP_SRC_ART = 1,
    wxPG_BMP_SRC_FILE = 2
};
//=============================================================================
// wxPGBitmapDialogAdapter
//=============================================================================

class wxPGBitmapDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    virtual bool DoShowDialog( wxPropertyGrid* propGrid,
                               wxPGProperty*   property );
};
//=============================================================================
// wxBitmapProperty
//
// Manages wxBitmap values from different source/index from a wxArrayString.
// Currently, it supports 2 source types plus a default value:
//
// - wxArtProvider:
//    source is equal to 1 (wxPG_BMP_SRC_ART)
//    params.Item(0) is a wxArtID string
//    params.Item(1) is a wxArtClient string
//
// - File:
//    source is equal to 2 (wxPG_BMP_SRC_FILE)
//    params.Item(0) is a file path string
//
// - Default value (no bitmap)
//    source is equal to 0 (wxPG_BMP_SRC_NONE)
//    params is empty
//
//=============================================================================

class wxBitmapProperty : public wxEnumProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxBitmapProperty )

    friend class wxPGBitmapDialogAdapter;

public:
    wxBitmapProperty( int                  source = wxPG_BMP_SRC_NONE,
                      const wxArrayString& params = wxArrayString(),
                      const wxString&      label  = wxPG_LABEL,
                      const wxString&      name   = wxPG_LABEL );

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

protected:
    void Init( int                  source = wxPG_BMP_SRC_NONE,
               const wxArrayString& params = wxArrayString() );

    wxBitmap        m_bmpThumb; // final thumbnail area
    wxImage         m_imgThumb; // intermediate thumbnail area

    static int      ms_indFilter; // index to the selected filter
    static wxString ms_lastDir;   // last used path
};

#endif //__WXGUIDESIGNER_GUI_PROPGRID_PROPBMP_H__
