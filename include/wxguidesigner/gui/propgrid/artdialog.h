///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/artdialog.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/06
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_PROPGRID_ARTDIALOG_H__
#define __WXGUIDESIGNER_GUI_PROPGRID_ARTDIALOG_H__

#include <wx/artprov.h>
#include <wx/dialog.h>

class wxButton;
class wxListCtrl;
class wxListEvent;
class wxPanel;
class wxRadioBox;
class wxStaticBitmap;
class wxStaticText;
//=============================================================================
// ArtProviderDialog
//=============================================================================
class ArtProviderDialog : public wxDialog 
{
public:
    ArtProviderDialog( wxWindow* parent, wxWindowID id = wxID_ANY,
                        const wxString& title = _("Load From wxArtProvider"),
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
    ~ArtProviderDialog();

    void SetArtBitmap( const wxArtID& id, const wxArtClient& client,
                        const wxSize& size = wxDefaultSize );

    void SetArtClient( const wxArtClient& client );

    wxArtClient GetArtClient()  { return m_client; }
    wxArtID     GetArtId()      { return m_id; }

private:
    void OnSelectItem( wxListEvent &event );
    void OnChooseClient( wxCommandEvent &event );

    wxButton        *m_btnCancel;
    wxButton        *m_btnOK;
    wxListCtrl      *m_lvwArt;
    wxPanel         *m_pnlClient;
    wxPanel         *m_pnlRight;
    wxRadioBox      *m_rbxClient;
    wxStaticText    *m_lblArt;
    wxStaticBitmap  *m_bmpArt;

    wxString m_client;
    wxString m_id;
};

#endif //__WXGUIDESIGNER_GUI_PROPGRID_ARTDIALOG_H__
