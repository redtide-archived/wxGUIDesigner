///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/property/book.cpp
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

#include "wxguidesigner/gui/property/book.h"
#include "wxguidesigner/gui/property/bitmap.h"
#include "wxguidesigner/gui/property/props.h"

#include "wxguidesigner/gui/handler.h"

wxGD::Property::Book::Book( wxGD::Handler *handler, wxWindow* parent )
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

    Bind( wxEVT_SIZE, &Book::OnSize, this );

    m_pgProps->Bind( wxEVT_PG_CHANGED,
                        &Book::OnPropGridChanged, this );

    m_pgProps->Bind( wxEVT_PG_SELECTED,
                        &Book::OnPropGridSelected, this );

    m_pgEvents->Bind( wxEVT_PG_CHANGED,
                        &Book::OnEventGridChanged, this );

    m_pgEvents->Bind( wxEVT_PG_SELECTED,
                        &Book::OnEventGridSelected, this );

    m_pgEvents->Bind( wxEVT_PG_DOUBLE_CLICK,
                        &Book::OnEventGridLeftDClick, this );

    m_pgHtml->Bind( wxEVT_COMMAND_HTML_LINK_CLICKED,
                        &Book::OnLinkClicked, this );

    m_egHtml->Bind( wxEVT_COMMAND_HTML_LINK_CLICKED,
                        &Book::OnLinkClicked, this );

    Bind( wxGD_EVT_OBJECT_SELECTED, &Book::OnObjectSelected, this );

    Bind( wxEVT_CHILD_FOCUS, &Book::OnChildFocus, this );
}

wxGD::Property::Book::~Book()
{
}

void wxGD::Property::Book::OnChildFocus( wxChildFocusEvent & )
{
    // Workaround: unwanted scrollbar scrolling when clicking on a property
}

void wxGD::Property::Book::OnSize( wxSizeEvent &event )
{
    int height = event.GetSize().GetHeight();

    if( m_pgSplitter && m_egSplitter )
    {
        m_pgSplitter->SetSashPosition( height * 0.7 );
        m_egSplitter->SetSashPosition( height * 0.7 );
    }
}

void wxGD::Property::Book::OnPropGridChanged( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProperty = event.GetProperty();
    if( pgProperty )
    {
        RTTI::Property property =
        m_handler->GetSelectedObject()->GetProperty( pgProperty->GetName() );

        if( property )
        {
            wxString value = pgProperty->GetValueAsString();
            property->SetValue( value );

            RTTI::PropertyEvent
                event( wxGD_EVT_PROPERTY_CHANGED, GetId(), property );

            m_handler->SendEvent( event );
        }
    }
}

void wxGD::Property::Book::OnPropGridSelected( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProperty = event.GetProperty();
    if( pgProperty )
    {
        wxString propertyName = pgProperty->GetBaseName();
        wxString description  = "<h5>" + pgProperty->GetLabel() + "</h5>";
        RTTI::Object object   = m_handler->GetSelectedObject();

        if( pgProperty->IsCategory() )
        {
            // Can't get a baseclass description from the object
            RTTI::ClassInfo classInfo =
            RTTI::ClassTree::Get()->GetClassInfo( propertyName );
            if( classInfo )
                description += classInfo->GetDescription();
        }
        else if( pgProperty->IsSubProperty() )
        {
            wxString            parentName  = pgProperty->GetParent()->GetName();
            RTTI::Property      parent      = object->GetProperty( parentName );
            RTTI::PropertyInfo  classInfo   = parent->GetInfo();

            for( size_t i = 0; i < classInfo->GetChildCount(); i++ )
            {
                RTTI::PropertyInfo child = classInfo->GetChild( i );
                if( child->GetName() == propertyName )
                {
                    description += child->GetDescription();
                    break;
                }
            }
        }
        else
        {
            RTTI::Property property = object->GetProperty( propertyName );
            if( property )
                description += property->GetDescription();
        }

        m_pgHtml->SetPage( description );
    }
}

void wxGD::Property::Book::OnEventGridChanged( wxPropertyGridEvent &event )
{
    // pgProperty represents an event type
    wxPGProperty *pgProperty  = event.GetProperty();
    if( pgProperty && pgProperty->GetParent() )
    {
        // Get the event class name from the pgProperty parent
        wxPGProperty *pgCategory = pgProperty->GetParent();
        if( !pgCategory )
            return; // Just to be sure, shouldn't happen

        RTTI::EventProperty gdEvent = // Get the EventProperty
        m_handler->GetSelectedObject()->GetEvent( pgCategory->GetName() );

        if( gdEvent )
        {
            // Iterate to find the right event type name
            for( size_t i = 0; i < gdEvent->GetTypeCount(); i++ )
            {
                wxString evtTypeName = pgProperty->GetLabel();
                if( gdEvent->GetTypeName(i) == evtTypeName )
                {
                    gdEvent->SetHandlerName( i, pgProperty->GetValueAsString() );

                    RTTI::EventPropertyEvent
                        event( wxGD_EVT_EVENT_CHANGED, GetId(), gdEvent );

                    m_handler->SendEvent( event );
                    break;
                }
            }
        }
    }
}

