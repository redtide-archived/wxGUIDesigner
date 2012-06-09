///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propbook.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/30
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include <wx/imaglist.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/html/htmlwin.h>
#include <wx/propgrid/propgrid.h>
#include <wx/tokenzr.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/gui/propgrid/propbook.h"
#include "wxguidesigner/gui/propgrid/propbmp.h"
#include "wxguidesigner/gui/propgrid/props.h"
#include "wxguidesigner/gui/handler.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/flags.h"
#include "wxguidesigner/rtti/tree.h"
#include "wxguidesigner/events.h"
#include "wxguidesigner/utils.h"

wxGDPropertyBook::wxGDPropertyBook( wxGDHandler *handler, wxWindow* parent )
:
wxNotebook( parent, wxID_ANY ),
m_handler( handler )
{
    m_pgSplitter =
        new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                                wxSP_3D | wxSP_LIVE_UPDATE );
    m_egSplitter =
        new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                                wxSP_3D | wxSP_LIVE_UPDATE );

    wxBoxSizer* PropDescSizer  = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* EventDescSizer = new wxBoxSizer( wxVERTICAL );

    m_pgProps  = new wxPropertyGrid( m_pgSplitter, wxID_ANY,
                                    wxDefaultPosition, wxDefaultSize,
                                    wxPG_BOLD_MODIFIED |
                                    wxPG_SPLITTER_AUTO_CENTER | wxTAB_TRAVERSAL );

    m_pgEvents = new wxPropertyGrid( m_egSplitter, wxID_ANY,
                                    wxDefaultPosition, wxDefaultSize,
                                    wxPG_BOLD_MODIFIED |
                                    wxPG_SPLITTER_AUTO_CENTER | wxTAB_TRAVERSAL);

    m_pgDesc = new wxPanel( m_pgSplitter, wxID_ANY, wxDefaultPosition,
                            wxDefaultSize, wxSUNKEN_BORDER );

    m_egDesc = new wxPanel( m_egSplitter, wxID_ANY, wxDefaultPosition,
                            wxDefaultSize, wxSUNKEN_BORDER );

    m_pgHtml = new wxHtmlWindow( m_pgDesc, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, wxHW_SCROLLBAR_AUTO );

    m_egHtml = new wxHtmlWindow( m_egDesc, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, wxHW_SCROLLBAR_AUTO );

    m_pgDesc->SetBackgroundColour( *wxWHITE );
    m_egDesc->SetBackgroundColour( *wxWHITE );

    m_pgSplitter->SetMinimumPaneSize( 1 );
    m_pgSplitter->SplitHorizontally( m_pgProps, m_pgDesc, 0 );

    m_egSplitter->SetMinimumPaneSize( 1 );
    m_egSplitter->SplitHorizontally( m_pgEvents, m_egDesc, 0 );

    PropDescSizer->Add( m_pgHtml, 1, wxALL | wxEXPAND, 5 );
    PropDescSizer->Fit( m_pgDesc );

    EventDescSizer->Add( m_egHtml, 1, wxALL | wxEXPAND, 5 );
    EventDescSizer->Fit( m_egDesc );

    m_pgDesc->SetSizer( PropDescSizer );
    m_pgDesc->Layout();

    m_egDesc->SetSizer( EventDescSizer );
    m_egDesc->Layout();

    wxImageList *imageList = new wxImageList(16,16);
    AssignImageList( imageList );

    wxBitmap bmpProps   = wxXmlResource::Get()->LoadBitmap("properties");
    wxBitmap bmpEvent   = wxXmlResource::Get()->LoadBitmap("events");
    int      propsIndex = -1, eventIndex = -1;

    if( bmpProps.IsOk() )
        propsIndex = imageList->Add( bmpProps );

    if( bmpEvent.IsOk() )
        eventIndex = imageList->Add( bmpEvent );

    AddPage( m_pgSplitter, _("Properties"), true,  propsIndex );
    AddPage( m_egSplitter, _("Events"),     false, eventIndex );

    Connect( wxEVT_SIZE, wxSizeEventHandler( wxGDPropertyBook::OnSize ), NULL, this );

    m_pgProps->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler
                            ( wxGDPropertyBook::OnPropGridChanged ), NULL, this );

    m_pgProps->Connect( wxEVT_PG_SELECTED, wxPropertyGridEventHandler
                            ( wxGDPropertyBook::OnPropGridSelected ), NULL, this );

    m_pgEvents->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler
                            ( wxGDPropertyBook::OnEventGridChanged ), NULL, this );

    m_pgEvents->Connect( wxEVT_PG_SELECTED, wxPropertyGridEventHandler
                            ( wxGDPropertyBook::OnEventGridSelected ), NULL, this );

    m_pgEvents->Connect( wxEVT_PG_DOUBLE_CLICK, wxPropertyGridEventHandler
                        ( wxGDPropertyBook::OnEventGridLeftDClick ), NULL, this );

    m_pgHtml->Connect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler
                                ( wxGDPropertyBook::OnLinkClicked ), NULL, this );

    m_egHtml->Connect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler
                                ( wxGDPropertyBook::OnLinkClicked ), NULL, this );

    Bind( wxGD_EVT_OBJECT_SELECTED, &wxGDPropertyBook::OnObjectSelected, this );
}

