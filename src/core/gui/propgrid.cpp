///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propgrid.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/10
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "core/utils.h"
#include "core/gui/propgrid.h"
#include "core/gui/manager.h"

#include <wx/splitter.h>
#include <wx/xrc/xmlres.h>

PropBookHandler::PropBookHandler( wxNotebook *owner )
{
    m_propBook = owner;
}

void PropBookHandler::OnPluginLoaded( Plugin *plugin )
{

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
