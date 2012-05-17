///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propgrid.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/12/10
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/gui/propgrid/propgrid.h"
#include "core/gui/propgrid/propbmp.h"
#include "core/gui/propgrid/props.h"
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

PropBookHandler::PropBookHandler( wxWindow *owner ) : m_propBook( owner )
{
    
    wxHtmlWindow *propDesc = XRCCTRL( *owner, "PropDescHtml", wxHtmlWindow );
    wxHtmlWindow *evtDesc  = XRCCTRL( *owner, "EventDescHtml", wxHtmlWindow );

    if ( propDesc && evtDesc )
    {
        propDesc->Bind( wxEVT_COMMAND_HTML_LINK_CLICKED,
                        &PropBookHandler::OnLinkClick, this );

        evtDesc->Bind( wxEVT_COMMAND_HTML_LINK_CLICKED,
                        &PropBookHandler::OnLinkClick, this );
    }
}

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
            PropertyType propType = prop->GetType();

            if ( propType == PROPERTY_TYPE_COLOUR )
            {
                wxColourPropertyValue value =
                pgProp->GetValue().GetAny().As< wxColourPropertyValue >();

                Colour col = { value.m_type, value.m_colour };

                prop->SetValue( col );
            }
            else if ( propType == PROPERTY_TYPE_BITMAP )
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

                    for ( size_t i = 0; i < count; i++ )
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
                    evt->SetFunctionName( i, pgProp->GetValueAsString() );
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
                evt->SetFunctionName( p->GetLabel(), evtFuncName );
        
            p->SetValueFromString( evtFuncName );
        }
    }
}

void PropBookHandler::OnLinkClick( wxHtmlLinkEvent &event )
{
    wxLaunchDefaultBrowser( event.GetLinkInfo().GetHref() );
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

                p->SetValueFromString( evt->GetFunctionName( n ) );
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
    if ( !prop.get() ) return NULL;

    wxString label = prop->GetLabel();
    wxString name  = prop->GetName();

    switch ( prop->GetType() )
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

            for ( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
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

            if ( !font.IsOk() )
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

            for ( size_t i = 0; i < prop->GetInfo()->GetChildCount(); i++ )
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
