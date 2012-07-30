///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/property/bitmapdialog.cpp
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
#include <wx/filectrl.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "wxguidesigner/gui/property/bitmapdialog.h"

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
// BitmapArtBrowser
//=============================================================================
wxGD::Property::BitmapArtBrowser::BitmapArtBrowser( wxWindow* parent )
:
wxPanel( parent )
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
 
    wxBoxSizer  *mainSizer  = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer  *rightSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer  *artBS      = new wxBoxSizer( wxVERTICAL );
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
    rightSizer->Fit( rightPnl );

    rightPnl->SetSizer( rightSizer );
    rightPnl->Layout();

    artBS->Add( lblSize,  0, wxALL, 5 );
    artBS->Add( m_lblArt, 0, wxALL, 5 );

    artGBS->Add( m_bmpArt, 0, wxALL,    5 );
    artGBS->Add( artBS,    1, wxEXPAND, 5 );

    artIdSBS->Add( m_artIdLvw, 1, wxEXPAND );

    sizer->Add( widthSpin,  0, wxBOTTOM, 5 );
    sizer->Add( heightSpin, 0, wxBOTTOM, 5 );

    previewSBS->Add( artGBS, 0, wxEXPAND, 5 );

    mainSizer->SetMinSize( wxSize( 450,450 ) );
    mainSizer->Add( artIdSBS, 1, wxEXPAND, 5 );
    mainSizer->Add( rightPnl, 0, wxEXPAND | wxLEFT, 5 );

    SetSizerAndFit( mainSizer );
    Centre( wxBOTH );

    SetArtClient();

    Bind( wxEVT_COMMAND_RADIOBOX_SELECTED,
            &BitmapArtBrowser::OnChooseClient, this );

    Bind( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            &BitmapArtBrowser::OnSelectItem, this );
}

wxGD::Property::BitmapArtBrowser::~BitmapArtBrowser()
{
}

void wxGD::Property::BitmapArtBrowser::SetArtClient( const wxArtClient &client )
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

void wxGD::Property::BitmapArtBrowser::SetArtBitmap(const wxArtID     &id,
                                                    const wxArtClient &client,
                                                    const wxSize      &size)
{
    wxBitmap bmp = wxArtProvider::GetBitmap( id, client, size );
    m_bmpArt->SetSize( bmp.GetWidth(), bmp.GetHeight() );
    m_bmpArt->SetBitmap( bmp );
    m_lblArt->SetLabel( wxString::Format( "%d x %d", bmp.GetWidth(),
                                                     bmp.GetHeight() ) );
    Refresh();
}

void wxGD::Property::BitmapArtBrowser::OnSelectItem( wxListEvent &event )
{
    const char *data = (const char*)event.GetData();
    m_id = data;
    SetArtBitmap( m_id, m_client, wxDefaultSize );
}

