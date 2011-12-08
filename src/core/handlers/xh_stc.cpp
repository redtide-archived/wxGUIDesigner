///////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_stc.h
// Purpose:     Implementation of wxStyledTextCtrl XRC handler.
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011-09-18
// Revision:    $Hash$
// Copyright:   (c) 2011 wxWidgets Team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_STC

//#include "wx/xrc/xh_stc.h"
#include "handlers/xh_stc.h"
#include "wx/stc/stc.h"

wxIMPLEMENT_DYNAMIC_CLASS( wxStyledTextCtrlXmlHandler, wxXmlResourceHandler )

wxStyledTextCtrlXmlHandler::wxStyledTextCtrlXmlHandler() : wxXmlResourceHandler()
{
}

wxObject *wxStyledTextCtrlXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE( stc, wxStyledTextCtrl );

    stc->Create( m_parentAsWindow,
                 GetID(),
                 wxDefaultPosition, wxDefaultSize,
                 GetStyle("style", 0),
                 GetName() );

    SetupWindow( stc );

    if ( HasParam("font") )
    {
        wxFont font = GetFont("font");
        stc->StyleSetFont( wxSTC_STYLE_DEFAULT, font );
    }

    // Line Numbers
    if ( GetBool("line_numbers") )
    {
        stc->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
        stc->SetMarginWidth( 0, stc->TextWidth( wxSTC_STYLE_LINENUMBER, "_99999" ) );
    }
    else
        stc->SetMarginWidth( 0, 0 );

    // Markers
    stc->MarkerDefine( wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_BOXPLUS );
    stc->MarkerDefine( wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_BOXPLUS );
    stc->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_BOXMINUS );
    stc->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
    stc->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY );
    stc->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
    stc->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY );

    stc->MarkerSetBackground( wxSTC_MARKNUM_FOLDER,         wxColour("BLACK") );
    stc->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN,     wxColour("BLACK") );
    stc->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID,  wxColour("BLACK") );
    stc->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND,      wxColour("BLACK") );

    stc->MarkerSetForeground( wxSTC_MARKNUM_FOLDER,         wxColour("WHITE") );
    stc->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN,     wxColour("WHITE") );
    stc->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID,  wxColour("WHITE") );
    stc->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND,      wxColour("WHITE") );

    // Folding
    if ( GetBool("fold") )
    {
        stc->SetMarginType ( 1, wxSTC_MARGIN_SYMBOL );
        stc->SetMarginMask ( 1, wxSTC_MASK_FOLDERS );
        stc->SetMarginWidth ( 1, 16 );
        stc->SetMarginSensitive ( 1, true );

        stc->SetProperty( "fold", "1" );
        stc->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
    }
    else
        stc->SetMarginWidth( 1, 0 );

    // Indentation Guides
    if ( HasParam("indentation_guides") )
        stc->SetIndentationGuides( GetBool("indentation_guides") );

    stc->SetMarginWidth( 2, 0 );

    // Lexers
    if ( HasParam("lexer") )
    {
        int lexer = ( int )GetLong("lexer");

        stc->SetLexer( lexer );

        if ( HasParam("keywords") )
        {
            wxString keywords = GetParamValue("keywords");
            stc->SetKeyWords( 0, keywords );
        }

        // Initialize styles
        stc->StyleClearAll();

        // Provide predefined lexer settings
        switch ( lexer )
        {
            case wxSTC_LEX_CPP:
            {
                stc->StyleSetBold(       wxSTC_C_WORD,           true );
                stc->StyleSetForeground( wxSTC_C_WORD,           *wxBLUE );
                stc->StyleSetForeground( wxSTC_C_STRING,         *wxRED );
                stc->StyleSetForeground( wxSTC_C_STRINGEOL,      *wxRED );
                stc->StyleSetForeground( wxSTC_C_PREPROCESSOR,   wxColour( 49, 106, 197 ) );
                stc->StyleSetForeground( wxSTC_C_COMMENT,        wxColour( 0, 128, 0 ) );
                stc->StyleSetForeground( wxSTC_C_COMMENTLINE,    wxColour( 0, 128, 0 ) );
                stc->StyleSetForeground( wxSTC_C_COMMENTDOC,     wxColour( 0, 128, 0 ) );
                stc->StyleSetForeground( wxSTC_C_COMMENTLINEDOC, wxColour( 0, 128, 0 ) );
                stc->StyleSetForeground( wxSTC_C_NUMBER,         *wxBLUE );
                break;
            }
            case wxSTC_LEX_XML:
            {
                stc->StyleSetBold( wxSTC_H_ATTRIBUTEUNKNOWN, true);
                stc->StyleSetBold( wxSTC_H_ENTITY, true);
                stc->StyleSetForeground( wxSTC_H_DEFAULT,            *wxBLACK );
                stc->StyleSetForeground( wxSTC_H_TAG,                wxTheColourDatabase->Find("STEEL BLUE") );
                stc->StyleSetForeground( wxSTC_H_TAGUNKNOWN,         *wxBLUE );
                stc->StyleSetForeground( wxSTC_H_ATTRIBUTE,          wxTheColourDatabase->Find("CADET BLUE") );
                stc->StyleSetForeground( wxSTC_H_ATTRIBUTEUNKNOWN,   *wxRED );
                stc->StyleSetForeground( wxSTC_H_NUMBER,             *wxBLACK );
                stc->StyleSetForeground( wxSTC_H_DOUBLESTRING,       wxTheColourDatabase->Find("FIREBRICK") );
                stc->StyleSetForeground( wxSTC_H_SINGLESTRING,       wxTheColourDatabase->Find("FIREBRICK") );
                stc->StyleSetForeground( wxSTC_H_OTHER,              *wxBLUE );
                stc->StyleSetForeground( wxSTC_H_COMMENT,            wxTheColourDatabase->Find("GREY") );
                stc->StyleSetForeground( wxSTC_H_ENTITY,             *wxRED );
                stc->StyleSetForeground( wxSTC_H_TAGEND,             *wxBLUE );
                stc->StyleSetForeground( wxSTC_H_XMLSTART,           *wxBLUE );
                stc->StyleSetForeground( wxSTC_H_XMLEND,             *wxBLUE );
                stc->StyleSetForeground( wxSTC_H_CDATA,              *wxRED );
                break;
            }
        }
    }

    // Tabs
    if ( HasParam("use_tabs") )
    {
        stc->SetUseTabs( GetBool("use_tabs") );

        if ( HasParam("tab_indents") )
            stc->SetTabIndents( GetBool("tab_indents") );

        if ( HasParam("tab_width") )
        {
            int tw = ( int )GetLong("tab_width");

            stc->SetTabWidth( tw );
            stc->SetIndent( tw );
        }
    }

    if ( HasParam("backspace_unindents") )
        stc->SetBackSpaceUnIndents( GetBool("backspace_unindents") );

    if ( HasParam("view_eol") )
        stc->SetViewEOL( GetBool("view_eol") );

    if ( HasParam("view_whitespace") )
        stc->SetViewWhiteSpace( ( GetBool("view_whitespace") ) );

    if ( HasParam("caret_width") )
        stc->SetCaretWidth( ( int )GetLong("caret_width") );

    if ( HasParam("text") )
    {
        wxString text = GetText("text");
        stc->SetText( text );
    }

    return stc;
}

bool wxStyledTextCtrlXmlHandler::CanHandle( wxXmlNode *node )
{
    return IsOfClass( node, "wxStyledTextCtrl" );
}

#endif //wxUSE_XRC && wxUSE_STC
