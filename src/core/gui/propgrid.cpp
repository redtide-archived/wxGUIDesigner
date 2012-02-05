///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propgrid.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/12/10
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/gui/propgrid.h"
#include "core/gui/properties.h"
#include "core/gui/manager.h"
#include "core/object/database.h"
#include "core/object/flags.h"
#include "core/object/tree.h"
#include "core/utils.h"
#include "core/defs.h"

#include <wx/html/htmlwin.h>
#include <wx/propgrid/propgrid.h>
#include <wx/splitter.h>
#include <wx/tokenzr.h>
#include <wx/xrc/xmlres.h>

void PropBookHandler::OnObjectCreated( Object object )
{
    // Do nothing.
}

void PropBookHandler::OnObjectDeleted( Object object )
{
    // Do nothing.
}

void PropBookHandler::OnObjectExpanded( Object object )
{
    // Do nothing.
}

void PropBookHandler::OnObjectSelected( Object object )
{
    LoadProperties( object );
    LoadEvents( object );
}

void PropBookHandler::OnPGChanged( wxPropertyGridEvent &event )
{
    wxPGProperty *pgProp  = event.GetProperty();
    if ( pgProp )
    {
        wxString propName = pgProp->GetName();
        Property prop =
        ObjectTree::Get()->GetSelectObject()->GetProperty( pgProp->GetName() );

        if ( prop.get() )
        {
            if ( prop->GetType() == PROPERTY_TYPE_COLOUR )
            {
                wxColourPropertyValue value = pgProp->GetValue().GetAny().
                                                As< wxColourPropertyValue >();

                Colour col = { value.m_type, value.m_colour };

                prop->SetValue( col );
            }
            else
            {
                prop->SetValue( pgProp->GetValue() );
            }
        }
    }
    event.Skip();
}

void PropBookHandler::OnEGChanged( wxPropertyGridEvent &event )
{
    // pgProp represents an event type
    wxPGProperty *pgProp  = event.GetProperty();
    if ( pgProp && pgProp->GetParent() )
    {
        // Get the event class name from the pgProp parent
        wxPGProperty *pgCat = pgProp->GetParent();
        if ( !pgCat )
        {
            event.Skip(); return; // Just to be sure, shouldn't happen
        }

        Event evt = // Get the Event
        ObjectTree::Get()->GetSelectObject()->GetEvent( pgCat->GetName() );

        if ( evt.get() )
        {
            // Iterate to find the right event type name
            for ( size_t i = 0; i < evt->GetTypeCount(); i++ )
            {
                wxString evtTypeName = pgProp->GetLabel();
                if ( evt->GetTypeName( i ) == evtTypeName )
                {
                    evt->SetHandlerName( i, pgProp->GetValueAsString() );
                    break;
                }
            }
        }
    }
    event.Skip();
}

void PropBookHandler::OnPGSelected( wxPropertyGridEvent &event )
{
    wxWindow     *wndDesc = wxFindWindowById( XRCID("PropDescHtml") );
    wxHtmlWindow *htmDesc = wxDynamicCast( wndDesc, wxHtmlWindow );
    wxPGProperty *pgProp  = event.GetProperty();

    if ( htmDesc && pgProp )
    {
        wxString propName    = pgProp->GetBaseName();
        wxString description = "<h5>" + pgProp->GetLabel() + "</h5>";
        Object   object      = ObjectTree::Get()->GetSelectObject();

        if ( pgProp->IsCategory() )
        {
            // Can't get a baseclass description from the object
            ClassInfo info = ClassInfoDB::Get()->GetClassInfo( propName );

            if ( info.get() )
                description += "<p>" + info->GetDescription() + "</p>";
        }
        else if ( pgProp->IsSubProperty() )
        {
            wxString parentName = pgProp->GetParent()->GetName();
            Property parent     = object->GetProperty( parentName );
            PropertyInfo info   = parent->GetInfo();

            for ( size_t i = 0; i < info->GetChildCount(); i++ )
            {
                PropertyInfo child = info->GetChild( i );
                if ( child->GetName() == propName )
                {
                    description += "<p>" + child->GetDescription() + "</p>";
                    break;
                }
            }
        }
        else
        {
            Property prop = object->GetProperty( propName );

            if ( prop )
                description += "<p>" + prop->GetDescription() + "</p>";
        }

        htmDesc->SetPage( description );
    }
    event.Skip();
}

