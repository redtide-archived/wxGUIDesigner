/////////////////////////////////////////////////////////////////////////////
// Name:        xh_propgrid.cpp
// Purpose:     XRC resource for wxPropertyGrid
// Author:      Jaakko Salli
// Modified by:
// Created:     May-16-2007
// Revision:    $Hash$
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if wxUSE_XRC && wxUSE_PROPGRID

#include "handlers/xh_propgrid.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
#endif

#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/propgridpagestate.h"
#include "wx/propgrid/manager.h"

wxIMPLEMENT_DYNAMIC_CLASS(wxPropertyGridXmlHandler, wxXmlResourceHandler)

wxPropertyGridXmlHandler::wxPropertyGridXmlHandler() : wxXmlResourceHandler()
{
    m_manager = NULL;
    m_populator = NULL;

    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxPG_DEFAULT_STYLE);
    XRC_ADD_STYLE(wxPG_AUTO_SORT);
    XRC_ADD_STYLE(wxPG_HIDE_CATEGORIES);
    XRC_ADD_STYLE(wxPG_BOLD_MODIFIED);
    XRC_ADD_STYLE(wxPG_SPLITTER_AUTO_CENTER);
    XRC_ADD_STYLE(wxPG_TOOLTIPS);
    XRC_ADD_STYLE(wxPG_HIDE_MARGIN);
    XRC_ADD_STYLE(wxPG_STATIC_SPLITTER);
    XRC_ADD_STYLE(wxPG_LIMITED_EDITING);
    XRC_ADD_STYLE(wxPG_TOOLBAR);
    XRC_ADD_STYLE(wxPG_DESCRIPTION);
    XRC_ADD_STYLE(wxPG_EX_INIT_NOCAT);
    XRC_ADD_STYLE(wxPG_EX_HELP_AS_TOOLTIPS);
    XRC_ADD_STYLE(wxPG_EX_AUTO_UNSPECIFIED_VALUES);
    XRC_ADD_STYLE(wxPG_EX_WRITEONLY_BUILTIN_ATTRIBUTES);
    XRC_ADD_STYLE(wxPG_EX_NO_FLAT_TOOLBAR);
    XRC_ADD_STYLE(wxPG_EX_MODE_BUTTONS);

    XRC_ADD_STYLE(wxPG_THEME_BORDER);
    XRC_ADD_STYLE(wxPG_NO_INTERNAL_BORDER);

    AddWindowStyles();
}

class WXDLLIMPEXP_PROPGRID wxPropertyGridXrcPopulator : public wxPropertyGridPopulator
{
public:
    wxPropertyGridXrcPopulator( wxPropertyGridXmlHandler* handler )
        : wxPropertyGridPopulator()
    {
        m_xrcHandler = handler;
        m_prevPopulator = m_xrcHandler->m_populator;
    }

    virtual ~wxPropertyGridXrcPopulator()
    {
        m_xrcHandler->m_populator = m_prevPopulator;
    }

    virtual void DoScanForChildren()
    {
        m_xrcHandler->CreateChildrenPrivately(m_pg, NULL);
    }

protected:
    wxPropertyGridXmlHandler*   m_xrcHandler;
    wxPropertyGridPopulator*    m_prevPopulator;
};


void wxPropertyGridXmlHandler::InitPopulator()
{
    wxPropertyGridXrcPopulator* populator
        = new wxPropertyGridXrcPopulator(this);
    m_populator = populator;
}

void wxPropertyGridXmlHandler::PopulatePage( wxPropertyGridPageState* state )
{
    wxString sColumns(wxS("columns"));
    if ( HasParam(sColumns) )
        state->SetColumnCount( GetLong(sColumns) );

    m_populator->SetState( state );
    m_populator->AddChildren( state->DoGetRoot() );
}

void wxPropertyGridXmlHandler::DonePopulator()
{
    delete m_populator;
}

void wxPropertyGridXmlHandler::HandlePropertyGridParams()
{
    wxString sVW(wxS("virtualwidth"));
    if ( HasParam(sVW) )
    {
        int vw = GetLong(sVW);
        m_pg->SetVirtualWidth(vw);
    }
}

