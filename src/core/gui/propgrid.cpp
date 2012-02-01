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
#include "core/object/tree.h"
#include "core/utils.h"

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
                                        ( evt->GetClassName(), wxPG_LABEL ) );

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
            }
            else
            {
                mainCat->AppendChild( pgProp );
            }
        }
    }
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
        }
        case PROPERTY_TYPE_BOOL:
        {
            wxPGProperty *pgProp =
                    new wxBoolProperty( prop->GetLabel(), prop->GetName(),
                                        prop->GetAsString() == "1" );
            pgProp->SetAttribute( "UseCheckbox", 1 );
            return pgProp;
        }
        case PROPERTY_TYPE_CATEGORY:
        {
            return new wxPropertyCategory( prop->GetLabel(), prop->GetName() );
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
        default:
            return NULL;
    }

    return NULL;
}