///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/propgrid/propbook.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/30
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/imaglist.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/splitter.h>
#include <wx/html/htmlwin.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/tokenzr.h>
#include <wx/xrc/xmlres.h>

#include "wxguidesigner/rtti/flags.h"
#include "wxguidesigner/rtti/database.h"
#include "wxguidesigner/rtti/tree.h"

#include "wxguidesigner/events.h"
#include "wxguidesigner/fontcontainer.h"
#include "wxguidesigner/utils.h"

#include "wxguidesigner/gui/propgrid/propbook.h"
#include "wxguidesigner/gui/propgrid/propbmp.h"
#include "wxguidesigner/gui/propgrid/props.h"

#include "wxguidesigner/gui/handler.h"

using namespace wxGD::Convert;

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
    int      propsIndex = -1;
    int      eventIndex = -1;

    if( bmpProps.IsOk() )
        propsIndex = imageList->Add( bmpProps );

    if( bmpEvent.IsOk() )
        eventIndex = imageList->Add( bmpEvent );

    AddPage( m_pgSplitter, _("Properties"), true,  propsIndex );
    AddPage( m_egSplitter, _("Events"),     false, eventIndex );

    Bind( wxEVT_SIZE, &wxGDPropertyBook::OnSize, this );

    m_pgProps->Bind( wxEVT_PG_CHANGED,
                        &wxGDPropertyBook::OnPropGridChanged, this );

    m_pgProps->Bind( wxEVT_PG_SELECTED,
                        &wxGDPropertyBook::OnPropGridSelected, this );

    m_pgEvents->Bind( wxEVT_PG_CHANGED,
                        &wxGDPropertyBook::OnEventGridChanged, this );

    m_pgEvents->Bind( wxEVT_PG_SELECTED,
                        &wxGDPropertyBook::OnEventGridSelected, this );

    m_pgEvents->Bind( wxEVT_PG_DOUBLE_CLICK,
                        &wxGDPropertyBook::OnEventGridLeftDClick, this );

    m_pgHtml->Bind( wxEVT_COMMAND_HTML_LINK_CLICKED,
                        &wxGDPropertyBook::OnLinkClicked, this );

    m_egHtml->Bind( wxEVT_COMMAND_HTML_LINK_CLICKED,
                        &wxGDPropertyBook::OnLinkClicked, this );

    Bind( wxGD_EVT_OBJECT_SELECTED, &wxGDPropertyBook::OnObjectSelected, this );

    Bind( wxEVT_CHILD_FOCUS, &wxGDPropertyBook::OnChildFocus, this );
}

wxGDPropertyBook::~wxGDPropertyBook()
{
}

void wxGDPropertyBook::OnChildFocus( wxChildFocusEvent & )
{
    // Workaround: unwanted scrollbar scrolling when clicking on a property
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
    wxPGProperty *pgProperty = event.GetProperty();
    if( pgProperty )
    {
        Property property =
        m_handler->GetSelectedObject()->GetProperty( pgProperty->GetName() );

        if( property )
        {
            PropertyType type = property->GetType();

            if( type == PROPERTY_TYPE_BOOL )
            {
                bool value = pgProperty->GetValue().GetAny().As< bool >();
                property->SetValue( value );
            }
            else if( type == PROPERTY_TYPE_COLOUR )
            {
                wxColourPropertyValue colourValue =
                pgProperty->GetValue().GetAny().As< wxColourPropertyValue >();

                property->SetValue( colourValue.m_colour, colourValue.m_type );
            }
            else if( type == PROPERTY_TYPE_FONT )
            {
                wxString value = wxEmptyString;

                wxGDFontProperty *fontProperty =
                wxDynamicCast( pgProperty, wxGDFontProperty );

                if( fontProperty )
                    value = fontProperty->GetValue().GetString();
                    
                property->SetValue( value );
            }
            else if( type == PROPERTY_TYPE_BITMAP )
            {
                wxString value   = pgProperty->GetValueAsString();
                int      bmpType = pgProperty->GetChoices().Index( value );

                wxGDBitmapProperty *bmpProperty =
                wxDynamicCast( pgProperty, wxGDBitmapProperty );

                if( bmpProperty )
                {
                    value = bmpProperty->GetValue().GetString();

                    // No need index string representation value for empty values
                    if( value.IsNumber() )
                        value = wxEmptyString;

                    property->SetValue( bmpType, value );
                }
                else
                {
                    property->SetValue( wxEmptyString );
                }
            }
            else if( type == PROPERTY_TYPE_STYLE )
            {
                wxString value = pgProperty->GetValueAsString();
                value.Replace( ", ", "|" );
                property->SetValue( value );
            }
            else if((type == PROPERTY_TYPE_POINT) ||
                    (type == PROPERTY_TYPE_SIZE)  )
            {
                wxString value = pgProperty->GetValueAsString();
                value.Replace( "; ", "," );
                property->SetValue( value );
            }
            else
            {
                wxString value = pgProperty->GetValueAsString();
                property->SetValue( value );
            }

            wxGDPropertyEvent event( wxGD_EVT_PROPERTY_CHANGED, GetId(), property );
            m_handler->SendEvent( event );
        }
    }

    event.Skip();
}

