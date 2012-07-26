/////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/property/bitmap.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012-05-08
// RCS-ID:      $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
/////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPERTY_BITMAP_H__
#define __WXGUIDESIGNER_GUI_PROPERTY_BITMAP_H__
//=============================================================================
// wxGDBitmapProperty
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

#endif //__WXGUIDESIGNER_GUI_PROPERTY_BITMAP_H__
