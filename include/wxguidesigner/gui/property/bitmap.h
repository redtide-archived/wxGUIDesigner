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

namespace wxGD
{
namespace Property
{
//=============================================================================
// wxGD::Property::Bitmap
//=============================================================================
class Bitmap : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( Bitmap )

    friend class BitmapDialogAdapter;

public:
    Bitmap( const wxString &label = wxPG_LABEL,
            const wxString &name  = wxPG_LABEL,
            const wxString &value = wxEmptyString );

    virtual ~Bitmap();

    wxString        ValueToString ( wxVariant &value,
                                    int        flags = 0 ) const;

    virtual wxPGEditorDialogAdapter *GetEditorDialog() const;

    virtual wxSize  OnMeasureImage( int item ) const;
    virtual void    OnCustomPaint( wxDC& dc, const wxRect& rect,
                                    wxPGPaintData& paintdata );
protected:
    wxBitmap        m_bmpThumb; // final thumbnail area
    wxImage         m_imgThumb; // intermediate thumbnail area

    static int      ms_indFilter; // index to the selected filter
    static wxString ms_lastDir;   // last used path
    };
//=============================================================================
// wxGD::Property::BitmapDialogAdapter
//=============================================================================
class BitmapDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    virtual bool    DoShowDialog  ( wxPropertyGrid* propGrid,
                                    wxPGProperty*   property );
};
}; // namespace Property
}; // namespace wxGD

#endif //__WXGUIDESIGNER_GUI_PROPERTY_BITMAP_H__