wxGDPropertyBook::~wxGDPropertyBook()
{
    Disconnect( wxEVT_SIZE, wxSizeEventHandler( wxGDPropertyBook::OnSize ), NULL, this );

    m_pgProps->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler
                            ( wxGDPropertyBook::OnPropGridChanged ), NULL, this );

    m_pgEvents->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler
                            ( wxGDPropertyBook::OnEventGridChanged ), NULL, this );

    m_pgEvents->Disconnect( wxEVT_LEFT_DCLICK, wxPropertyGridEventHandler
                        ( wxGDPropertyBook::OnEventGridLeftDClick ), NULL, this );

    m_pgHtml->Disconnect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler
                                ( wxGDPropertyBook::OnLinkClicked ), NULL, this );

    m_egHtml->Disconnect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler
                                ( wxGDPropertyBook::OnLinkClicked ), NULL, this );
}

void wxGDPropertyBook::OnSize( wxSizeEvent &event )
{
    int height = event.GetSize().GetHeight();

    if( m_pgSplitter && m_egSplitter )
    {
        m_pgSplitter->SetSashPosition( height * 0.7 );
        m_egSplitter->SetSashPosition( height * 0.7 );
    }

    event.Skip();
}

void wxGDPropertyBook::OnPropGridChanged( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProp  = event.GetProperty();
    if( pgProp )
    {
        wxString propName = pgProp->GetName();
        Property prop =
        m_handler->GetTree()->GetSelectObject()->GetProperty( pgProp->GetName() );

        if( prop )
        {
            PropertyType propType = prop->GetType();

            if( propType == PROPERTY_TYPE_COLOUR )
            {
                wxColourPropertyValue value =
                pgProp->GetValue().GetAny().As< wxColourPropertyValue >();

                Colour col = { value.m_type, value.m_colour };

                prop->SetValue( col );
            }
            else if( propType == PROPERTY_TYPE_BITMAP )
            {
                wxArrayString params  = pgProp->GetValue().GetArrayString();
                wxString      strType = pgProp->GetValueAsString();
                int           bmpType = pgProp->GetChoices().Index( strType );
                size_t        count   = params.GetCount();

                //wxLogDebug("strType:%s", strType);
                //wxLogDebug("bmpType:%i", bmpType);

                if( count )
                {
                    wxString debug  = "params:";
                    wxString artId  = wxEmptyString;

                    for( size_t i = 0; i < count; i++ )
                    {
                        wxString param = params.Item(i);
                        wxString sep = i == 0 ? "" : " ";
                        debug += sep + param;

                        switch( bmpType )
                        {
                            case wxPG_BMP_SRC_ART:
                            {
                                if( i == 0 )
                                {
                                    artId = param;
                                    //wxLogDebug("Saving stock_id:%s", param );
                                    prop->AddAttribute( "stock_id", param );
                                }
                                else if( (i == 1) && !(artId.empty()) )
                                {
                                    // Remove the last '_C' in wxArtClient
                                    param = param.Truncate( param.length() - 2 );
                                    //wxLogDebug("Saving stock_client:%s", param );
                                    prop->AddAttribute( "stock_client", param );
                                }
                                else
                                {
                                    wxLogDebug("invalid param %s", param);
                                }

                                break;
                            }
                            case wxPG_BMP_SRC_FILE:
                            {
                                if( i == 0 )
                                {
                                    //wxLogDebug("Saving path:%s", param );
                                    prop->SetValue( param );
                                }
                                else
                                {
                                    //wxLogDebug("File value is empty");
                                }
                                break;
                            }
                        }
                    }

                    //wxLogDebug( debug );
                }
            }
            else
            {
                prop->SetValue( pgProp->GetValue() );
            }
        }
    }

    event.Skip();
}

