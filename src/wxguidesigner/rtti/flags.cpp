///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/flags.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/02/02
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/rtti/flags.h"

#include <wx/anybutton.h>
#include <wx/checkbox.h>
#include <wx/choicebk.h>
#include <wx/clrpicker.h>
#include <wx/collpane.h>
#include <wx/datectrl.h>
#include <wx/defs.h>
#include <wx/dialog.h>
#include <wx/dirctrl.h>
#include <wx/filectrl.h>
#include <wx/filepicker.h>
#include <wx/fontpicker.h>
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/scrolwin.h>
#include <wx/slider.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/toolbar.h>
#include <wx/toolbook.h>
#include <wx/toplevel.h>
#include <wx/treectrl.h>
#include <wx/wizard.h>
#include <wx/wrapsizer.h>

wxFlagsManager::wxFlagsManager()
{
    Init();
}

wxFlagsManager::~wxFlagsManager()
{
}

wxFlagsManager *wxFlagsManager::ms_instance = NULL;

wxFlagsManager *wxFlagsManager::Get()
{
    if( !ms_instance )
        ms_instance = new wxFlagsManager;

    return ms_instance;
}

void wxFlagsManager::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void wxFlagsManager::AddFlag( const wxString& name, int value )
{
    m_flagNames.Add( name );
    m_flagValues.Add( value );
}

int wxFlagsManager::GetFlag( const wxString &name )
{
    for( size_t i = 0; i < m_flagValues.GetCount(); i++ )
    {
        if( m_flagNames.Item( i ) == name )
            return m_flagValues.Item( i );
    }

    return 0; // Default style
}