void wxGD::Property::Book::OnEventGridSelected( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProperty = event.GetProperty();
    if( pgProperty )
    {
        wxPGProperty *pgPropEvtType = NULL;
        if( !pgProperty->IsCategory() )
        {
            pgPropEvtType = pgProperty;
            pgProperty    = pgPropEvtType->GetParent();

            if( !pgProperty )
                return;
        }

        wxString            name    = pgProperty->GetLabel();
        RTTI::EventProperty gdEvent =
            m_handler->GetSelectedObject()->GetEvent( name );

        wxString description = "<h5>" + name + "</h5>";

        if( pgPropEvtType )
        {
            wxString typeName = pgPropEvtType->GetLabel();
            for( size_t i = 0; i < gdEvent->GetTypeCount(); i++ )
            {
                if( gdEvent->GetTypeName(i) == typeName )
                {
                    description = "<h5>" + typeName + "</h5>";
                    description += gdEvent->GetTypeDescription(i);
                    break;
                }
            }
        }
        else
        {
            if( !gdEvent->GetDescription().empty() )
                description += gdEvent->GetDescription();
        }

        m_egHtml->SetPage( description );
    }
}

void wxGD::Property::Book::OnEventGridLeftDClick( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProperty = event.GetProperty();
    if( pgProperty && !pgProperty->IsCategory() )
    {
        wxString gdEventType = pgProperty->GetLabel();
        wxString name( gdEventType );

        name.MakeLower().Replace("wxevt_", "");

        wxString          handlerName = "On";
        wxStringTokenizer tokenizer( name, "_" );
        while( tokenizer.HasMoreTokens() )
        {
            handlerName.Append( tokenizer.GetNextToken().MakeCapitalized() );
        }

        RTTI::Object object      = m_handler->GetSelectedObject();
        wxPGProperty *pgCategory = pgProperty->GetParent();

        if( object && pgCategory )
        {
            RTTI::EventProperty gdEvent =
                object->GetEvent( pgCategory->GetLabel() );

            if( gdEvent )
            {
                gdEvent->SetHandlerName( gdEventType, handlerName );

                RTTI::EventPropertyEvent
                    event( wxGD_EVT_EVENT_CHANGED, GetId(), gdEvent );

                m_handler->SendEvent( event );
            }

            pgProperty->SetValueFromString( handlerName );
        }
    }
}

void wxGD::Property::Book::OnLinkClicked( wxHtmlLinkEvent &event )
{
    wxLaunchDefaultBrowser( event.GetLinkInfo().GetHref() );
}

void wxGD::Property::Book::OnObjectSelected( RTTI::ObjectEvent &event )
{
    RTTI::Object object = event.GetObject();

    LoadProperties( object );
    LoadEvents( object );
}

void wxGD::Property::Book::LoadEvents( RTTI::Object object )
{
    m_pgEvents->Clear();
    m_egHtml->SetPage( wxEmptyString );

    if( !object )
        return;

    RTTI::EventProperties events = object->GetEvents();

    for( RTTI::EventProperties::const_iterator it = events.begin();
                                                it != events.end(); ++it )
    {
        RTTI::EventProperty gdEvent = *it;
        if( gdEvent )
        {
            wxPGProperty *pgCategory =
            m_pgEvents->Append( new wxPropertyCategory
            (
                gdEvent->GetName(), wxPG_LABEL )
            );

            for( size_t i = 0; i < gdEvent->GetTypeCount(); i++ )
            {
                wxPGProperty *pgProperty =
                pgCategory->AppendChild( new wxStringProperty
                (
                    gdEvent->GetTypeName(i), wxPG_LABEL )
                );

                pgProperty->SetValueFromString( gdEvent->GetHandlerName(i) );
            }
        }
    }
}

void wxGD::Property::Book::LoadProperties( RTTI::Object object )
{
    m_pgProps->Clear();
    m_pgHtml->SetPage( wxEmptyString );

    if( !object )
        return;

    wxPGProperty *pgCategory =
    m_pgProps->Append( new wxPropertyCategory
    (
        object->GetClassName(), wxPG_LABEL )
    );

    RTTI::Properties properties = object->GetProperties();

    for( RTTI::Properties::const_iterator it = properties.begin();
                                        it != properties.end(); ++it )
    {
        RTTI::Property  property    = *it;
        wxPGProperty    *pgProperty = AddProperty( property );

        if( pgProperty )
        {
            if( pgProperty->IsCategory() )
            {
                m_pgProps->Append( pgProperty );

                RTTI::Properties children = property->GetChildren();

                for( RTTI::Properties::const_iterator   child = children.begin();
                                                        child != children.end();
                                                        ++child )
                {
                    wxPGProperty *pgChild = AddProperty( *child );

                    if( pgChild )
                        pgProperty->AppendChild( pgChild );
                }

                RTTI::ClassInfo classInfo =
                RTTI::ClassTree::Get()->GetClassInfo( property->GetName() );

                if( classInfo && classInfo->IsTypeOf( RTTI::CLASS_TYPE_ABSTRACT ) )
                {
                    pgProperty->SetBackgroundColour( wxColour( 255, 255, 225 ) );
                }
                else if( classInfo && classInfo->IsTypeOf( RTTI::CLASS_TYPE_ITEM ) )
                {
                    pgProperty->SetBackgroundColour( wxColour( 225, 255, 255 ) );
                }
            }
            else
            {
                pgCategory->AppendChild( pgProperty );
            }
        }
    }

    wxPGCell pgCell; pgCell.SetBgCol( wxColour( 255, 255, 225 ) );
    m_pgProps->SetUnspecifiedValueAppearance( pgCell );
}