wxObject *wxPropertyGridXmlHandler::DoCreateResource()
{
    const wxXmlNode* node = m_node;
    wxString nodeName = node->GetName();
    wxString emptyString;

    if ( nodeName == wxS("property") )
    {
        wxString clas = node->GetAttribute(wxS("class"), emptyString);

        wxString label;
        wxString sLabel(wxS("label"));
        if ( HasParam(sLabel) )
            label = GetText(sLabel);

        wxString name;
        wxString sName(wxS("name"));
        if ( HasParam(sName) )
            name = GetText(sName);
        else
            name = label;

        wxString sValue(wxS("value"));
        wxString value;
        wxString* pValue = NULL;
        if ( HasParam(sValue) )
        {
            value = GetText(sValue);
            pValue = &value;
        }

        wxXmlNode* choicesNode = GetParamNode(wxS("choices"));
        wxPGChoices choices;
        if ( choicesNode )
        {
            choices = m_populator->ParseChoices(choicesNode->GetNodeContent(),
                                                choicesNode->GetAttribute(wxS("id"), emptyString) );
        }

        wxPGProperty* property = m_populator->Add( clas, label, name, pValue, &choices );

        if ( !property )
            return NULL;

        wxString sFlags(wxS("flags"));
        wxString flags;
        if ( HasParam(sFlags) )
            property->SetFlagsFromString( GetText(sFlags) );

        wxString sTip(wxS("tip"));
        if ( HasParam(sTip) )
            property->SetHelpString(GetText(sTip));

        if ( property->GetChildCount() )
        {
            wxPGProperty* pwc = property;

            // FIXME
            wxString sExpanded(wxS("expanded"));
            if ( HasParam(sExpanded) )
                pwc->SetExpanded(GetBool(sExpanded));
        }

        // Need to call AddChildren even for non-parent properties for attributes and such
        m_populator->AddChildren(property);

        wxXmlNode *parentNode = node->GetParent();

        if (m_class == wxS("wxPropertyGridPage")    ||
            parentNode->GetName() == wxS("property") )
        {
            wxASSERT( m_manager );

            wxString sDesc( wxS("description") );
            if ( HasParam( sDesc ) )
            {
                wxString text = GetText( sDesc );
                property->SetHelpString( text );
            }
        }
    }
    else if ( nodeName == wxS("attribute") )
    {
        // attribute
        wxString s1 = node->GetAttribute(wxS("name"), emptyString);
        if ( s1.length() )
        {
            m_populator->AddAttribute( s1, node->GetAttribute(wxS("type"), emptyString),
                                       node->GetNodeContent() );
        }
    }
    else if( m_class == wxS("wxPropertyGrid"))
    {
        XRC_MAKE_INSTANCE(control, wxPropertyGrid)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetPosition(), GetSize(),
                        GetStyle(),
                        GetName());

        m_pg = control;
        HandlePropertyGridParams();

        InitPopulator();
        PopulatePage(control->GetState());
        DonePopulator();

        SetupWindow(control);

        return control;
    }
    else if ( nodeName == wxS("choices") )
    {
        // Add choices list outside of a property
        m_populator->ParseChoices( node->GetNodeContent(),
                                   node->GetAttribute(wxS("id"), emptyString));
    }
    else if ( nodeName == wxS("splitterpos") )
    {
        // splitterpos
        wxASSERT(m_populator);
        wxString sIndex = node->GetAttribute(wxS("index"), emptyString);

        long index;
        if ( !sIndex.ToLong(&index, 10) )
            index = 0;

        wxString s = node->GetNodeContent();
        long pos;
        if ( wxPropertyGridPopulator::ToLongPCT(s, &pos, m_pg->GetClientSize().x) )
            m_populator->GetState()->DoSetSplitterPosition( pos, index, false );
    }
    else if ( m_class == wxS("wxPropertyGridPage") )
    {
        wxPropertyGridManager *manager = wxDynamicCast( m_parentAsWindow, wxPropertyGridManager );
            
        if ( manager )
        {
            wxPropertyGridManager* oldManager = m_manager;
            m_manager = manager;

            wxString label;
            wxString sLabel(wxS("label"));
            if ( HasParam(sLabel) )
                label = GetText(sLabel);
            else
                label = wxString::Format(_("Page %i"),(int)(m_manager->GetPageCount()+1));

            wxPropertyGridPage *page = m_manager->AddPage(label);
            PopulatePage(page);

            m_manager = oldManager;
            return page;
        }
        else
        {
            ReportError
            (
                node->GetParent(),
                "wxPropertyGridPage parent must be a wxPropertyGridManager"
            );
        }
    }
    else if( m_class == wxS("wxPropertyGridManager"))
    {
        XRC_MAKE_INSTANCE(control, wxPropertyGridManager)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetPosition(), GetSize(),
                        GetStyle(),
                        GetName());

        wxPropertyGridManager* oldManager = m_manager;
        m_manager = control;
        m_pg = control->GetGrid();
        HandlePropertyGridParams();

        InitPopulator();
        CreateChildrenPrivately(control, NULL);
        DonePopulator();

        m_manager = oldManager;
        SetupWindow(control);

        return control;
    }
    else
    {
        wxASSERT( false );
    }

    return NULL;
}

bool wxPropertyGridXmlHandler::CanHandle(wxXmlNode *node)
{
    wxString name = node->GetName();

    bool canhandle =
        (
            (
                m_populator && ( name == wxS("property")  ||
                                 name == wxS("attribute") ||
                                 name == wxS("choices")   ||
                                 name == wxS("splitterpos") )
            )
            || (!m_populator && IsOfClass(node, wxS("wxPropertyGrid")))
            || (!m_populator && IsOfClass(node, wxS("wxPropertyGridManager")))
            || ( m_manager   && IsOfClass(node, wxS("wxPropertyGridPage")))
        );
//  wxLogDebug("%s: %s", node->GetAttribute("name"), canhandle ? "true" : "false");
    return canhandle;
}

#endif // wxUSE_XRC && wxUSE_PROPGRID
