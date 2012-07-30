///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/property/bitmapdialog.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/06
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPERTY_BITMAPDIALOG_H__
#define __WXGUIDESIGNER_GUI_PROPERTY_BITMAPDIALOG_H__

class wxBoxSizer;
class wxFileCtrl;
class wxListView;
class wxRadioBox;
class wxStaticBitmap;
class wxStaticText;

namespace wxGD
{
namespace Property
{
class BitmapArtBrowser : public wxPanel 
{
public:
    BitmapArtBrowser( wxWindow *parent ); 
    ~BitmapArtBrowser();

    wxArtClient     GetArtClient() const { return m_client; }
    wxArtID         GetArtId()     const { return m_id; }

private:
    void            OnSelectItem    ( wxListEvent       &event );
    void            OnChooseClient  ( wxCommandEvent    &event );

    void            SetArtClient  ( const wxArtClient   &client = wxART_OTHER );
    void            SetArtBitmap  ( const wxArtID       &id,
                                    const wxArtClient   &client = wxART_OTHER,
                                    const wxSize        &size   = wxDefaultSize );
    wxListView      *m_artIdLvw;
    wxStaticBitmap  *m_bmpArt;
    wxStaticText    *m_lblArt;

    wxString        m_client;
    wxString        m_id;
};

class BitmapDialog : public wxDialog 
{
public:
    BitmapDialog( wxWindow *parent, int source = 0 ); 
    ~BitmapDialog();

    void            SetDirectory    ( const wxString &directory );
    void            SetFilterIndex  ( int index );
    int             GetFilterIndex()    const;
    int             GetSelection()      const;
    wxString        GetPath()           const;
    wxString        GetDirectory()      const;
    wxArtClient     GetArtClient()      const;
    wxArtID         GetArtId()          const;

private:
    void            SetSelection( size_t selection );

    void            OnSelectSource  ( wxCommandEvent    &event );
    void            OnFileActivated ( wxFileCtrlEvent   &event );

    wxBoxSizer          *m_sizer;
    BitmapArtBrowser    *m_artReq;
    wxFileCtrl          *m_fileReq;
    wxRadioBox          *m_fileTypeRbx;
    wxString            m_path;
};

}; // namespace Property
}; // namespace wxGD

#endif //__WXGUIDESIGNER_GUI_PROPERTY_BITMAPDIALOG_H__