void wxGDPropertyBook::OnPropGridSelected( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProp = event.GetProperty();
    if( pgProp )
    {
        wxString propName    = pgProp->GetBaseName();
        wxString description = "<h5>" + pgProp->GetLabel() + "</h5>";
        Object   object      = m_handler->GetTree()->GetSelectObject();

        if( pgProp->IsCategory() )
        {
            // Can't get a baseclass description from the object
            ClassInfo info = ClassInfoDB::Get()->GetClassInfo( propName );

            if( info )
                description += "<p>" + info->GetDescription() + "</p>";
        }
        else if( pgProp->IsSubProperty() )
        {
            wxString parentName = pgProp->GetParent()->GetName();
            Property parent     = object->GetProperty( parentName );
            PropertyInfo info   = parent->GetInfo();

            for( size_t i = 0; i < info->GetChildCount(); i++ )
            {
                PropertyInfo child = info->GetChild( i );
                if( child->GetName() == propName )
                {
                    description += "<p>" + child->GetDescription() + "</p>";
                    break;
                }
            }
        }
        else
        {
            Property prop = object->GetProperty( propName );

            if( prop )
                description += "<p>" + prop->GetDescription() + "</p>";
        }

        m_pgHtml->SetPage( description );
    }

    event.Skip();
}

void wxGDPropertyBook::OnEventGridChanged( wxPropertyGridEvent &event )
{
    // pgProp represents an event type
    wxPGProperty *pgProp  = event.GetProperty();
    if( pgProp && pgProp->GetParent() )
    {
        // Get the event class name from the pgProp parent
        wxPGProperty *pgCat = pgProp->GetParent();
        if( !pgCat )
        {
            event.Skip(); return; // Just to be sure, shouldn't happen
        }

        Event evt = // Get the Event
        m_handler->GetTree()->GetSelectObject()->GetEvent( pgCat->GetName() );

        if( evt )
        {
            // Iterate to find the right event type name
            for( size_t i = 0; i < evt->GetTypeCount(); i++ )
            {
                wxString evtTypeName = pgProp->GetLabel();
                if( evt->GetTypeName( i ) == evtTypeName )
                {
                    evt->SetFunctionName( i, pgProp->GetValueAsString() );
                    break;
                }
            }
        }
    }

    event.Skip();
}

void wxGDPropertyBook::OnEventGridSelected( wxPropertyGridEvent &event )
{
    wxPGProperty *pgPropEvt = event.GetProperty();
    if( pgPropEvt )
    {
        wxPGProperty *pgPropEvtType = NULL;
        if( !pgPropEvt->IsCategory() )
        {
            pgPropEvtType = pgPropEvt;
            pgPropEvt     = pgPropEvtType->GetParent();

            if( !pgPropEvt )
                return;
        }
        wxString evtName = pgPropEvt->GetLabel();
        Event evt = m_handler->GetTree()->GetSelectObject()->GetEvent( evtName );
        wxString desc = "<h5>" + evtName + "</h5>";

        if( pgPropEvtType )
        {
            wxString evtTypeName = pgPropEvtType->GetLabel();
            for( size_t i = 0; i < evt->GetTypeCount(); i++ )
            {
                if( evt->GetTypeName( i ) == evtTypeName )
                {
                    desc = "<h5>" + evtTypeName + "</h5>";
                    desc += "<p>" + evt->GetTypeDescription( i ) + "</p>";
                    break;
                }
            }
        }
        else
        {
            if( !evt->GetDescription().empty() )
                desc += "<p>" + evt->GetDescription() + "</p>";
        }

        m_egHtml->SetPage( desc );
    }
    event.Skip();
}

void wxGDPropertyBook::OnEventGridLeftDClick( wxPropertyGridEvent &event )
{
    wxPGProperty *p = event.GetProperty();
    if( p && !p->IsCategory() )
    {
        wxString evtTypeName( p->GetLabel() );

        evtTypeName.MakeLower().Replace("wxevt_", "");

        wxString          evtFuncName = "On";
        wxStringTokenizer tokenizer( evtTypeName, "_" );
        while( tokenizer.HasMoreTokens() )
        {
            evtFuncName.Append( tokenizer.GetNextToken().MakeCapitalized() );
        }

        Object obj = m_handler->GetTree()->GetSelectObject();
        wxPGProperty *evtCat = p->GetParent();

        if( obj && evtCat )
        {
            Event evt = obj->GetEvent( evtCat->GetLabel() );
            if( evt )
                evt->SetFunctionName( p->GetLabel(), evtFuncName );
        
            p->SetValueFromString( evtFuncName );
        }
    }
}

