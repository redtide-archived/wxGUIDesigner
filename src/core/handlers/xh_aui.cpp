///////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_aui.h
// Purpose:     Implementation of wxAUI XRC handler.
// Author:      Andrea Zanellato
// Created:     2011-09-18
// RCS-ID:      $Id: $
// Copyright:   (c) 2011 wxWidgets Team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_AUI

//#include "wx/xrc/xh_aui.h"
#include "handlers/xh_aui.h"
#include "wx/aui/framemanager.h"

wxIMPLEMENT_DYNAMIC_CLASS( wxAuiXmlHandler, wxXmlResourceHandler )

wxAuiXmlHandler::wxAuiXmlHandler()
                : wxXmlResourceHandler(),
                  m_manager(NULL),
                  m_window(NULL),
                  m_isInside(false)
{
}

wxAuiManager *wxAuiXmlHandler::GetAuiManager( wxWindow *managed )
{
    for ( unsigned int i = 0; i < m_managers.GetCount(); i++ )
    {
        wxAuiManager *mgr = m_managers.Item( i );
        if ( mgr && ( mgr->GetManagedWindow() == managed ) )
            return mgr;
    }
    return NULL;
}

void wxAuiXmlHandler::OnManagedWindowClose( wxWindowDestroyEvent &event )
{
    wxWindow *window = wxDynamicCast( event.GetEventObject(), wxWindow );
    for ( unsigned int i = 0; i < m_managers.GetCount(); i++ )
    {
        wxAuiManager *mgr = m_managers.Item( i );
        if ( mgr && ( mgr->GetManagedWindow() == window ) )
        {
            mgr->UnInit();
            m_managers.Remove( mgr );
            break;
        }
    }
    event.Skip();
}

