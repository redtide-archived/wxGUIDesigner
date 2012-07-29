///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/dialog/prefs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/07
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
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

wxDECLARE_EVENT( wxGD_EVT_GUI_OPTION_CHANGED, wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_GUI_CONFIG_UPDATE,  wxCommandEvent );
wxDECLARE_EVENT( wxGD_EVT_GUI_CONFIG_SAVE,    wxCommandEvent );
//=============================================================================
// PrefsDialog
//=============================================================================
namespace wxGD
{
class PrefsDialog : public wxDialog 
{
public:
    PrefsDialog( wxWindow *parent ); 
    ~PrefsDialog();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxButton                *m_apply;
    wxButton                *m_ok;
    wxButton                *m_cancel;
    wxStdDialogButtonSizer  *m_btnSizer;
    wxTreebook              *m_tbkPrefs;
};
//=============================================================================
// PageProject
//=============================================================================
class PageProject : public wxPanel 
{
public:
    PageProject( PrefsDialog *parent ); 
    ~PageProject();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxChoice        *m_choPrjVer;
    wxChoice        *m_choPrjEnc;
    wxCheckBox      *m_chkCompat;
    wxCheckListBox  *m_clbCodeGen; // TODO: Load/Save code generator languages

    int  m_selEnc;
    int  m_selXrcVer;
    bool m_compat;
};
//=============================================================================
// PageGUI
//=============================================================================
class PageGUI : public wxPanel 
{
public:
    PageGUI( PrefsDialog *parent ); 
    ~PageGUI();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxCheckBox  *m_chkIcons;

    bool m_smallIcons;
};
//=============================================================================
// PageLocale
//=============================================================================
class PageLocale : public wxPanel 
{
public:
    PageLocale( PrefsDialog *parent ); 
    ~PageLocale();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

    wxBitmapComboBox *m_bcbLang;
    wxCheckBox       *m_chkLang;

    bool m_enabled;
    int  m_selected;
};
//=============================================================================
// PageEditors
//=============================================================================
class PageEditors : public wxPanel 
{
public:
    PageEditors( PrefsDialog *parent ); 
    ~PageEditors();

private:
    void OnPrefsChanged ( wxCommandEvent &event );
    void OnUpdatePrefs  ( wxCommandEvent &event );
    void OnSavePrefs    ( wxCommandEvent &event );

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
};

#endif //__WXGUIDESIGNER_GUI_DIALOG_PREFS_H__
