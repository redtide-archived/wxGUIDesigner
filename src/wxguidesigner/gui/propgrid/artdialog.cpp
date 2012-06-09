///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/artdialog.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/06
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/gui/propgrid/artdialog.h"

#include <wx/button.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

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

static void FillBitmaps( wxImageList *images, wxListCtrl *list, int& index,
                        const wxArtClient& client, const wxSize& size )
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
// ArtProviderDialog
//=============================================================================

ArtProviderDialog::ArtProviderDialog( wxWindow* parent, wxWindowID id,
                                        const wxString& title,
                                        const wxPoint& pos, const wxSize& size,
                                        long style )
: wxDialog( parent, id, title, pos, size, style )
{
    wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );

    mainSizer->SetMinSize( wxSize( 450,450 ) );

    wxStaticBoxSizer* sbsArtID = new wxStaticBoxSizer
        ( new wxStaticBox( this, wxID_ANY, "wxArtID" ), wxVERTICAL );

    m_lvwArt = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );

    m_lvwArt->InsertColumn( 0, "wxArtID" );

    sbsArtID->Add( m_lvwArt, 1, wxALL|wxEXPAND, 5 );
    mainSizer->Add( sbsArtID, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );

    m_pnlRight = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxTAB_TRAVERSAL );

    wxBoxSizer *rightSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbsPreview = new wxStaticBoxSizer
    (
        new wxStaticBox( m_pnlRight, wxID_ANY, _("Preview") ), wxVERTICAL
    );

	wxGridSizer* gbsArt;
	gbsArt = new wxGridSizer( 0, 2, 0, 0 );
	
	m_bmpArt = new wxStaticBitmap( m_pnlRight, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	gbsArt->Add( m_bmpArt, 0, wxALL, 5 );
	
	wxBoxSizer* bsArt;
	bsArt = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* lblSize;
	lblSize = new wxStaticText( m_pnlRight, wxID_ANY, _("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblSize->Wrap( -1 );
	bsArt->Add( lblSize, 0, wxALL, 5 );
	
	m_lblArt = new wxStaticText( m_pnlRight, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblArt->Wrap( -1 );
	bsArt->Add( m_lblArt, 0, wxALL, 5 );
	
	
	gbsArt->Add( bsArt, 1, wxEXPAND, 5 );
    sbsPreview->Add( gbsArt, 0, wxEXPAND, 5 );

    rightSizer->Add( sbsPreview, 0, wxEXPAND, 5 );

    wxString m_rbxClientChoices[] = {
        "wxART_OTHER",
        "wxART_MENU",
        "wxART_BUTTON",
        "wxART_TOOLBAR",
        "wxART_FRAME_ICON",
        "wxART_CMN_DIALOG",
        "wxART_MESSAGE_BOX",
        "wxART_HELP_BROWSER"
    };

    int m_rbxClientNChoices = sizeof( m_rbxClientChoices ) / sizeof( wxString );
    m_rbxClient = new wxRadioBox( m_pnlRight, wxID_ANY, "wxArtClient",
                                    wxDefaultPosition, wxDefaultSize,
                                    m_rbxClientNChoices, m_rbxClientChoices,
                                    1, wxRA_SPECIFY_COLS );
    m_rbxClient->SetSelection( 0 );
    rightSizer->Add( m_rbxClient, 1, wxEXPAND, 5 );

    wxBoxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

    m_btnCancel = new wxButton( m_pnlRight, wxID_CANCEL, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize, 0 );

    buttonSizer->Add( m_btnCancel, 0, wxBOTTOM|wxTOP, 5 );

    buttonSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    m_btnOK = new wxButton( m_pnlRight, wxID_OK, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize, 0 );

    buttonSizer->Add( m_btnOK, 0, wxBOTTOM|wxTOP, 5 );

    rightSizer->Add( buttonSizer, 0, wxEXPAND, 5 );

    m_pnlRight->SetSizer( rightSizer );
    m_pnlRight->Layout();

    rightSizer->Fit( m_pnlRight );
    mainSizer->Add( m_pnlRight, 0, wxALL|wxEXPAND, 5 );

    this->SetSizerAndFit( mainSizer );
    this->Centre( wxBOTH );

    SetArtClient( wxART_MESSAGE_BOX );

    Bind( wxEVT_COMMAND_RADIOBOX_SELECTED,
            &ArtProviderDialog::OnChooseClient, this );

    Bind( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            &ArtProviderDialog::OnSelectItem, this );
}

ArtProviderDialog::~ArtProviderDialog()
{
}

void ArtProviderDialog::SetArtClient( const wxArtClient& client )
{
    wxBusyCursor bcur;

    wxImageList *ils = new wxImageList( 16, 16 );
    int index = 0;

    long sel = m_lvwArt->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED);
    if( sel < 0 )
        sel = 0;

    m_lvwArt->DeleteAllItems();
    FillBitmaps( ils, m_lvwArt, index, client, wxSize( 16, 16 ) );
    m_lvwArt->AssignImageList( ils, wxIMAGE_LIST_SMALL );
    m_lvwArt->SetColumnWidth( 0, wxLIST_AUTOSIZE );

    m_lvwArt->SetItemState( sel, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED );

    m_client = client;
    m_id     = (const char*)m_lvwArt->GetItemData( sel );

    SetArtBitmap( m_id, m_client );
}

void ArtProviderDialog::SetArtBitmap( const wxArtID& id,
                                        const wxArtClient& client,
                                        const wxSize& size )
{
    wxBitmap bmp = wxArtProvider::GetBitmap( id, client, size );
    m_bmpArt->SetSize( bmp.GetWidth(), bmp.GetHeight() );
    m_bmpArt->SetBitmap( bmp );
    m_lblArt->SetLabel( wxString::Format( "%d x %d", bmp.GetWidth(),
                                                    bmp.GetHeight() ) );
    Refresh();
}

void ArtProviderDialog::OnSelectItem( wxListEvent &event )
{
    const char *data = (const char*)event.GetData();
    m_id = data;
    SetArtBitmap( m_id, m_client, wxDefaultSize );
}

void ArtProviderDialog::OnChooseClient( wxCommandEvent &event )
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