wxPGProperty *wxGD::Property::Book::AddProperty( RTTI::Property property )
{
    if( !property )
        return NULL;

    wxString            label = property->GetLabel();
    wxString            name  = property->GetName();
    RTTI::PropertyType  type  = property->GetType();

    if( type == RTTI::PROPERTY_TYPE_ARRAYSTRING )
    {
        return new wxArrayStringProperty( label, name,
                                          property->GetAsArrayString() );
    }
    else if( type == RTTI::PROPERTY_TYPE_BITMAP )
    {
        return new wxGD::Property::Bitmap( label, name, property->GetAsString() );
    }
    else if( type == RTTI::PROPERTY_TYPE_BOOL )
    {
        wxPGProperty *pgProperty = new wxBoolProperty ( label, name,
                                                        property->GetAsBool() );
        pgProperty->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );

        return pgProperty;
    }
    else if( type == RTTI::PROPERTY_TYPE_CATEGORY )
    {
        return new wxPropertyCategory( label, name );
    }
    else if( type == RTTI::PROPERTY_TYPE_COLOUR )
    {
        int      type   = property->GetAsSystemColour();
        wxColour colour = property->GetAsColour();

        wxColourPropertyValue colourValue( type, colour );

        wxGD::Property::Colour
        *pgProperty = new wxGD::Property::Colour( label, name, colourValue );

        return pgProperty;
    }/*
    else if( type == RTTI::PROPERTY_TYPE_DIMENSION )
    {
        return new wxGDDimensionProperty( label, name, property->GetAsDimension() );
    }*/
    else if( type == RTTI::PROPERTY_TYPE_FLOAT || type == RTTI::PROPERTY_TYPE_DOUBLE )
    {
        return new wxFloatProperty( label, name, property->GetAsDouble() );
    }
    else if( type == RTTI::PROPERTY_TYPE_ENUM )
    {
        wxArrayString flagNames;
        wxArrayInt    flagValues;

        for( size_t i = 0; i < property->GetInfo()->GetChildCount(); i++ )
        {
            wxString flagName = property->GetInfo()->GetChild(i)->GetName();
            int      flagVal  = wxGD::Flags::Get()->Get( flagName );

            flagNames.Add( flagName );
            flagValues.Add( flagVal );
        }

        return new wxEnumProperty ( label, name, flagNames, flagValues,
                                                property->GetAsInteger() );
    }
    else if( type == RTTI::PROPERTY_TYPE_FONT )
    {
        return new wxGD::Property::Font( label, name, property->GetAsFont() );
    }
    else if( type == RTTI::PROPERTY_TYPE_INT )
    {
        return new wxIntProperty( label, name, property->GetAsInteger() );
    }
    else if( type == RTTI::PROPERTY_TYPE_NAME   ||
             type == RTTI::PROPERTY_TYPE_STRING ||
             type == RTTI::PROPERTY_TYPE_TEXT   ) // TODO
    {
        return new wxStringProperty( label, name, property->GetAsString() );
    }
    else if( type == RTTI::PROPERTY_TYPE_SIZE )
    {
        return new wxGD::Property::Size( label, name, property->GetAsSize() );
    }
    else if( type == RTTI::PROPERTY_TYPE_POINT )
    {
        return new wxGD::Property::Point( label, name, property->GetAsPoint() );
    }
    else if( type == RTTI::PROPERTY_TYPE_URL )
    {
        return new wxFileProperty( label, name, property->GetAsURL() );
    }
    else if( type == RTTI::PROPERTY_TYPE_STYLE )
    {
        wxArrayString styleNames;
        wxArrayInt    styleValues;

        for( size_t i = 0; i < property->GetInfo()->GetChildCount(); i++ )
        {
            wxString styleName = property->GetInfo()->GetChild(i)->GetName();
            int      styleVal  = wxGD::Flags::Get()->Get( styleName );

            styleNames.Add( styleName );
            styleValues.Add( styleVal );
        }

        wxPGProperty *pgProperty =
        new wxGD::Property::Flags( label, name, styleNames, styleValues,
                                                property->GetAsStyle() );
        return pgProperty;
    }

    return NULL;
}
