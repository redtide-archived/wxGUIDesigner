///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/dialog/prefs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/07
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_GUI_DIALOG_PREFS_H__
#define __WXGUIDESIGNER_GUI_DIALOG_PREFS_H__

class wxBitmapComboBox;
class wxButton;
class wxCheckBox;
class wxCheckListBox;
class wxChoice;
class wxDialog;
class wxPanel;
class wxSpinCtrl;
class wxStaticText;
class wxStdDialogButtonSizer;
class wxTreebook;
class wxWindow;

class wxCommandEvent;
class wxUpdateUIEvent;

class wxGDHandler;

wxDECLARE_EVENT( wxGD_EVT_GUI_OPTION_CHANGED, wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_GUI_CONFIG_UPDATE,  wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_GUI_CONFIG_SAVE,    wxCommandEvent );
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
// wxGDPageProject
//=============================================================================
class wxGDPageProject : public wxPanel 
{
public:
    wxGDPageProject( wxGDHandler *handler, wxGDDialogPrefs *parent ); 
    ~wxGDPageProject();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxGDHandler     *m_handler;
    wxChoice        *m_choPrjVer;
    wxChoice        *m_choPrjEnc;
    wxChoice        *m_choWxVer;
    wxCheckListBox  *m_clbCodeGen; // TODO: Load/Save code generator languages

    int m_selEnc;
    int m_selXrcVer;
    int m_selWxVer;
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

    bool m_enabled;
    int  m_selected;
};
//=============================================================================
// wxGDPageEditors
//=============================================================================
class wxGDPageEditors : public wxPanel 
{
public:
    wxGDPageEditors( wxGDHandler *handler, wxGDDialogPrefs *parent ); 
    ~wxGDPageEditors();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxGDHandler      *m_handler;
    wxBitmapComboBox *m_bcbEditor;
    wxCheckBox       *m_chkLineNums;
    wxCheckBox       *m_chkGuides;
    wxCheckBox       *m_chkWSpace;
    wxCheckBox       *m_chkEOL;
    wxCheckBox       *m_chkBackspace;
    wxCheckBox       *m_chkUseTabs;
    wxCheckBox       *m_chkTabsIndent;
    wxChoice         *m_choTabsWidth;
    wxSpinCtrl       *m_spnCaretW;

    bool m_showLines;
    bool m_showGuides;
    bool m_viewWSpace;
    bool m_viewEOL;
    bool m_useBSIndent;
    bool m_useTabs;
    bool m_useTabsIndent;
//  int  m_selEditor;
    int  m_selTabsW;
    int  m_caretW;
};

#endif //__WXGUIDESIGNER_GUI_DIALOG_PREFS_H__
