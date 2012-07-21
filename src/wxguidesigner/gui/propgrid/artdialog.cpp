///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/artdialog.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/06
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "wxguidesigner/gui/propgrid/artdialog.h"

#define ART_ICON( id ) \
    { \
        int ind; \
        wxIcon icon = wxArtProvider::GetIcon( id, client, size ); \
        if( icon.IsOk() ) \
            ind = images->Add( icon ); \
        else \
            ind = 0; \
        list->InsertItem( index, #id, ind ); \
        list->SetItemPtrData( index, wxPtrToUInt( id ) ); \
        index++; \
    }

static void FillBitmaps( wxImageList *images, wxListCtrl *list, int &index,
                        const wxArtClient &client, const wxSize &size )
{
    ART_ICON(wxART_ERROR)
    ART_ICON(wxART_QUESTION)
    ART_ICON(wxART_WARNING)
    ART_ICON(wxART_INFORMATION)
    ART_ICON(wxART_ADD_BOOKMARK)
    ART_ICON(wxART_DEL_BOOKMARK)
    ART_ICON(wxART_HELP_SIDE_PANEL)
    ART_ICON(wxART_HELP_SETTINGS)
    ART_ICON(wxART_HELP_BOOK)
    ART_ICON(wxART_HELP_FOLDER)
    ART_ICON(wxART_HELP_PAGE)
    ART_ICON(wxART_GO_BACK)
    ART_ICON(wxART_GO_FORWARD)
    ART_ICON(wxART_GO_UP)
    ART_ICON(wxART_GO_DOWN)
    ART_ICON(wxART_GO_TO_PARENT)
    ART_ICON(wxART_GO_HOME)
    ART_ICON(wxART_GOTO_FIRST)
    ART_ICON(wxART_GOTO_LAST)
    ART_ICON(wxART_PRINT)
    ART_ICON(wxART_HELP)
    ART_ICON(wxART_TIP)
    ART_ICON(wxART_REPORT_VIEW)
    ART_ICON(wxART_LIST_VIEW)
    ART_ICON(wxART_NEW_DIR)
    ART_ICON(wxART_FOLDER)
    ART_ICON(wxART_FOLDER_OPEN);
    ART_ICON(wxART_GO_DIR_UP)
    ART_ICON(wxART_EXECUTABLE_FILE)
    ART_ICON(wxART_NORMAL_FILE)
    ART_ICON(wxART_TICK_MARK)
    ART_ICON(wxART_CROSS_MARK)
    ART_ICON(wxART_MISSING_IMAGE)
    ART_ICON(wxART_NEW);
    ART_ICON(wxART_FILE_OPEN)
    ART_ICON(wxART_FILE_SAVE)
    ART_ICON(wxART_FILE_SAVE_AS)
    ART_ICON(wxART_DELETE);
    ART_ICON(wxART_COPY)
    ART_ICON(wxART_CUT)
    ART_ICON(wxART_PASTE)
    ART_ICON(wxART_UNDO)
    ART_ICON(wxART_REDO)
    ART_ICON(wxART_PLUS)
    ART_ICON(wxART_MINUS)
    ART_ICON(wxART_QUIT)
    ART_ICON(wxART_FIND)
    ART_ICON(wxART_FIND_AND_REPLACE)
    ART_ICON(wxART_HARDDISK)
    ART_ICON(wxART_FLOPPY)
    ART_ICON(wxART_CDROM)
    ART_ICON(wxART_REMOVABLE)
}
//=============================================================================
// BitmapRequesterDialog
//=============================================================================
BitmapRequesterDialog::BitmapRequesterDialog( wxWindow* parent )
:
wxDialog( parent, wxID_ANY, _("Choose an image"), wxDefaultPosition,
            wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER )
{
//-----------------------------------------------------------------------------
// Controls
//-----------------------------------------------------------------------------
    m_artIdLvw = new wxListView( this, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER |
                                               wxLC_SINGLE_SEL );
    m_artIdLvw->InsertColumn( 0, "wxArtID" );

    wxPanel *rightPnl = new wxPanel( this, wxID_ANY, wxDefaultPosition,
                                    wxDefaultSize, wxTAB_TRAVERSAL );
    wxString clientRbxChoices[] = {
        "wxART_OTHER",
        "wxART_MENU",
        "wxART_BUTTON",
        "wxART_TOOLBAR",
        "wxART_FRAME_ICON",
        "wxART_CMN_DIALOG",
        "wxART_MESSAGE_BOX",
        "wxART_HELP_BROWSER"
    };

    int clientRbxNChoices = sizeof( clientRbxChoices ) / sizeof( wxString );
    wxRadioBox *clientRbx = new wxRadioBox( rightPnl, wxID_ANY, "wxArtClient",
                                            wxDefaultPosition, wxDefaultSize,
                                            clientRbxNChoices, clientRbxChoices,
                                            1, wxRA_SPECIFY_COLS );
    clientRbx->SetSelection( 0 );

    m_bmpArt = new wxStaticBitmap( rightPnl, wxID_ANY, wxNullBitmap );

    wxStaticText *lblSize = new wxStaticText( rightPnl, wxID_ANY, _("Size:") );
    m_lblArt              = new wxStaticText( rightPnl, wxID_ANY, wxEmptyString );
    m_lblArt->Wrap( -1 );
    lblSize->Wrap( -1 );

    wxSpinCtrl *widthSpin = new wxSpinCtrl( rightPnl, wxID_ANY, wxEmptyString,
                                            wxDefaultPosition, wxDefaultSize,
                                            wxSP_ARROW_KEYS, 0, 255, 22 );
    widthSpin->Enable( false );

    wxSpinCtrl *heightSpin = new wxSpinCtrl( rightPnl, wxID_ANY, wxEmptyString,
                                            wxDefaultPosition, wxDefaultSize,
                                            wxSP_ARROW_KEYS, 0, 255, 22 );
    heightSpin->Enable( false );

    wxButton *cancel = new wxButton( rightPnl, wxID_CANCEL, wxEmptyString );
    wxButton *ok     = new wxButton( rightPnl, wxID_OK,     wxEmptyString );
//-----------------------------------------------------------------------------
// Sizers
//-----------------------------------------------------------------------------
    wxBoxSizer *mainSizer   = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *rightSizer  = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *artBS       = new wxBoxSizer( wxVERTICAL );
    wxGridSizer *artGBS     = new wxGridSizer( 0, 2, 0, 0 );

    wxStaticBoxSizer *artIdSBS = new wxStaticBoxSizer
    (
        new wxStaticBox( this, wxID_ANY, "wxArtID" ), wxVERTICAL
    );

    wxStaticBoxSizer *sizer = new wxStaticBoxSizer
    (
        new wxStaticBox( rightPnl, wxID_ANY, _("Size") ), wxHORIZONTAL
    );

    wxStaticBoxSizer* previewSBS = new wxStaticBoxSizer
    (
        new wxStaticBox( rightPnl, wxID_ANY, _("Preview") ), wxVERTICAL
    );

    rightSizer->Add( clientRbx,   1, wxEXPAND, 5 );
    rightSizer->Add( sizer, 0, 0, 5 );
    rightSizer->Add( previewSBS,  0, wxEXPAND, 5 );
    rightSizer->Add( buttonSizer, 0, wxEXPAND, 5 );
    rightSizer->Fit( rightPnl );

    rightPnl->SetSizer( rightSizer );
    rightPnl->Layout();

    buttonSizer->Add( cancel, 1, wxBOTTOM | wxTOP, 5 );
    buttonSizer->Add( ok,     1, wxBOTTOM | wxTOP, 5 );

    artBS->Add( lblSize,  0, wxALL, 5 );
    artBS->Add( m_lblArt, 0, wxALL, 5 );

    artGBS->Add( m_bmpArt, 0, wxALL,    5 );
    artGBS->Add( artBS,    1, wxEXPAND, 5 );

    artIdSBS->Add( m_artIdLvw, 1, wxEXPAND | wxALL );

    sizer->Add( widthSpin,  0, wxBOTTOM, 5 );
    sizer->Add( heightSpin, 0, wxBOTTOM, 5 );

    previewSBS->Add( artGBS,   0, wxEXPAND, 5 );

    mainSizer->SetMinSize( wxSize( 450,450 ) );
    mainSizer->Add( artIdSBS, 1, wxEXPAND | wxLEFT | wxTOP | wxBOTTOM, 5 );
    mainSizer->Add( rightPnl, 0, wxEXPAND | wxALL, 5 );

    this->SetSizerAndFit( mainSizer );
    this->Centre( wxBOTH );

    SetArtClient();

    Bind( wxEVT_COMMAND_RADIOBOX_SELECTED,
            &BitmapRequesterDialog::OnChooseClient, this );

    Bind( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            &BitmapRequesterDialog::OnSelectItem, this );
}

BitmapRequesterDialog::~BitmapRequesterDialog()
{
}

void BitmapRequesterDialog::SetArtClient( const wxArtClient &client )
{
    wxBusyCursor bcur;

    wxImageList *ils = new wxImageList( 16, 16 );
    int index = 0;

    long sel = m_artIdLvw->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED);
    if( sel < 0 )
        sel = 0;

    m_artIdLvw->DeleteAllItems();
    FillBitmaps( ils, m_artIdLvw, index, client, wxSize( 16, 16 ) );
    m_artIdLvw->AssignImageList( ils, wxIMAGE_LIST_SMALL );
    m_artIdLvw->SetColumnWidth( 0, wxLIST_AUTOSIZE );

    m_artIdLvw->SetItemState( sel, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED );

    m_client = client;
    m_id     = (const char*)m_artIdLvw->GetItemData( sel );

    SetArtBitmap( m_id, m_client );
}