void wxGDPropertyBook::OnLinkClicked( wxHtmlLinkEvent &event )
{
    wxLaunchDefaultBrowser( event.GetLinkInfo().GetHref() );
}

void wxGDPropertyBook::OnObjectCreated( wxGDObjectEvent &event )
{
    // Do nothing.
}

void wxGDPropertyBook::OnObjectDeleted( wxGDObjectEvent &event )
{
    // Do nothing.
}

void wxGDPropertyBook::OnObjectExpanded( wxGDObjectEvent &event )
{
    // Do nothing.
}

void wxGDPropertyBook::OnObjectSelected( wxGDObjectEvent &event )
{
    Object object = event.GetObject();
    if( !object )
        return;

    LoadProperties( object );
    LoadEvents( object );
}

void wxGDPropertyBook::LoadEvents( Object object )
{
    m_pgEvents->Clear();

    for( size_t i = 0; i < object->GetEventCount(); i++ )
    {
        Event evt = object->GetEvent( i );
        if( evt )
        {
            wxPGProperty *cat = m_pgEvents->Append( new wxPropertyCategory
                                        ( evt->GetName(), wxPG_LABEL ) );

            for( size_t n = 0; n < evt->GetTypeCount(); n++ )
            {
                wxPGProperty *p = cat->AppendChild( new wxStringProperty
                                        ( evt->GetTypeName( n ), wxPG_LABEL ) );

                p->SetValueFromString( evt->GetFunctionName( n ) );
            }
        }
    }
}

void wxGDPropertyBook::LoadProperties( Object object )
{
    m_pgProps->Clear();

    wxPGProperty *mainCat = m_pgProps->Append( new wxPropertyCategory
                                    ( object->GetClassName(), wxPG_LABEL ) );

    for( size_t i = 0; i < object->GetPropertyCount(); i++ )
    {
        Property     prop    = object->GetProperty( i );
        wxPGProperty *pgProp = AddProperty( prop );

        if( pgProp )
        {
            if( pgProp->IsCategory() )
            {
                m_pgProps->Append( pgProp );

                for( size_t n = 0; n < prop->GetChildCount(); n++ )
                {
                    Property        child = prop->GetChild( n );
                    wxPGProperty *pgChild = AddProperty( child );

                    if( pgChild )
                        pgProp->AppendChild( pgChild );
                }
                ClassInfo info =
                    ClassInfoDB::Get()->GetClassInfo( prop->GetName() );

                if( info && info->IsTypeOf( CLASS_TYPE_ABSTRACT ) )
                {
                    pgProp->SetBackgroundColour( wxColour( 255, 255, 225 ) );
                }
                else if( info && info->IsTypeOf( CLASS_TYPE_ITEM ) )
                {
                    pgProp->SetBackgroundColour( wxColour( 225, 255, 255 ) );
                }
            }
            else
            {
                mainCat->AppendChild( pgProp );
            }
        }
    }
    wxPGCell cell; cell.SetBgCol( wxColour( 255, 255, 225 ) );
    m_pgProps->SetUnspecifiedValueAppearance( cell );
}

