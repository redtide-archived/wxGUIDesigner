///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/artdialog.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/06
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPGRID_ARTDIALOG_H__
#define __WXGUIDESIGNER_GUI_PROPGRID_ARTDIALOG_H__

class wxBoxSizer;
class wxFileCtrl;
class wxListView;
class wxRadioBox;
class wxStaticBitmap;
class wxStaticText;

class BitmapRequesterArtPanel : public wxPanel 
{
public:
    BitmapRequesterArtPanel( wxWindow *parent ); 
    ~BitmapRequesterArtPanel();

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

class BitmapRequesterDialog : public wxDialog 
{
public:
    BitmapRequesterDialog( wxWindow *parent, int source = 0 ); 
    ~BitmapRequesterDialog();

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

    wxBoxSizer              *m_sizer;
    BitmapRequesterArtPanel *m_artReq;
    wxFileCtrl              *m_fileReq;
    wxRadioBox              *m_fileTypeRbx;
    wxString                m_path;
};

#endif //__WXGUIDESIGNER_GUI_PROPGRID_ARTDIALOG_H__