void wxGD::Property::BitmapArtBrowser::OnChooseClient( wxCommandEvent &event )
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
//=============================================================================
// BitmapDialog
//=============================================================================
wxGD::Property::BitmapDialog::BitmapDialog( wxWindow* parent, int source )
:
wxDialog( parent, wxID_ANY, _("Open an image file"), wxDefaultPosition,
            wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER )
{
    wxString wildCard = _("All files");
    wildCard.append(" (*.*)|*.*");

    wxList& handlers = wxImage::GetHandlers();
    wxList::iterator node;

    for( node = handlers.begin(); node != handlers.end(); ++node )
    {
        wxImageHandler *handler = wxDynamicCast( *node, wxImageHandler );
        if( handler )
        {
            wxString ext_lo = handler->GetExtension();
            wxString ext_up = ext_lo.Upper();

            wildCard.append("|");
            wildCard.append( ext_up );
            wildCard.append(" ");
            wildCard.append(_("files") );
            wildCard.append(" (*.");
            wildCard.append( ext_up );
            wildCard.append(")|*.");
            wildCard.append( ext_lo );
        }
    }

    wxBoxSizer *buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *mainSizer   = new wxBoxSizer( wxVERTICAL );
    m_sizer                 = new wxBoxSizer( wxVERTICAL );

    m_artReq                = new BitmapArtBrowser( this );
    m_fileReq               = new wxFileCtrl( this, wxID_ANY, wxEmptyString,
                                                wxEmptyString, wildCard );

    wxButton *cancel        = new wxButton( this, wxID_CANCEL, wxEmptyString );
    wxButton *ok            = new wxButton( this, wxID_OK,     wxEmptyString );

    m_sizer->Add( m_artReq,  1, wxEXPAND | wxLEFT | wxRIGHT, 5 );
    m_sizer->Add( m_fileReq, 1, wxEXPAND | wxLEFT | wxRIGHT, 5 );

    m_fileReq->Hide();

    cancel->SetDefault();

    wxString fileTypeValues[] = { _("wxArtProvider"), _("File") };
    int fileTypeChoices = sizeof( fileTypeValues ) / sizeof( wxString );
    m_fileTypeRbx = new wxRadioBox( this, wxID_ANY, _("Source"),
                                    wxDefaultPosition, wxDefaultSize,
                                    fileTypeChoices, fileTypeValues,
                                    2, wxRA_SPECIFY_COLS );
    if( source >= fileTypeChoices )
        source = 0;

    SetSelection( source );

    buttonSizer->Add( cancel, 1, wxBOTTOM | wxALL,  5 );
    buttonSizer->Add( ok,     1, wxBOTTOM | wxALL, 5 );

    mainSizer->Add( m_fileTypeRbx, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP , 5 );
    mainSizer->Add( m_sizer,       1, wxEXPAND, 5 );
    mainSizer->Add( buttonSizer,   0, wxEXPAND, 5 );
    mainSizer->Fit( this );

    SetSizer( mainSizer );
    SetMinSize( wxSize( 480,480 ) );
    Layout();
    Centre( wxBOTH );

    Bind( wxEVT_COMMAND_RADIOBOX_SELECTED,
            &BitmapDialog::OnSelectSource, this );

    Bind( wxEVT_FILECTRL_FILEACTIVATED,
            &BitmapDialog::OnFileActivated, this );
}

wxGD::Property::BitmapDialog::~BitmapDialog()
{
}

wxArtClient wxGD::Property::BitmapDialog::GetArtClient() const
{
    return m_artReq->GetArtClient();
}

wxArtID wxGD::Property::BitmapDialog::GetArtId() const
{
    return m_artReq->GetArtId();
}

void wxGD::Property::BitmapDialog::SetFilterIndex( int index )
{
    m_fileReq->SetFilterIndex( index );
}

int wxGD::Property::BitmapDialog::GetFilterIndex() const
{
    return m_fileReq->GetFilterIndex();
}

wxString wxGD::Property::BitmapDialog::GetPath() const
{
    return m_fileReq->GetPath();
}

wxString wxGD::Property::BitmapDialog::GetDirectory() const
{
    return m_fileReq->GetDirectory();
}

int wxGD::Property::BitmapDialog::GetSelection() const
{
    return m_fileTypeRbx->GetSelection();
}

void wxGD::Property::BitmapDialog::SetDirectory( const wxString &directory )
{
    m_fileReq->SetDirectory( directory );
}

void wxGD::Property::BitmapDialog::SetSelection( size_t selection )
{
    if( selection >= m_fileTypeRbx->GetCount() )
        return;

    m_fileTypeRbx->SetSelection( selection );

    if( !selection )
    {
        m_artReq->Show();
        m_fileReq->Hide();
    }
    else if( selection == 1 )
    {
        m_artReq->Hide();
        m_fileReq->Show();
    }
}

void wxGD::Property::BitmapDialog::OnSelectSource( wxCommandEvent &event )
{
    SetSelection( event.GetSelection() );
    Layout();
}

void wxGD::Property::BitmapDialog::OnFileActivated( wxFileCtrlEvent & )
{
    EndModal( wxID_OK );
}