wxPGProperty *wxGDPropertyBook::AddProperty( Property prop )
{
    if( !prop ) return NULL;

    wxString label = prop->GetLabel();
    wxString name  = prop->GetName();

    switch( prop->GetType() )
    {
        case PROPERTY_TYPE_ARRAYSTRING:
        {
            return new wxArrayStringProperty( label, name,
                                              prop->GetAsArrayString() );
        }
        case PROPERTY_TYPE_BITMAP:
        {
            int           bmpType     = wxPG_BMP_SRC_NONE;
            size_t        attribCount = prop->GetAttributeCount();
            wxArrayString params;

            if( attribCount ) // wxArtProvider
            {
                wxString artId  = wxEmptyString;
                wxString client = wxEmptyString;

                for( size_t i = 0; i < attribCount; i++ )
                {
                    wxString attrName = prop->GetAttribute(i).first;
                    wxString attrVal  = prop->GetAttribute(i).second;

                    if( attrName == "stock_id" )
                    {
                        artId  = attrVal;
                    }
                    else if( attrName == "stock_client" )
                    {
                        client = attrVal + "_C";
                    }
                }

                if( !artId.empty() )
                {
                    bmpType = wxPG_BMP_SRC_ART;

                    params.Add( artId );

                    if( !client.empty() )
                    {
                        params.Add( client );
                    }
                }

                //wxLogDebug( "Loading bmpType:%i ID:%s Client:%s", bmpType, artId, client );

                return new wxBitmapProperty( bmpType, params, label, name );
            }

            wxString value = prop->GetAsString(); // File

            if( !value.empty() )
            {
                bmpType = wxPG_BMP_SRC_FILE;

                params.Add( value );

                //wxLogDebug( "Loading bmpType:%i File:%s", bmpType, value );
            }

            if( bmpType == wxPG_BMP_SRC_NONE )
            {
                //wxLogDebug("Loading empty bitmap");
            }

            return new wxBitmapProperty( bmpType, params, label, name );
        }
        case PROPERTY_TYPE_BOOL:
        {
            wxPGProperty *pgProp = new wxBoolProperty( label, name,
                                                        prop->GetAsBool() );

            pgProp->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );

            return pgProp;
        }
        case PROPERTY_TYPE_CATEGORY:
        {
            return new wxPropertyCategory( label, name );
        }
        case PROPERTY_TYPE_COLOUR:
        {
            Colour col = prop->GetAsColour();

            wxColourPropertyValue colVal( col.type, col.colour );

            wxGDColourProperty
            *pgProp = new wxGDColourProperty( label, name, colVal );

            return pgProp;
        }/*
        case PROPERTY_TYPE_DIMENSION:
        {
            return new wxDimensionProperty( label, name,
                                            prop->GetAsDimension() );
        }*/
        case PROPERTY_TYPE_FLOAT:
        case PROPERTY_TYPE_DOUBLE:
        {
            return new wxFloatProperty( label, name, prop->GetAsDouble() );
        }
        case PROPERTY_TYPE_ENUM:
        {
            wxArrayString flagNames;
            wxArrayInt    flagValues;

            for( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
            {
                wxString flagName = prop->GetInfo()->GetChild( i )->GetName();
                int      flagVal  = wxFlagsManager::Get()->GetFlag( flagName );

                flagNames.Add( flagName );
                flagValues.Add( flagVal );
            }

            return new wxEnumProperty( label, name, flagNames, flagValues,
                                                    prop->GetAsInt() );
        }
        case PROPERTY_TYPE_FONT:
        {
            wxFont font = prop->GetAsFont();

            wxPGProperty *pgProp = new wxFontProperty( label, name, font );

            if( !font.IsOk() )
                pgProp->SetValueToUnspecified();

            return pgProp;
        }
        case PROPERTY_TYPE_INT:
        {
            return new wxIntProperty( label, name, prop->GetAsInt() );
        }
        case PROPERTY_TYPE_NAME:
        case PROPERTY_TYPE_STRING:
        case PROPERTY_TYPE_TEXT:
        {
            return new wxStringProperty( label, name, prop->GetAsString() );
        }
        case PROPERTY_TYPE_POINT:
        {
            return new wxSizeProperty( label, name, prop->GetAsSize() );
        }
        case PROPERTY_TYPE_SIZE:
        {
            return new wxPointProperty( label, name, prop->GetAsPoint() );
        }
        case PROPERTY_TYPE_URL:
        {
            return new wxFileProperty( label, name, prop->GetAsURL() );
        }
        case PROPERTY_TYPE_FLAG:
        case PROPERTY_TYPE_STYLE:
        {
            wxArrayString styleNames;
            wxArrayInt    styleValues;

            for( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
            {
                wxString styleName = prop->GetInfo()->GetChild( i )->GetName();
                int      styleVal  = wxFlagsManager::Get()->GetFlag( styleName );

                styleNames.Add( styleName );
                styleValues.Add( styleVal );
            }

            wxPGProperty *pgProp = new wxFlagsProperty( label, name,
                                                        styleNames, styleValues,
                                                        prop->GetAsInt() );

            pgProp->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );
            return pgProp;
        }
        default:
            return NULL;
    }

    return NULL;
}