void wxGDPropertyBook::OnPropGridSelected( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProperty = event.GetProperty();
    if( pgProperty )
    {
        wxString propName    = pgProperty->GetBaseName();
        wxString description = "<h5>" + pgProperty->GetLabel() + "</h5>";
        Object   object      = m_handler->GetSelectedObject();

        if( pgProperty->IsCategory() )
        {
            // Can't get a baseclass description from the object
            ClassInfo info = ClassInfoDB::Get()->GetClassInfo( propName );

            if( info )
                description += "<p>" + info->GetDescription() + "</p>";
        }
        else if( pgProperty->IsSubProperty() )
        {
            wxString parentName = pgProperty->GetParent()->GetName();
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
            Property property = object->GetProperty( propName );

            if( property )
                description += "<p>" + property->GetDescription() + "</p>";
        }

        m_pgHtml->SetPage( description );
    }

    event.Skip();
}

void wxGDPropertyBook::OnEventGridChanged( wxPropertyGridEvent &event )
{
    // pgProperty represents an event type
    wxPGProperty *pgProperty  = event.GetProperty();
    if( pgProperty && pgProperty->GetParent() )
    {
        // Get the event class name from the pgProperty parent
        wxPGProperty *pgCat = pgProperty->GetParent();
        if( !pgCat )
        {
            event.Skip(); return; // Just to be sure, shouldn't happen
        }

        Event evt = // Get the Event
        m_handler->GetSelectedObject()->GetEvent( pgCat->GetName() );

        if( evt )
        {
            // Iterate to find the right event type name
            for( size_t i = 0; i < evt->GetTypeCount(); i++ )
            {
                wxString evtTypeName = pgProperty->GetLabel();
                if( evt->GetTypeName( i ) == evtTypeName )
                {
                    evt->SetHandlerName( i, pgProperty->GetValueAsString() );

                    wxGDEvent event( wxGD_EVT_EVENT_CHANGED, GetId(), evt );
                    m_handler->SendEvent( event );

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
        Event evt = m_handler->GetSelectedObject()->GetEvent( evtName );
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

        Object obj = m_handler->GetSelectedObject();
        wxPGProperty *evtCat = p->GetParent();

        if( obj && evtCat )
        {
            Event evt = obj->GetEvent( evtCat->GetLabel() );
            if( evt )
            {
                evt->SetHandlerName( p->GetLabel(), evtFuncName );

                wxGDEvent event( wxGD_EVT_EVENT_CHANGED, GetId(), evt );
                m_handler->SendEvent( event );
            }

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

    if( !object )
        return;

    Events events = object->GetEvents();

    for( Events::const_iterator it = events.begin(); it != events.end(); ++it )
    {
        Event evt = *it;
        if( evt )
        {
            wxPGProperty *cat =
            m_pgEvents->Append( new wxPropertyCategory
                                        ( evt->GetName(), wxPG_LABEL ) );

            for( size_t n = 0; n < evt->GetTypeCount(); n++ )
            {
                wxPGProperty *p =
                cat->AppendChild( new wxStringProperty
                                        ( evt->GetTypeName( n ), wxPG_LABEL ) );

                p->SetValueFromString( evt->GetHandlerName( n ) );
            }
        }
    }
}

void wxGDPropertyBook::LoadProperties( Object object )
{
    m_pgProps->Clear();

    if( !object )
        return;

    wxPGProperty *mainCat =
    m_pgProps->Append( new wxPropertyCategory( object->GetClassName(), wxPG_LABEL ) );

    Properties properties = object->GetProperties();

    for( Properties::const_iterator it = properties.begin();
                                    it != properties.end(); ++it )
    {
        Property     property    = *it;
        wxPGProperty *pgProperty = AddProperty( property );

        if( pgProperty )
        {
            if( pgProperty->IsCategory() )
            {
                m_pgProps->Append( pgProperty );

                Properties children = property->GetChildren();

                for( Properties::const_iterator it = children.begin();
                                                it != children.end(); ++it )
                {
                    wxPGProperty *pgChild = AddProperty( *it );

                    if( pgChild )
                        pgProperty->AppendChild( pgChild );
                }

                ClassInfo info =
                ClassInfoDB::Get()->GetClassInfo( property->GetName() );

                if( info && info->IsTypeOf( CLASS_TYPE_ABSTRACT ) )
                {
                    pgProperty->SetBackgroundColour( wxColour( 255, 255, 225 ) );
                }
                else if( info && info->IsTypeOf( CLASS_TYPE_ITEM ) )
                {
                    pgProperty->SetBackgroundColour( wxColour( 225, 255, 255 ) );
                }
            }
            else
            {
                mainCat->AppendChild( pgProperty );
            }
        }
    }

    wxPGCell cell; cell.SetBgCol( wxColour( 255, 255, 225 ) );
    m_pgProps->SetUnspecifiedValueAppearance( cell );
}

wxPGProperty *wxGDPropertyBook::AddProperty( Property property )
{
    if( !property )
        return NULL;

    wxString     label = property->GetLabel();
    wxString     name  = property->GetName();
    PropertyType type  = property->GetType();

    if( type == PROPERTY_TYPE_ARRAYSTRING )
    {
        return new wxArrayStringProperty( label, name,
                                          property->GetAsArrayString() );
    }
    else if( type == PROPERTY_TYPE_BITMAP )
    {
        wxString value      = property->GetAsString();
        int      bitmapType = StringToBitmapType( value );

        return new wxGDBitmapProperty( bitmapType, value, label, name );
    }
    else if( type == PROPERTY_TYPE_BOOL )
    {
        wxPGProperty *pgProperty = new wxBoolProperty ( label, name,
                                                        property->GetAsBool() );
        pgProperty->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );

        return pgProperty;
    }
    else if( type == PROPERTY_TYPE_CATEGORY )
    {
        return new wxPropertyCategory( label, name );
    }
    else if( type == PROPERTY_TYPE_COLOUR )
    {
        int      type   = property->GetAsSystemColour();
        wxColour colour = property->GetAsColour();

        wxColourPropertyValue colourValue( type, colour );

        wxGDColourProperty
        *pgProperty = new wxGDColourProperty( label, name, colourValue );

        return pgProperty;
    }/*
    else if( type == PROPERTY_TYPE_DIMENSION )
    {
        return new wxDimensionProperty( label, name,
                                        property->GetAsDimension() );
    }*/
    else if( type == PROPERTY_TYPE_FLOAT || type == PROPERTY_TYPE_DOUBLE )
    {
        return new wxFloatProperty( label, name, property->GetAsDouble() );
    }
    else if( type == PROPERTY_TYPE_ENUM )
    {
        wxArrayString flagNames;
        wxArrayInt    flagValues;

        for( size_t i = 0; i < property->GetInfo()->GetChildCount(); i++ )
        {
            wxString flagName = property->GetInfo()->GetChild( i )->GetName();
            int      flagVal  = wxFlagsManager::Get()->GetFlag( flagName );

            flagNames.Add( flagName );
            flagValues.Add( flagVal );
        }

        return new wxEnumProperty ( label, name, flagNames, flagValues,
                                                property->GetAsInteger() );
    }
    else if( type == PROPERTY_TYPE_FONT )
    {
        return new wxGDFontProperty( label, name, property->GetAsFont() );
    }
    else if( type == PROPERTY_TYPE_INT )
    {
        return new wxIntProperty( label, name, property->GetAsInteger() );
    }
    else if( type == PROPERTY_TYPE_NAME   ||
             type == PROPERTY_TYPE_STRING ||
             type == PROPERTY_TYPE_TEXT   )
    {
        return new wxStringProperty( label, name, property->GetAsString() );
    }
    else if( type == PROPERTY_TYPE_SIZE )
    {
        return new wxGDSizeProperty( label, name, property->GetAsSize() );
    }
    else if( type == PROPERTY_TYPE_POINT )
    {
        return new wxGDPointProperty( label, name, property->GetAsPoint() );
    }
    else if( type == PROPERTY_TYPE_URL )
    {
        return new wxFileProperty( label, name, property->GetAsURL() );
    }
    else if( type == PROPERTY_TYPE_FLAG || type == PROPERTY_TYPE_STYLE )
    {
        wxArrayString styleNames;
        wxArrayInt    styleValues;

        for( size_t i = 0; i < property->GetInfo()->GetChildCount(); i++ )
        {
            wxString styleName = property->GetInfo()->GetChild(i)->GetName();
            int      styleVal  = wxFlagsManager::Get()->GetFlag( styleName );

            styleNames.Add( styleName );
            styleValues.Add( styleVal );
        }

        wxPGProperty *pgProperty =
        new wxGDFlagsProperty( label, name, styleNames, styleValues,
                                                property->GetAsInteger() );
        return pgProperty;
    }

    return NULL;
}