void PropBookHandler::OnEGSelected( wxPropertyGridEvent &event )
{
    wxWindow     *wndDesc   = wxFindWindowById( XRCID("EventDescHtml") );
    wxHtmlWindow *htmDesc   = wxDynamicCast( wndDesc, wxHtmlWindow );
    wxPGProperty *pgPropEvt = event.GetProperty();

    if ( htmDesc && pgPropEvt )
    {
        wxPGProperty *pgPropEvtType = NULL;
        if ( !pgPropEvt->IsCategory() )
        {
            pgPropEvtType = pgPropEvt;
            pgPropEvt     = pgPropEvtType->GetParent();

            if ( !pgPropEvt )
                return;
        }
        wxString evtName = pgPropEvt->GetLabel();
        Event evt = ObjectTree::Get()->GetSelectObject()->GetEvent( evtName );
        wxString desc = "<h5>" + evtName + "</h5>";

        if ( pgPropEvtType )
        {
            wxString evtTypeName = pgPropEvtType->GetLabel();
            for ( size_t i = 0; i < evt->GetTypeCount(); i++ )
            {
                if ( evt->GetTypeName( i ) == evtTypeName )
                {
                    desc = "<h5>" + evtTypeName + "</h5>";
                    desc += "<p>" + evt->GetTypeDescription( i ) + "</p>";
                    break;
                }
            }
        }
        else
        {
            if ( !evt->GetDescription().empty() )
                desc += "<p>" + evt->GetDescription() + "</p>";
        }

        htmDesc->SetPage( desc );
    }
    event.Skip();
}

void PropBookHandler::OnEGDblClick( wxPropertyGridEvent &event )
{
    wxPGProperty *p = event.GetProperty();
    if ( p && !p->IsCategory() )
    {
        wxString evtTypeName( p->GetLabel() );

        evtTypeName.MakeLower().Replace("wxevt_", "");

        wxString          evtFuncName = "On";
        wxStringTokenizer tokenizer( evtTypeName, "_" );
        while ( tokenizer.HasMoreTokens() )
        {
            evtFuncName.Append( tokenizer.GetNextToken().MakeCapitalized() );
        }

        Object obj = ObjectTree::Get()->GetSelectObject();
        wxPGProperty *evtCat = p->GetParent();

        if ( obj.get() && evtCat )
        {
            Event evt = obj->GetEvent( evtCat->GetLabel() );
            if ( evt.get() )
                evt->SetHandlerName( p->GetLabel(), evtFuncName );
        
            p->SetValueFromString( evtFuncName );
        }
    }
}

void PropBookHandler::OnSize( wxSizeEvent &event )
{
    wxWindow *propWnd = wxFindWindowById( XRCID("PropGridSplitter") );
    wxWindow *evtWnd = wxFindWindowById( XRCID("EventGridSplitter") );

    wxSplitterWindow *propSplit = wxDynamicCast( propWnd, wxSplitterWindow );
    wxSplitterWindow *evtSplit  = wxDynamicCast( evtWnd, wxSplitterWindow );
    int               height    = event.GetSize().GetHeight();

    if ( propSplit && evtSplit )
    {
        propSplit->SetSashPosition( height * 0.7 );
        evtSplit->SetSashPosition( height * 0.7 );
    }
    event.Skip();
}

void PropBookHandler::LoadEvents( Object object )
{
    wxPropertyGrid *eg = GUIManager::Get()->GetEventsGrid();
    if ( !eg )
        return;

    eg->Clear();

    for ( size_t i = 0; i < object->GetEventCount(); i++ )
    {
        Event evt = object->GetEvent( i );
        if ( evt.get() )
        {
            wxPGProperty *cat = eg->Append( new wxPropertyCategory
                                        ( evt->GetName(), wxPG_LABEL ) );

            for ( size_t n = 0; n < evt->GetTypeCount(); n++ )
            {
                wxPGProperty *p = cat->AppendChild( new wxStringProperty
                                        ( evt->GetTypeName( n ), wxPG_LABEL ) );

                p->SetValueFromString( evt->GetHandlerName( n ) );
            }
        }
    }
}