void wxFlagsManager::Init()
{
    // TODO: seems that 0 is not a valid value for the wxIntProperty,
    // so, if this isn't a bug or those flags with 0 as value (default values)
    // aren't necessary, just comment them

    // wxBorder                             Old styles
//  wxFLAG_ADD(wxBORDER_DEFAULT);   // 0
    wxFLAG_ADD(wxBORDER);           // same as wxBORDER_SIMPLE
    wxFLAG_ADD(wxBORDER_NONE);      wxFLAG_ADD(wxNO_BORDER);
//  wxFLAG_ADD(wxDOUBLE_BORDER);    wxFLAG_ADD(wxBORDER_DOUBLE); // deprecated
    wxFLAG_ADD(wxBORDER_THEME);     // replace wxDOUBLE_BORDER
    wxFLAG_ADD(wxBORDER_RAISED);    wxFLAG_ADD(wxRAISED_BORDER);
    wxFLAG_ADD(wxBORDER_SIMPLE);    wxFLAG_ADD(wxSIMPLE_BORDER);
    wxFLAG_ADD(wxBORDER_STATIC);    wxFLAG_ADD(wxSTATIC_BORDER);
    wxFLAG_ADD(wxBORDER_SUNKEN);    wxFLAG_ADD(wxSUNKEN_BORDER);

    // wxDirection
    wxFLAG_ADD(wxTOP);
    wxFLAG_ADD(wxBOTTOM);
    wxFLAG_ADD(wxLEFT);
    wxFLAG_ADD(wxRIGHT);
    wxFLAG_ADD(wxALL);

    // wxStretch
    wxFLAG_ADD(wxEXPAND);
    wxFLAG_ADD(wxSHAPED);

    // wxSizerFlagBits
    wxFLAG_ADD(wxFIXED_MINSIZE);
    wxFLAG_ADD(wxRESERVE_SPACE_EVEN_IF_HIDDEN);

    // wxAlignment
    wxFLAG_ADD(wxALIGN_CENTER);
    wxFLAG_ADD(wxALIGN_LEFT);
    wxFLAG_ADD(wxALIGN_RIGHT);
    wxFLAG_ADD(wxALIGN_TOP);
    wxFLAG_ADD(wxALIGN_BOTTOM);
    wxFLAG_ADD(wxALIGN_CENTER_VERTICAL);
    wxFLAG_ADD(wxALIGN_CENTER_HORIZONTAL);

    // wxWindow Styles
    wxFLAG_ADD(wxHSCROLL);
    wxFLAG_ADD(wxVSCROLL);
    wxFLAG_ADD(wxALWAYS_SHOW_SB);
    wxFLAG_ADD(wxCLIP_CHILDREN);
    wxFLAG_ADD(wxFULL_REPAINT_ON_RESIZE);
    wxFLAG_ADD(wxTAB_TRAVERSAL);
    wxFLAG_ADD(wxTRANSPARENT_WINDOW);
    wxFLAG_ADD(wxWANTS_CHARS);

    // wxWindow Extra Styles
    wxFLAG_ADD(wxWS_EX_BLOCK_EVENTS);
    wxFLAG_ADD(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFLAG_ADD(wxWS_EX_TRANSIENT);
    wxFLAG_ADD(wxWS_EX_CONTEXTHELP);
    wxFLAG_ADD(wxWS_EX_PROCESS_IDLE);
    wxFLAG_ADD(wxWS_EX_PROCESS_UI_UPDATES);

    // wxFrame and wxDialog
    wxFLAG_ADD(wxCAPTION);
    wxFLAG_ADD(wxCLOSE_BOX);
    wxFLAG_ADD(wxICONIZE);
    wxFLAG_ADD(wxMAXIMIZE);
    wxFLAG_ADD(wxMAXIMIZE_BOX);
    wxFLAG_ADD(wxMINIMIZE);
    wxFLAG_ADD(wxMINIMIZE_BOX);
    wxFLAG_ADD(wxRESIZE_BORDER);
    wxFLAG_ADD(wxSTAY_ON_TOP);
    wxFLAG_ADD(wxSYSTEM_MENU);

    // wxFrame styles
    wxFLAG_ADD(wxDEFAULT_FRAME_STYLE);
    wxFLAG_ADD(wxFRAME_NO_TASKBAR);
    wxFLAG_ADD(wxFRAME_FLOAT_ON_PARENT);
    wxFLAG_ADD(wxFRAME_SHAPED);
    wxFLAG_ADD(wxFRAME_TOOL_WINDOW);

    // wxFrame extra styles
    wxFLAG_ADD(wxFRAME_EX_CONTEXTHELP);
    wxFLAG_ADD(wxFRAME_EX_METAL);

    // wxDialog
    wxFLAG_ADD(wxDEFAULT_DIALOG_STYLE); // wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
    wxFLAG_ADD(wxDIALOG_NO_PARENT);

    // wxDialog extra styles
    wxFLAG_ADD(wxDIALOG_EX_CONTEXTHELP);
    wxFLAG_ADD(wxDIALOG_EX_METAL);

    // wxWizard
    wxFLAG_ADD(wxWIZARD_EX_HELPBUTTON);

    wxFLAG_ADD(wxOK);
    wxFLAG_ADD(wxCANCEL);

    // wxAnyButton
    wxFLAG_ADD(wxBU_LEFT);
    wxFLAG_ADD(wxBU_TOP);
    wxFLAG_ADD(wxBU_RIGHT);
    wxFLAG_ADD(wxBU_BOTTOM);
    wxFLAG_ADD(wxBU_EXACTFIT);
    wxFLAG_ADD(wxBU_NOTEXT);

    // wxCheckBox
    wxFLAG_ADD(wxCHK_2STATE);
    wxFLAG_ADD(wxCHK_3STATE);
    wxFLAG_ADD(wxCHK_ALLOW_3RD_STATE_FOR_USER);

    // wxChoice / wxComboBox
    wxFLAG_ADD(wxCB_DROPDOWN);
    wxFLAG_ADD(wxCB_READONLY);
    wxFLAG_ADD(wxCB_SIMPLE);
    wxFLAG_ADD(wxCB_SORT);

    // wxTextCtrl
    wxFLAG_ADD(wxTE_LEFT);
    wxFLAG_ADD(wxTE_CENTER);
    wxFLAG_ADD(wxTE_CENTRE);
    wxFLAG_ADD(wxTE_RIGHT);
    wxFLAG_ADD(wxTE_BESTWRAP);
    wxFLAG_ADD(wxTE_CHARWRAP);
    wxFLAG_ADD(wxTE_DONTWRAP);
    wxFLAG_ADD(wxTE_WORDWRAP);
    wxFLAG_ADD(wxTE_AUTO_URL);
    wxFLAG_ADD(wxTE_CAPITALIZE);
    wxFLAG_ADD(wxTE_MULTILINE);
    wxFLAG_ADD(wxTE_NOHIDESEL);
    wxFLAG_ADD(wxTE_NO_VSCROLL);
    wxFLAG_ADD(wxTE_PROCESS_ENTER);
    wxFLAG_ADD(wxTE_PROCESS_TAB);
    wxFLAG_ADD(wxTE_RICH);
    wxFLAG_ADD(wxTE_RICH2);
    wxFLAG_ADD(wxTE_READONLY);
    wxFLAG_ADD(wxTE_PASSWORD);

    // wxFileCtrl
    wxFLAG_ADD(wxFC_DEFAULT_STYLE);
    wxFLAG_ADD(wxFC_OPEN);
    wxFLAG_ADD(wxFC_SAVE);
    wxFLAG_ADD(wxFC_MULTIPLE);
    wxFLAG_ADD(wxFC_NOSHOWHIDDEN);
    wxFLAG_ADD(wxFC_NOSHOWHIDDEN);

    // wxGauge
    wxFLAG_ADD(wxGA_HORIZONTAL);
    wxFLAG_ADD(wxGA_VERTICAL);
    wxFLAG_ADD(wxGA_SMOOTH);

    // wxGenericDirCtrl
    wxFLAG_ADD(wxDIRCTRL_DIR_ONLY);
    wxFLAG_ADD(wxDIRCTRL_3D_INTERNAL);
    wxFLAG_ADD(wxDIRCTRL_SELECT_FIRST);
    wxFLAG_ADD(wxDIRCTRL_EDIT_LABELS);
    wxFLAG_ADD(wxDIRCTRL_MULTIPLE);

    // wxListBox
    wxFLAG_ADD(wxLB_SINGLE);
    wxFLAG_ADD(wxLB_MULTIPLE);
    wxFLAG_ADD(wxLB_EXTENDED);
    wxFLAG_ADD(wxLB_HSCROLL);
    wxFLAG_ADD(wxLB_ALWAYS_SB);
    wxFLAG_ADD(wxLB_NEEDED_SB);
    wxFLAG_ADD(wxLB_NO_SB);
    wxFLAG_ADD(wxLB_SORT);

    // wxListCtrl
    wxFLAG_ADD(wxLC_LIST);
    wxFLAG_ADD(wxLC_REPORT);
    wxFLAG_ADD(wxLC_VIRTUAL);
    wxFLAG_ADD(wxLC_ICON);
    wxFLAG_ADD(wxLC_SMALL_ICON);
    wxFLAG_ADD(wxLC_ALIGN_TOP);
    wxFLAG_ADD(wxLC_ALIGN_LEFT);
    wxFLAG_ADD(wxLC_AUTOARRANGE);
    wxFLAG_ADD(wxLC_EDIT_LABELS);
    wxFLAG_ADD(wxLC_NO_HEADER);
    wxFLAG_ADD(wxLC_SINGLE_SEL);
    wxFLAG_ADD(wxLC_SORT_ASCENDING);
    wxFLAG_ADD(wxLC_SORT_DESCENDING);
    wxFLAG_ADD(wxLC_HRULES);
    wxFLAG_ADD(wxLC_VRULES);

    // wxRadioBox
    wxFLAG_ADD(wxRA_SPECIFY_ROWS);
    wxFLAG_ADD(wxRA_SPECIFY_COLS);

    // wxRadioButton
    wxFLAG_ADD(wxRB_GROUP);
    wxFLAG_ADD(wxRB_SINGLE);

    // wxScrollBar
    wxFLAG_ADD(wxSB_HORIZONTAL);
    wxFLAG_ADD(wxSB_VERTICAL);

    // wxSlider
    wxFLAG_ADD(wxSL_HORIZONTAL);
    wxFLAG_ADD(wxSL_VERTICAL);
    wxFLAG_ADD(wxSL_AUTOTICKS);
    wxFLAG_ADD(wxSL_MIN_MAX_LABELS);
    wxFLAG_ADD(wxSL_VALUE_LABEL);
    wxFLAG_ADD(wxSL_LABELS);
    wxFLAG_ADD(wxSL_LEFT);
    wxFLAG_ADD(wxSL_RIGHT);
    wxFLAG_ADD(wxSL_TOP);
    wxFLAG_ADD(wxSL_BOTTOM);
    wxFLAG_ADD(wxSL_SELRANGE);
    wxFLAG_ADD(wxSL_INVERSE);

    // wxSpinButton and wxSpinCtrl
    wxFLAG_ADD(wxSP_HORIZONTAL);
    wxFLAG_ADD(wxSP_VERTICAL);
    wxFLAG_ADD(wxSP_ARROW_KEYS);
    wxFLAG_ADD(wxSP_WRAP);

    // wxStaticLine
    wxFLAG_ADD(wxLI_HORIZONTAL);
    wxFLAG_ADD(wxLI_VERTICAL);

    // wxStaticText
    wxFLAG_ADD(wxST_NO_AUTORESIZE);
    wxFLAG_ADD(wxST_ELLIPSIZE_START);
    wxFLAG_ADD(wxST_ELLIPSIZE_MIDDLE);
    wxFLAG_ADD(wxST_ELLIPSIZE_END);

    // wxTreeCtrl
    wxFLAG_ADD(wxTR_EDIT_LABELS);
    wxFLAG_ADD(wxTR_NO_BUTTONS);
    wxFLAG_ADD(wxTR_HAS_BUTTONS);
    wxFLAG_ADD(wxTR_TWIST_BUTTONS);
    wxFLAG_ADD(wxTR_NO_LINES);
    wxFLAG_ADD(wxTR_FULL_ROW_HIGHLIGHT);
    wxFLAG_ADD(wxTR_LINES_AT_ROOT);
    wxFLAG_ADD(wxTR_HIDE_ROOT);
    wxFLAG_ADD(wxTR_ROW_LINES);
    wxFLAG_ADD(wxTR_HAS_VARIABLE_ROW_HEIGHT);
    wxFLAG_ADD(wxTR_SINGLE);
    wxFLAG_ADD(wxTR_MULTIPLE);
    wxFLAG_ADD(wxTR_DEFAULT_STYLE);

    // wxChoicebook
    wxFLAG_ADD(wxCHB_DEFAULT);
    wxFLAG_ADD(wxCHB_TOP);
    wxFLAG_ADD(wxCHB_LEFT);
    wxFLAG_ADD(wxCHB_RIGHT);
    wxFLAG_ADD(wxCHB_BOTTOM);

    // wxCollapsiblePane
    wxFLAG_ADD(wxCP_DEFAULT_STYLE);
    wxFLAG_ADD(wxCP_NO_TLW_RESIZE);

    // wxListbook
    wxFLAG_ADD(wxLB_DEFAULT);
    wxFLAG_ADD(wxLB_TOP);
    wxFLAG_ADD(wxLB_LEFT);
    wxFLAG_ADD(wxLB_RIGHT);
    wxFLAG_ADD(wxLB_BOTTOM);

    // wxNotebook
    wxFLAG_ADD(wxNB_TOP);
    wxFLAG_ADD(wxNB_LEFT);
    wxFLAG_ADD(wxNB_RIGHT);
    wxFLAG_ADD(wxNB_BOTTOM);
    wxFLAG_ADD(wxNB_FIXEDWIDTH);
    wxFLAG_ADD(wxNB_MULTILINE);
    wxFLAG_ADD(wxNB_NOPAGETHEME);
    wxFLAG_ADD(wxNB_FLAT);

    // wxScrolledWindow
    wxFLAG_ADD(wxRETAINED);
    wxFLAG_ADD(wxSB_VERTICAL);

    // wxSplitterWindow
    wxFLAG_ADD(wxSP_3D);
    wxFLAG_ADD(wxSP_THIN_SASH);
    wxFLAG_ADD(wxSP_3DSASH);
    wxFLAG_ADD(wxSP_3DBORDER);
    wxFLAG_ADD(wxSP_BORDER);
    wxFLAG_ADD(wxSP_NOBORDER);
    wxFLAG_ADD(wxSP_NO_XP_THEME);
    wxFLAG_ADD(wxSP_PERMIT_UNSPLIT);
    wxFLAG_ADD(wxSP_LIVE_UPDATE);

    // wxBookCtrl
    wxFLAG_ADD(wxBK_DEFAULT);
    wxFLAG_ADD(wxBK_TOP);
    wxFLAG_ADD(wxBK_BOTTOM);
    wxFLAG_ADD(wxBK_LEFT);
    wxFLAG_ADD(wxBK_RIGHT);

    // wxToolbook
    wxFLAG_ADD(wxTBK_BUTTONBAR);
    wxFLAG_ADD(wxTBK_HORZ_LAYOUT);

   // wxStatusBar
    wxFLAG_ADD(wxSTB_SIZEGRIP);
    wxFLAG_ADD(wxSTB_SHOW_TIPS);
    wxFLAG_ADD(wxSTB_ELLIPSIZE_START);
    wxFLAG_ADD(wxSTB_ELLIPSIZE_MIDDLE);
    wxFLAG_ADD(wxSTB_ELLIPSIZE_END);
    wxFLAG_ADD(wxSTB_DEFAULT_STYLE);

    // wxItemKind
    wxFLAG_ADD(wxITEM_NORMAL);
    wxFLAG_ADD(wxITEM_CHECK);
    wxFLAG_ADD(wxITEM_DROPDOWN);
    wxFLAG_ADD(wxITEM_RADIO);
    wxFLAG_ADD(wxITEM_SEPARATOR);

    // wxMenuBar
    wxFLAG_ADD(wxMB_DOCKABLE);

    // wxToolBar
    wxFLAG_ADD(wxTB_FLAT);
    wxFLAG_ADD(wxTB_DOCKABLE);
    wxFLAG_ADD(wxTB_HORIZONTAL);
    wxFLAG_ADD(wxTB_VERTICAL);
    wxFLAG_ADD(wxTB_TEXT);
    wxFLAG_ADD(wxTB_NOICONS);
    wxFLAG_ADD(wxTB_NODIVIDER);
    wxFLAG_ADD(wxTB_NOALIGN);
    wxFLAG_ADD(wxTB_HORZ_LAYOUT);
    wxFLAG_ADD(wxTB_HORZ_TEXT);
    wxFLAG_ADD(wxTB_NO_TOOLTIPS);
    wxFLAG_ADD(wxTB_BOTTOM);
    wxFLAG_ADD(wxTB_RIGHT);

    // wxColourPickerCtrl
    wxFLAG_ADD(wxCLRP_DEFAULT_STYLE);
    wxFLAG_ADD(wxCLRP_USE_TEXTCTRL);
    wxFLAG_ADD(wxCLRP_SHOW_LABEL);

    // wxDatePickerCtrl
    wxFLAG_ADD(wxDP_SPIN);
    wxFLAG_ADD(wxDP_DROPDOWN);
    wxFLAG_ADD(wxDP_DEFAULT);
    wxFLAG_ADD(wxDP_ALLOWNONE);
    wxFLAG_ADD(wxDP_SHOWCENTURY);

    // wxDirPickerCtrl
    wxFLAG_ADD(wxDIRP_DEFAULT_STYLE);
    wxFLAG_ADD(wxDIRP_USE_TEXTCTRL);
    wxFLAG_ADD(wxDIRP_DIR_MUST_EXIST);
    wxFLAG_ADD(wxDIRP_CHANGE_DIR);
    wxFLAG_ADD(wxDIRP_SMALL);

    // wxFilePickerCtrl
    wxFLAG_ADD(wxFLP_DEFAULT_STYLE);
    wxFLAG_ADD(wxFLP_USE_TEXTCTRL);
    wxFLAG_ADD(wxFLP_OPEN);
    wxFLAG_ADD(wxFLP_SAVE);
    wxFLAG_ADD(wxFLP_OVERWRITE_PROMPT);
    wxFLAG_ADD(wxFLP_FILE_MUST_EXIST);
    wxFLAG_ADD(wxFLP_CHANGE_DIR);
    wxFLAG_ADD(wxFLP_SMALL);

    // wxFontPickerCtrl
    wxFLAG_ADD(wxFNTP_DEFAULT_STYLE);
    wxFLAG_ADD(wxFNTP_USE_TEXTCTRL);
    wxFLAG_ADD(wxFNTP_FONTDESC_AS_LABEL);
    wxFLAG_ADD(wxFNTP_USEFONT_FOR_LABEL);

    // wxOrientation
    wxFLAG_ADD(wxHORIZONTAL);
    wxFLAG_ADD(wxVERTICAL);

    // wxWrapSizer
    wxFLAG_ADD(wxWRAPSIZER_DEFAULT_FLAGS);
    wxFLAG_ADD(wxEXTEND_LAST_ON_EACH_LINE);
    wxFLAG_ADD(wxREMOVE_LEADING_SPACES);
}
