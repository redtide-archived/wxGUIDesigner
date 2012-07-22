/////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/propbmp.h
// Purpose:     wxGDBitmapProperty and related support classes
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012-05-08
// RCS-ID:      $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
/////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPGRID_PROPBMP_H__
#define __WXGUIDESIGNER_GUI_PROPGRID_PROPBMP_H__
//=============================================================================
// wxGDBitmapProperty
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
class wxGDBitmapProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( wxGDBitmapProperty )

    friend class wxPGBitmapDialogAdapter;

public:
    wxGDBitmapProperty( const wxString &label = wxPG_LABEL,
                        const wxString &name  = wxPG_LABEL,
                        const wxString &value = wxEmptyString );

    virtual ~wxGDBitmapProperty();

    wxString                        ValueToString ( wxVariant &value,
                                                    int        flags = 0 ) const;
//=============================================================================
// wxPGBitmapDialogAdapter
//=============================================================================
    virtual wxPGEditorDialogAdapter *GetEditorDialog() const;
//=============================================================================
// Thumbnail
//=============================================================================
    virtual wxSize                  OnMeasureImage( int item ) const;
    virtual void                    OnCustomPaint( wxDC& dc, const wxRect& rect,
                                                    wxPGPaintData& paintdata );
protected:
    wxBitmap                        m_bmpThumb; // final thumbnail area
    wxImage                         m_imgThumb; // intermediate thumbnail area

    static int                      ms_indFilter; // index to the selected filter
    static wxString                 ms_lastDir;   // last used path
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

#endif //__WXGUIDESIGNER_GUI_PROPGRID_PROPBMP_H__
