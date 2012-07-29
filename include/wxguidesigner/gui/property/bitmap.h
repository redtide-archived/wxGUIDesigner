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
//=============================================================================
// BitmapProperty
//=============================================================================
class BitmapProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS( BitmapProperty )

    friend class BitmapDialogAdapter;

public:
    BitmapProperty( const wxString &label = wxPG_LABEL,
                        const wxString &name  = wxPG_LABEL,
                        const wxString &value = wxEmptyString );

    virtual ~BitmapProperty();

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
// BitmapDialogAdapter
//=============================================================================
class BitmapDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    virtual bool    DoShowDialog( wxPropertyGrid* propGrid,
                                    wxPGProperty* property );
};
};

#endif //__WXGUIDESIGNER_GUI_PROPERTY_BITMAP_H__
