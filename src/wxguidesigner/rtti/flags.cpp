///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/flags.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/02/02
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
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

wxGD::Flags::Flags()
{
    Init();
}

wxGD::Flags::~Flags()
{
}

wxGD::Flags *wxGD::Flags::ms_instance = NULL;

wxGD::Flags *wxGD::Flags::Get()
{
    if( !ms_instance )
        ms_instance = new wxGD::Flags;

    return ms_instance;
}

void wxGD::Flags::Free()
{
    if( ms_instance )
    {
        delete ms_instance;
        ms_instance = NULL;
    }
}

void wxGD::Flags::Add( const wxString &name, int value )
{
    m_flagNames.Add( name );
    m_flagValues.Add( value );
}

int wxGD::Flags::Get( const wxString &name )
{
    for( size_t index = 0; index < m_flagValues.size(); index++ )
    {
        if( m_flagNames.Item( index ) == name )
            return m_flagValues.Item( index );
    }

    return 0; // Default style
}

wxString wxGD::Flags::Get( size_t index )
{
    if( index < m_flagNames.GetCount() )
        return m_flagNames.Item( index );

    return "";
}

void wxGD::Flags::Init()
{
    // TODO: seems that 0 is not a valid value for the wxIntProperty,
    // so, if this isn't a bug or those flags with 0 as value (default values)
    // aren't necessary, just comment them

    #define wxGD_ADD_FLAG( style ) Add( #style, style )

    // wxBorder                             Old styles
//  wxGD_ADD_FLAG(wxBORDER_DEFAULT);   // 0
    wxGD_ADD_FLAG(wxBORDER);           // same as wxBORDER_SIMPLE
    wxGD_ADD_FLAG(wxBORDER_NONE);      wxGD_ADD_FLAG(wxNO_BORDER);
//  wxGD_ADD_FLAG(wxDOUBLE_BORDER);    wxGD_ADD_FLAG(wxBORDER_DOUBLE); // deprecated
    wxGD_ADD_FLAG(wxBORDER_THEME);     // replace wxDOUBLE_BORDER
    wxGD_ADD_FLAG(wxBORDER_RAISED);    wxGD_ADD_FLAG(wxRAISED_BORDER);
    wxGD_ADD_FLAG(wxBORDER_SIMPLE);    wxGD_ADD_FLAG(wxSIMPLE_BORDER);
    wxGD_ADD_FLAG(wxBORDER_STATIC);    wxGD_ADD_FLAG(wxSTATIC_BORDER);
    wxGD_ADD_FLAG(wxBORDER_SUNKEN);    wxGD_ADD_FLAG(wxSUNKEN_BORDER);

    // wxDirection
    wxGD_ADD_FLAG(wxTOP);
    wxGD_ADD_FLAG(wxBOTTOM);
    wxGD_ADD_FLAG(wxLEFT);
    wxGD_ADD_FLAG(wxRIGHT);
    wxGD_ADD_FLAG(wxALL);

    // wxStretch
    wxGD_ADD_FLAG(wxEXPAND);
    wxGD_ADD_FLAG(wxSHAPED);

    // wxSizerFlagBits
    wxGD_ADD_FLAG(wxFIXED_MINSIZE);
    wxGD_ADD_FLAG(wxRESERVE_SPACE_EVEN_IF_HIDDEN);

    // wxAlignment
    wxGD_ADD_FLAG(wxALIGN_CENTER);
    wxGD_ADD_FLAG(wxALIGN_LEFT);
    wxGD_ADD_FLAG(wxALIGN_RIGHT);
    wxGD_ADD_FLAG(wxALIGN_TOP);
    wxGD_ADD_FLAG(wxALIGN_BOTTOM);
    wxGD_ADD_FLAG(wxALIGN_CENTER_VERTICAL);
    wxGD_ADD_FLAG(wxALIGN_CENTER_HORIZONTAL);

    // wxWindow Styles
    wxGD_ADD_FLAG(wxHSCROLL);
    wxGD_ADD_FLAG(wxVSCROLL);
    wxGD_ADD_FLAG(wxALWAYS_SHOW_SB);
    wxGD_ADD_FLAG(wxCLIP_CHILDREN);
    wxGD_ADD_FLAG(wxFULL_REPAINT_ON_RESIZE);
    wxGD_ADD_FLAG(wxTAB_TRAVERSAL);
    wxGD_ADD_FLAG(wxTRANSPARENT_WINDOW);
    wxGD_ADD_FLAG(wxWANTS_CHARS);

    // wxWindow Extra Styles
    wxGD_ADD_FLAG(wxWS_EX_BLOCK_EVENTS);
    wxGD_ADD_FLAG(wxWS_EX_VALIDATE_RECURSIVELY);
    wxGD_ADD_FLAG(wxWS_EX_TRANSIENT);
    wxGD_ADD_FLAG(wxWS_EX_CONTEXTHELP);
    wxGD_ADD_FLAG(wxWS_EX_PROCESS_IDLE);
    wxGD_ADD_FLAG(wxWS_EX_PROCESS_UI_UPDATES);

    // wxFrame and wxDialog
    wxGD_ADD_FLAG(wxCAPTION);
    wxGD_ADD_FLAG(wxCLOSE_BOX);
    wxGD_ADD_FLAG(wxICONIZE);
    wxGD_ADD_FLAG(wxMAXIMIZE);
    wxGD_ADD_FLAG(wxMAXIMIZE_BOX);
    wxGD_ADD_FLAG(wxMINIMIZE);
    wxGD_ADD_FLAG(wxMINIMIZE_BOX);
    wxGD_ADD_FLAG(wxRESIZE_BORDER);
    wxGD_ADD_FLAG(wxSTAY_ON_TOP);
    wxGD_ADD_FLAG(wxSYSTEM_MENU);

    // wxFrame styles
    wxGD_ADD_FLAG(wxDEFAULT_FRAME_STYLE);
    wxGD_ADD_FLAG(wxFRAME_NO_TASKBAR);
    wxGD_ADD_FLAG(wxFRAME_FLOAT_ON_PARENT);
    wxGD_ADD_FLAG(wxFRAME_SHAPED);
    wxGD_ADD_FLAG(wxFRAME_TOOL_WINDOW);

    // wxFrame extra styles
    wxGD_ADD_FLAG(wxFRAME_EX_CONTEXTHELP);
    wxGD_ADD_FLAG(wxFRAME_EX_METAL);

    // wxDialog
    wxGD_ADD_FLAG(wxDEFAULT_DIALOG_STYLE); // wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
    wxGD_ADD_FLAG(wxDIALOG_NO_PARENT);

    // wxDialog extra styles
    wxGD_ADD_FLAG(wxDIALOG_EX_CONTEXTHELP);
    wxGD_ADD_FLAG(wxDIALOG_EX_METAL);

    // wxWizard
    wxGD_ADD_FLAG(wxWIZARD_EX_HELPBUTTON);

    wxGD_ADD_FLAG(wxOK);
    wxGD_ADD_FLAG(wxCANCEL);

    // wxAnyButton
    wxGD_ADD_FLAG(wxBU_LEFT);
    wxGD_ADD_FLAG(wxBU_TOP);
    wxGD_ADD_FLAG(wxBU_RIGHT);
    wxGD_ADD_FLAG(wxBU_BOTTOM);
    wxGD_ADD_FLAG(wxBU_EXACTFIT);
    wxGD_ADD_FLAG(wxBU_NOTEXT);

    // wxCheckBox
    wxGD_ADD_FLAG(wxCHK_2STATE);
    wxGD_ADD_FLAG(wxCHK_3STATE);
    wxGD_ADD_FLAG(wxCHK_ALLOW_3RD_STATE_FOR_USER);

    // wxChoice / wxComboBox
    wxGD_ADD_FLAG(wxCB_DROPDOWN);
    wxGD_ADD_FLAG(wxCB_READONLY);
    wxGD_ADD_FLAG(wxCB_SIMPLE);
    wxGD_ADD_FLAG(wxCB_SORT);

    // wxTextCtrl
    wxGD_ADD_FLAG(wxTE_LEFT);
    wxGD_ADD_FLAG(wxTE_CENTER);
    wxGD_ADD_FLAG(wxTE_CENTRE);
    wxGD_ADD_FLAG(wxTE_RIGHT);
    wxGD_ADD_FLAG(wxTE_BESTWRAP);
    wxGD_ADD_FLAG(wxTE_CHARWRAP);
    wxGD_ADD_FLAG(wxTE_DONTWRAP);
    wxGD_ADD_FLAG(wxTE_WORDWRAP);
    wxGD_ADD_FLAG(wxTE_AUTO_URL);
    wxGD_ADD_FLAG(wxTE_CAPITALIZE);
    wxGD_ADD_FLAG(wxTE_MULTILINE);
    wxGD_ADD_FLAG(wxTE_NOHIDESEL);
    wxGD_ADD_FLAG(wxTE_NO_VSCROLL);
    wxGD_ADD_FLAG(wxTE_PROCESS_ENTER);
    wxGD_ADD_FLAG(wxTE_PROCESS_TAB);
    wxGD_ADD_FLAG(wxTE_RICH);
    wxGD_ADD_FLAG(wxTE_RICH2);
    wxGD_ADD_FLAG(wxTE_READONLY);
    wxGD_ADD_FLAG(wxTE_PASSWORD);

    // wxFileCtrl
    wxGD_ADD_FLAG(wxFC_DEFAULT_STYLE);
    wxGD_ADD_FLAG(wxFC_OPEN);
    wxGD_ADD_FLAG(wxFC_SAVE);
    wxGD_ADD_FLAG(wxFC_MULTIPLE);
    wxGD_ADD_FLAG(wxFC_NOSHOWHIDDEN);
    wxGD_ADD_FLAG(wxFC_NOSHOWHIDDEN);

    // wxGauge
    wxGD_ADD_FLAG(wxGA_HORIZONTAL);
    wxGD_ADD_FLAG(wxGA_VERTICAL);
    wxGD_ADD_FLAG(wxGA_SMOOTH);

    // wxGenericDirCtrl
    wxGD_ADD_FLAG(wxDIRCTRL_DIR_ONLY);
    wxGD_ADD_FLAG(wxDIRCTRL_3D_INTERNAL);
    wxGD_ADD_FLAG(wxDIRCTRL_SELECT_FIRST);
    wxGD_ADD_FLAG(wxDIRCTRL_EDIT_LABELS);
    wxGD_ADD_FLAG(wxDIRCTRL_MULTIPLE);

    // wxListBox
    wxGD_ADD_FLAG(wxLB_SINGLE);
    wxGD_ADD_FLAG(wxLB_MULTIPLE);
    wxGD_ADD_FLAG(wxLB_EXTENDED);
    wxGD_ADD_FLAG(wxLB_HSCROLL);
    wxGD_ADD_FLAG(wxLB_ALWAYS_SB);
    wxGD_ADD_FLAG(wxLB_NEEDED_SB);
    wxGD_ADD_FLAG(wxLB_NO_SB);
    wxGD_ADD_FLAG(wxLB_SORT);

    // wxListCtrl
    wxGD_ADD_FLAG(wxLC_LIST);
    wxGD_ADD_FLAG(wxLC_REPORT);
    wxGD_ADD_FLAG(wxLC_VIRTUAL);
    wxGD_ADD_FLAG(wxLC_ICON);
    wxGD_ADD_FLAG(wxLC_SMALL_ICON);
    wxGD_ADD_FLAG(wxLC_ALIGN_TOP);
    wxGD_ADD_FLAG(wxLC_ALIGN_LEFT);
    wxGD_ADD_FLAG(wxLC_AUTOARRANGE);
    wxGD_ADD_FLAG(wxLC_EDIT_LABELS);
    wxGD_ADD_FLAG(wxLC_NO_HEADER);
    wxGD_ADD_FLAG(wxLC_SINGLE_SEL);
    wxGD_ADD_FLAG(wxLC_SORT_ASCENDING);
    wxGD_ADD_FLAG(wxLC_SORT_DESCENDING);
    wxGD_ADD_FLAG(wxLC_HRULES);
    wxGD_ADD_FLAG(wxLC_VRULES);

    // wxRadioBox
    wxGD_ADD_FLAG(wxRA_SPECIFY_ROWS);
    wxGD_ADD_FLAG(wxRA_SPECIFY_COLS);

    // wxRadioButton
    wxGD_ADD_FLAG(wxRB_GROUP);
    wxGD_ADD_FLAG(wxRB_SINGLE);

    // wxScrollBar
    wxGD_ADD_FLAG(wxSB_HORIZONTAL);
    wxGD_ADD_FLAG(wxSB_VERTICAL);

    // wxSlider
    wxGD_ADD_FLAG(wxSL_HORIZONTAL);
    wxGD_ADD_FLAG(wxSL_VERTICAL);
    wxGD_ADD_FLAG(wxSL_AUTOTICKS);
    wxGD_ADD_FLAG(wxSL_MIN_MAX_LABELS);
    wxGD_ADD_FLAG(wxSL_VALUE_LABEL);
    wxGD_ADD_FLAG(wxSL_LABELS);
    wxGD_ADD_FLAG(wxSL_LEFT);
    wxGD_ADD_FLAG(wxSL_RIGHT);
    wxGD_ADD_FLAG(wxSL_TOP);
    wxGD_ADD_FLAG(wxSL_BOTTOM);
    wxGD_ADD_FLAG(wxSL_SELRANGE);
    wxGD_ADD_FLAG(wxSL_INVERSE);

    // wxSpinButton and wxSpinCtrl
    wxGD_ADD_FLAG(wxSP_HORIZONTAL);
    wxGD_ADD_FLAG(wxSP_VERTICAL);
    wxGD_ADD_FLAG(wxSP_ARROW_KEYS);
    wxGD_ADD_FLAG(wxSP_WRAP);

    // wxStaticLine
    wxGD_ADD_FLAG(wxLI_HORIZONTAL);
    wxGD_ADD_FLAG(wxLI_VERTICAL);

    // wxStaticText
    wxGD_ADD_FLAG(wxST_NO_AUTORESIZE);
    wxGD_ADD_FLAG(wxST_ELLIPSIZE_START);
    wxGD_ADD_FLAG(wxST_ELLIPSIZE_MIDDLE);
    wxGD_ADD_FLAG(wxST_ELLIPSIZE_END);

    // wxTreeCtrl
    wxGD_ADD_FLAG(wxTR_EDIT_LABELS);
    wxGD_ADD_FLAG(wxTR_NO_BUTTONS);
    wxGD_ADD_FLAG(wxTR_HAS_BUTTONS);
    wxGD_ADD_FLAG(wxTR_TWIST_BUTTONS);
    wxGD_ADD_FLAG(wxTR_NO_LINES);
    wxGD_ADD_FLAG(wxTR_FULL_ROW_HIGHLIGHT);
    wxGD_ADD_FLAG(wxTR_LINES_AT_ROOT);
    wxGD_ADD_FLAG(wxTR_HIDE_ROOT);
    wxGD_ADD_FLAG(wxTR_ROW_LINES);
    wxGD_ADD_FLAG(wxTR_HAS_VARIABLE_ROW_HEIGHT);
    wxGD_ADD_FLAG(wxTR_SINGLE);
    wxGD_ADD_FLAG(wxTR_MULTIPLE);
    wxGD_ADD_FLAG(wxTR_DEFAULT_STYLE);

    // wxChoicebook
    wxGD_ADD_FLAG(wxCHB_DEFAULT);
    wxGD_ADD_FLAG(wxCHB_TOP);
    wxGD_ADD_FLAG(wxCHB_LEFT);
    wxGD_ADD_FLAG(wxCHB_RIGHT);
    wxGD_ADD_FLAG(wxCHB_BOTTOM);

    // wxCollapsiblePane
    wxGD_ADD_FLAG(wxCP_DEFAULT_STYLE);
    wxGD_ADD_FLAG(wxCP_NO_TLW_RESIZE);

    // wxListbook
    wxGD_ADD_FLAG(wxLB_DEFAULT);
    wxGD_ADD_FLAG(wxLB_TOP);
    wxGD_ADD_FLAG(wxLB_LEFT);
    wxGD_ADD_FLAG(wxLB_RIGHT);
    wxGD_ADD_FLAG(wxLB_BOTTOM);

    // wxNotebook
    wxGD_ADD_FLAG(wxNB_TOP);
    wxGD_ADD_FLAG(wxNB_LEFT);
    wxGD_ADD_FLAG(wxNB_RIGHT);
    wxGD_ADD_FLAG(wxNB_BOTTOM);
    wxGD_ADD_FLAG(wxNB_FIXEDWIDTH);
    wxGD_ADD_FLAG(wxNB_MULTILINE);
    wxGD_ADD_FLAG(wxNB_NOPAGETHEME);
    wxGD_ADD_FLAG(wxNB_FLAT);

    // wxScrolledWindow
    wxGD_ADD_FLAG(wxRETAINED);
    wxGD_ADD_FLAG(wxSB_VERTICAL);

    // wxSplitterWindow
    wxGD_ADD_FLAG(wxSP_3D);
    wxGD_ADD_FLAG(wxSP_THIN_SASH);
    wxGD_ADD_FLAG(wxSP_3DSASH);
    wxGD_ADD_FLAG(wxSP_3DBORDER);
    wxGD_ADD_FLAG(wxSP_BORDER);
    wxGD_ADD_FLAG(wxSP_NOBORDER);
    wxGD_ADD_FLAG(wxSP_NO_XP_THEME);
    wxGD_ADD_FLAG(wxSP_PERMIT_UNSPLIT);
    wxGD_ADD_FLAG(wxSP_LIVE_UPDATE);

    // wxBookCtrl
    wxGD_ADD_FLAG(wxBK_DEFAULT);
    wxGD_ADD_FLAG(wxBK_TOP);
    wxGD_ADD_FLAG(wxBK_BOTTOM);
    wxGD_ADD_FLAG(wxBK_LEFT);
    wxGD_ADD_FLAG(wxBK_RIGHT);

    // wxToolbook
    wxGD_ADD_FLAG(wxTBK_BUTTONBAR);
    wxGD_ADD_FLAG(wxTBK_HORZ_LAYOUT);

   // wxStatusBar
    wxGD_ADD_FLAG(wxSTB_SIZEGRIP);
    wxGD_ADD_FLAG(wxSTB_SHOW_TIPS);
    wxGD_ADD_FLAG(wxSTB_ELLIPSIZE_START);
    wxGD_ADD_FLAG(wxSTB_ELLIPSIZE_MIDDLE);
    wxGD_ADD_FLAG(wxSTB_ELLIPSIZE_END);
    wxGD_ADD_FLAG(wxSTB_DEFAULT_STYLE);

    // wxItemKind
    wxGD_ADD_FLAG(wxITEM_NORMAL);
    wxGD_ADD_FLAG(wxITEM_CHECK);
    wxGD_ADD_FLAG(wxITEM_DROPDOWN);
    wxGD_ADD_FLAG(wxITEM_RADIO);
    wxGD_ADD_FLAG(wxITEM_SEPARATOR);

    // wxMenuBar
    wxGD_ADD_FLAG(wxMB_DOCKABLE);

    // wxToolBar
    wxGD_ADD_FLAG(wxTB_FLAT);
    wxGD_ADD_FLAG(wxTB_DOCKABLE);
    wxGD_ADD_FLAG(wxTB_HORIZONTAL);
    wxGD_ADD_FLAG(wxTB_VERTICAL);
    wxGD_ADD_FLAG(wxTB_TEXT);
    wxGD_ADD_FLAG(wxTB_NOICONS);
    wxGD_ADD_FLAG(wxTB_NODIVIDER);
    wxGD_ADD_FLAG(wxTB_NOALIGN);
    wxGD_ADD_FLAG(wxTB_HORZ_LAYOUT);
    wxGD_ADD_FLAG(wxTB_HORZ_TEXT);
    wxGD_ADD_FLAG(wxTB_NO_TOOLTIPS);
    wxGD_ADD_FLAG(wxTB_BOTTOM);
    wxGD_ADD_FLAG(wxTB_RIGHT);

    // wxColourPickerCtrl
    wxGD_ADD_FLAG(wxCLRP_DEFAULT_STYLE);
    wxGD_ADD_FLAG(wxCLRP_USE_TEXTCTRL);
    wxGD_ADD_FLAG(wxCLRP_SHOW_LABEL);

    // wxDatePickerCtrl
    wxGD_ADD_FLAG(wxDP_SPIN);
    wxGD_ADD_FLAG(wxDP_DROPDOWN);
    wxGD_ADD_FLAG(wxDP_DEFAULT);
    wxGD_ADD_FLAG(wxDP_ALLOWNONE);
    wxGD_ADD_FLAG(wxDP_SHOWCENTURY);

    // wxDirPickerCtrl
    wxGD_ADD_FLAG(wxDIRP_DEFAULT_STYLE);
    wxGD_ADD_FLAG(wxDIRP_USE_TEXTCTRL);
    wxGD_ADD_FLAG(wxDIRP_DIR_MUST_EXIST);
    wxGD_ADD_FLAG(wxDIRP_CHANGE_DIR);
    wxGD_ADD_FLAG(wxDIRP_SMALL);

    // wxFilePickerCtrl
    wxGD_ADD_FLAG(wxFLP_DEFAULT_STYLE);
    wxGD_ADD_FLAG(wxFLP_USE_TEXTCTRL);
    wxGD_ADD_FLAG(wxFLP_OPEN);
    wxGD_ADD_FLAG(wxFLP_SAVE);
    wxGD_ADD_FLAG(wxFLP_OVERWRITE_PROMPT);
    wxGD_ADD_FLAG(wxFLP_FILE_MUST_EXIST);
    wxGD_ADD_FLAG(wxFLP_CHANGE_DIR);
    wxGD_ADD_FLAG(wxFLP_SMALL);

    // wxFontPickerCtrl
    wxGD_ADD_FLAG(wxFNTP_DEFAULT_STYLE);
    wxGD_ADD_FLAG(wxFNTP_USE_TEXTCTRL);
    wxGD_ADD_FLAG(wxFNTP_FONTDESC_AS_LABEL);
    wxGD_ADD_FLAG(wxFNTP_USEFONT_FOR_LABEL);

    // wxOrientation
    wxGD_ADD_FLAG(wxHORIZONTAL);
    wxGD_ADD_FLAG(wxVERTICAL);

    // wxWrapSizer
    wxGD_ADD_FLAG(wxWRAPSIZER_DEFAULT_FLAGS);
    wxGD_ADD_FLAG(wxEXTEND_LAST_ON_EACH_LINE);
    wxGD_ADD_FLAG(wxREMOVE_LEADING_SPACES);
    #undef wxGD_ADD_FLAG
}