void PropBookHandler::LoadProperties( Object object )
{
    wxPropertyGrid *pg = GUIManager::Get()->GetPropertiesGrid();
    if ( !pg )
        return;

    pg->Clear();

    wxPGProperty *mainCat = pg->Append( new wxPropertyCategory
                                    ( object->GetClassName(), wxPG_LABEL ) );

    for ( size_t i = 0; i < object->GetPropertyCount(); i++ )
    {
        Property     prop    = object->GetProperty( i );
        wxPGProperty *pgProp = AddProperty( prop );

        if ( pgProp )
        {
            if ( pgProp->IsCategory() )
            {
                pg->Append( pgProp );

                for ( size_t n = 0; n < prop->GetChildCount(); n++ )
                {
                    Property        child = prop->GetChild( n );
                    wxPGProperty *pgChild = AddProperty( child );

                    if ( pgChild )
                        pgProp->AppendChild( pgChild );
                }
                ClassInfo info =
                    ClassInfoDB::Get()->GetClassInfo( prop->GetName() );

                if ( info.get() && info->IsTypeOf( CLASS_TYPE_ABSTRACT ) )
                {
                    pgProp->SetBackgroundColour( wxColour( 255, 255, 225 ) );
                }
                else if ( info.get() && info->IsTypeOf( CLASS_TYPE_ITEM ) )
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
    pg->SetUnspecifiedValueAppearance( cell );
}

wxPGProperty *PropBookHandler::AddProperty( Property prop )
{
    if ( !prop.get() )
        return NULL;

    switch ( prop->GetType() )
    {
        case PROPERTY_TYPE_ARRAYSTRING:
        {
            return new wxArrayStringProperty( prop->GetLabel(), prop->GetName(),
                                              prop->GetAsArrayString() );
        }/*
        case PROPERTY_TYPE_BITMAP:
        {
            return new wxBitmapProperty( prop->GetLabel(), prop->GetName(),
                                         prop->GetAsBitmap() );
        }*/
        case PROPERTY_TYPE_BOOL:
        {
            wxPGProperty *pgProp =
                    new wxBoolProperty( prop->GetLabel(), prop->GetName(),
                                        prop->GetAsBool() );

            pgProp->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );

            return pgProp;
        }
        case PROPERTY_TYPE_CATEGORY:
        {
            return new wxPropertyCategory( prop->GetLabel(), prop->GetName() );
        }
        case PROPERTY_TYPE_COLOUR:
        {
            wxColourPropertyValue val( prop->GetAsColour().type,
                                       prop->GetAsColour().colour );
            wxPGProperty *pgProp =
                    new wxSystemColourProperty( prop->GetLabel(),
                                                prop->GetName(), val );
            if // No system or custom colour
            ( ( val.m_colour == wxNullColour ) && ( val.m_type == 0 ) )
            {
                pgProp->SetValueToUnspecified();
            }
            return pgProp;
        }/*
        case PROPERTY_TYPE_DIMENSION:
        {
            return new wxDimensionProperty( prop->GetLabel(), prop->GetName(),
                                            prop->GetAsDimension() );
        }*/
        case PROPERTY_TYPE_FLOAT:
        case PROPERTY_TYPE_DOUBLE:
        {
            return new wxFloatProperty( prop->GetLabel(), prop->GetName(),
                                        prop->GetAsDouble() );
        }
        case PROPERTY_TYPE_ENUM:
        {
            wxArrayString flagNames;
            wxArrayInt    flagValues;

            for ( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
            {
                wxString name = prop->GetInfo()->GetChild( i )->GetName();
                int      flag = wxFlagsManager::Get()->GetFlag( name );

                flagNames.Add( name );
                flagValues.Add( flag );
            }
            return new wxEnumProperty( prop->GetLabel(), prop->GetName(),
                                    flagNames, flagValues, prop->GetAsInt() );
        }
        case PROPERTY_TYPE_FONT:
        {
            wxFont font = prop->GetAsFont();
            wxPGProperty *pgProp =
                new wxFontProperty( prop->GetLabel(), prop->GetName(), font );

            if ( !font.IsOk() )
            {
                pgProp->SetValueToUnspecified();
            }
            return pgProp;
        }
        case PROPERTY_TYPE_INT:
        {
            return new wxIntProperty( prop->GetLabel(), prop->GetName(),
                                      prop->GetAsInt() );
        }
        case PROPERTY_TYPE_NAME:
        case PROPERTY_TYPE_STRING:
        case PROPERTY_TYPE_TEXT:
        {
            return new wxStringProperty( prop->GetLabel(), prop->GetName(),
                                         prop->GetAsString() );
        }
        case PROPERTY_TYPE_POINT:
        {
            return new wxSizeProperty( prop->GetLabel(), prop->GetName(),
                                       prop->GetAsSize() );
        }
        case PROPERTY_TYPE_SIZE:
        {
            return new wxPointProperty( prop->GetLabel(), prop->GetName(),
                                        prop->GetAsPoint() );
        }
        case PROPERTY_TYPE_URL:
        {
            return new wxFileProperty( prop->GetLabel(), prop->GetName(),
                                       prop->GetAsURL() );
        }
        case PROPERTY_TYPE_FLAG:
        case PROPERTY_TYPE_STYLE:
        {
            wxArrayString styleNames;
            wxArrayInt    styleValues;

            for ( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
            {
                wxString name = prop->GetInfo()->GetChild( i )->GetName();
                int     style = wxFlagsManager::Get()->GetFlag( name );

                styleNames.Add( name );
                styleValues.Add( style );
            }

            wxPGProperty *pgProp =
                new wxFlagsProperty( prop->GetLabel(), prop->GetName(),
                                    styleNames, styleValues, prop->GetAsInt() );

            pgProp->SetAttribute( wxPG_BOOL_USE_CHECKBOX, true );
            return pgProp;
        }
        default:
            return NULL;
    }

    return NULL;
}