void BitmapRequesterDialog::SetArtBitmap  ( const wxArtID       &id,
                                            const wxArtClient   &client,
                                            const wxSize        &size )
{
    wxBitmap bmp = wxArtProvider::GetBitmap( id, client, size );
    m_bmpArt->SetSize( bmp.GetWidth(), bmp.GetHeight() );
    m_bmpArt->SetBitmap( bmp );
    m_lblArt->SetLabel( wxString::Format( "%d x %d", bmp.GetWidth(),
                                                    bmp.GetHeight() ) );
    Refresh();
}

void BitmapRequesterDialog::OnSelectItem( wxListEvent &event )
{
    const char *data = (const char*)event.GetData();
    m_id = data;
    SetArtBitmap( m_id, m_client, wxDefaultSize );
}

void BitmapRequesterDialog::OnChooseClient( wxCommandEvent &event )
{
    switch( event.GetSelection() )
    {
    case 0:
        SetArtClient( wxART_OTHER );
        break;
    case 1:
        SetArtClient( wxART_MENU );
        break;
    case 2:
        SetArtClient( wxART_BUTTON );
        break;
    case 3:
        SetArtClient( wxART_TOOLBAR );
        break;
    case 4:
        SetArtClient( wxART_FRAME_ICON );
        break;
    case 5:
        SetArtClient( wxART_CMN_DIALOG );
        break;
    case 6:
        SetArtClient( wxART_MESSAGE_BOX );
        break;
    case 7:
        SetArtClient( wxART_HELP_BROWSER );
        break;
    }
}
