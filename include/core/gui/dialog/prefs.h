///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/dialog/prefs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/07
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGD_GUI_DIALOG_PREFS_H__
#define __WXGD_GUI_DIALOG_PREFS_H__

class wxBitmapComboBox;
class wxButton;
class wxCheckBox;
class wxDialog;
class wxPanel;
class wxStaticText;
class wxStdDialogButtonSizer;
class wxTreebook;
class wxWindow;

class wxCommandEvent;
class wxUpdateUIEvent;

class wxGDHandler;

wxDECLARE_EVENT( wxGD_EVT_GUI_OPTION_CHANGED, wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_GUI_CONFIG_UPDATE, wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_GUI_CONFIG_SAVE,   wxCommandEvent );
//=============================================================================
// wxGDDialogPrefs
//=============================================================================
class wxGDDialogPrefs : public wxDialog 
{
public:
    wxGDDialogPrefs( wxGDHandler *handler, wxWindow *parent ); 
    ~wxGDDialogPrefs();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxGDHandler             *m_handler;
    wxButton                *m_apply;
    wxButton                *m_ok;
    wxButton                *m_cancel;
    wxStdDialogButtonSizer  *m_btnSizer;
    wxTreebook              *m_tbkPrefs;
};
//=============================================================================
// wxGDPageLocale
//=============================================================================
class wxGDPageLocale : public wxPanel 
{
public:
    wxGDPageLocale( wxGDHandler *handler, wxGDDialogPrefs *parent ); 
    ~wxGDPageLocale();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxGDHandler      *m_handler;
    wxBitmapComboBox *m_bcbLang;
    wxCheckBox       *m_chkLang;
    wxPanel          *m_pnlLocale;
    wxStaticText     *m_lblLang;

    bool m_enabled;
    int  m_selected;
};

#endif //__WXGD_GUI_DIALOG_PREFS_H__