wxObject *wxAuiXmlHandler::DoCreateResource()
{
    if ( m_class == "wxAuiManager" )
    {
        wxAuiManager *manager = NULL;

        wxXmlNode *windowNode = m_node->GetParent();
        while ( windowNode )
        {
            if ( ( windowNode->GetName() == "object" || windowNode->GetName() == "object_ref") )
            {
                wxString classname = "class";
                wxString name      = "name";

                if ( windowNode->HasAttribute( classname ) &&
                     windowNode->HasAttribute( name ) )
                {
                    classname = windowNode->GetAttribute( classname );
                    name      = windowNode->GetAttribute( name );

                    if ( classname != "wxAuiPaneInfo" &&
                         classname != "wxStatusBar"   &&
                         classname != "wxToolBar"     &&
                         classname != "wxMenuBar" )
                    {
                        // Using XRCID here causes undefined references when linking
                        wxWindow *window = wxWindow::FindWindowByName( name );

                        if ( window )
                        {
                            bool          old_ins = m_isInside;
                            wxAuiManager *old_mgr = m_manager;
                            wxWindow     *old_win = m_window;

                            manager    = new wxAuiManager();
                            m_window   = window;
                            m_manager  = manager;
                            m_isInside = true;

                            m_managers.Add( manager );
                            manager->SetManagedWindow( window );
                            window->Bind( wxEVT_DESTROY, &wxAuiXmlHandler::OnManagedWindowClose, this );

                            CreateChildren( manager );

                            if ( HasParam("perspective") )
                                manager->LoadPerspective( GetParamValue("perspective") );

                            m_window   = old_win;
                            m_manager  = old_mgr;
                            m_isInside = old_ins;
                        }
                    }
                }
            }
            windowNode = windowNode->GetNext();
        }

        return manager;
    }
    else if ( m_class == "wxAuiPaneInfo" )
    {
        wxXmlNode *node   = GetParamNode("object");
        wxWindow  *window = NULL;

        if ( !node )
            node = GetParamNode("object_ref");

        if ( node )
        {
            bool old_ins = m_isInside;
            m_isInside = false;

            wxObject *object = CreateResFromNode( node, m_window, NULL );

            m_isInside = old_ins;
            window = wxDynamicCast( object, wxWindow );

            if ( window == NULL && object != NULL )
            {
                ReportError( node, _("wxAuiPaneInfo child must be a window.") );
            }
        }

        if ( window )
        {
            wxAuiPaneInfo paneInfo = wxAuiPaneInfo();

            wxString name = GetName();          paneInfo.Name( name );
// Caption
            if ( HasParam("caption") )          paneInfo.Caption( GetText("caption") );
            if ( HasParam("caption_visible") )  paneInfo.CaptionVisible( GetBool("caption_visible") );
// Buttons
            if ( HasParam("close_button") )     paneInfo.CloseButton( GetBool("close_button") );
            if ( HasParam("minimize_button") )  paneInfo.MinimizeButton( GetBool("minimize_button") );
            if ( HasParam("maximize_button") )  paneInfo.MaximizeButton( GetBool("maximize_button") );
            if ( HasParam("pin_button") )       paneInfo.PinButton( GetBool("pin_button") );
            if ( HasParam("gripper") )          paneInfo.Gripper( GetBool("gripper") );
// Appearance
            if ( HasParam("pane_border") )      paneInfo.PaneBorder( GetBool("pane_border") );
// State
            if ( HasParam("dock") )             paneInfo.Dock();
            else if ( HasParam("float") )       paneInfo.Float();

// Dockable Directions
            if ( HasParam("top_dockable") )     paneInfo.TopDockable( GetBool("top_dockable") );
            if ( HasParam("bottom_dockable") )  paneInfo.BottomDockable( GetBool("bottom_dockable") );
            if ( HasParam("left_dockable") )    paneInfo.LeftDockable( GetBool("left_dockable") );
            if ( HasParam("right_dockable") )   paneInfo.RightDockable( GetBool("right_dockable") );
// Behaviours
            if ( HasParam("dock_fixed") )       paneInfo.DockFixed( GetBool("dock_fixed") );
            if ( HasParam("resizable") )        paneInfo.Resizable( GetBool("resizable") );
            if ( HasParam("movable") )          paneInfo.Movable( GetBool("movable") );
            if ( HasParam("floatable") )        paneInfo.Floatable( GetBool("floatable") );
// Sizes
            if ( HasParam("floating_size") )    paneInfo.FloatingSize( GetSize("floating_size") );
            if ( HasParam("min_size") )         paneInfo.MinSize( GetSize("min_size") );
            if ( HasParam("max_size") )         paneInfo.MaxSize( GetSize("max_size") );
            if ( HasParam("best_size") )        paneInfo.BestSize( GetSize("best_size") );
// Positions
            if ( HasParam("row") )              paneInfo.Row( GetLong("row") );
            if ( HasParam("layer") )            paneInfo.Layer( GetLong("layer") );
            if ( HasParam("default_pane") )     paneInfo.DefaultPane();
            else if( HasParam("toolbar_pane") ) paneInfo.ToolbarPane();

// Directions - CenterPane()/CentrePane != Center()/Centre()
            if ( HasParam("center_pane") ||
                 HasParam("centre_pane") )      paneInfo.CenterPane();
            if ( HasParam("direction") )        paneInfo.Direction( GetLong("direction") );
            else if ( HasParam("top") )         paneInfo.Top();
            else if ( HasParam("bottom") )      paneInfo.Bottom();
            else if ( HasParam("left") )        paneInfo.Left();
            else if ( HasParam("right") )       paneInfo.Right();
            else if ( HasParam("center") ||
                      HasParam("centre") )      paneInfo.Center();

            m_manager->AddPane( window, paneInfo );
            m_manager->Update();
        }
        else
        {
            ReportError(_("No wxWindow derived class object specified inside wxAuiPaneInfo.") );
        }
        return window;
    }
    return NULL;
}

bool wxAuiXmlHandler::CanHandle( wxXmlNode *node )
{
    return (!m_isInside && IsOfClass( node, "wxAuiManager")) ||
            (m_isInside && IsOfClass( node, "wxAuiPaneInfo"));
}

#endif // wxUSE_XRC && wxUSE_AUI
