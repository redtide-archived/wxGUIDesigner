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

#include <map>

class wxListView;
class wxStaticBitmap;
class wxStaticText;

class BitmapRequesterDialog : public wxDialog 
{
public:
    BitmapRequesterDialog ( wxWindow *parent ); 
    ~BitmapRequesterDialog();

    wxArtClient     GetArtClient()  { return m_client; }
    wxArtID         GetArtId()      { return m_id; }

private:
    void            OnSelectItem    ( wxListEvent       &event );
    void            OnChooseClient  ( wxCommandEvent    &event );

    void            SetArtClient  ( const wxArtClient   &client = wxART_OTHER );
    void            SetArtBitmap  ( const wxArtID       &id,
                                    const wxArtClient   &client = wxART_OTHER,
                                    const wxSize        &size   = wxDefaultSize );

    std::map< wxString, wxControl * > m_controls;

    wxListView      *m_artIdLvw;
    wxStaticBitmap  *m_bmpArt;
    wxStaticText    *m_lblArt;

    wxString        m_client;
    wxString        m_id;
};

#endif //__WXGUIDESIGNER_GUI_PROPGRID_ARTDIALOG_H__
