///////////////////////////////////////////////////////////////////////////////
// Name:        sdk/plugin/plugin.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "plugin.h"

#include <wx/artprov.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>

#include <wx/log.h>

//-----------------------------------------------------------------------------
//  Plugin
//-----------------------------------------------------------------------------
wxIMPLEMENT_DYNAMIC_CLASS( Plugin, wxObject );

Plugin::Plugin() : wxObject()
{
    m_name = "";
    m_desc = "";
    m_bmp  = wxNullBitmap;
}

Plugin::~Plugin()
{
    m_items.erase( m_items.begin(), m_items.end() );
}
//-----------------------------------------------------------------------------
//  Plugin Event
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS( PluginEvent, wxEvent );

wxDEFINE_EVENT( wxEVT_PLUGIN_LOADED,    PluginEvent );
wxDEFINE_EVENT( wxEVT_PLUGIN_UNLOADED,  PluginEvent );

PluginEvent::PluginEvent( wxEventType type, int id )
            : wxEvent( id, type )
{
    
}

PluginEvent::PluginEvent( const PluginEvent &event )
            : wxEvent( event )
{
    
}

PluginEvent::~PluginEvent()
{
    
}

//-----------------------------------------------------------------------------
//  PluginManager
//-----------------------------------------------------------------------------

PluginManager::PluginManager()
{
    wxString dir =
#ifdef __WXMSW__
    wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') +
    wxFILE_SEP_PATH + "xrc/plugins" + wxFILE_SEP_PATH;
#else
    wxStandardPaths::Get().GetResourcesDir().BeforeLast('/') +
    wxFILE_SEP_PATH + "wxguidesigner/xrc/plugins" + wxFILE_SEP_PATH;
#endif

    if ( !wxDirExists( dir ) )
    {
        wxString msg = _("Plugins directory") + " " + dir + " " +
                       _("doesn't exists!");
        wxMessageBox( msg, "wxGUIDesigner", wxOK | wxICON_EXCLAMATION, NULL );
        m_dir = "";
    }
    else
    {
        m_dir = dir;
    }
}

PluginManager::~PluginManager()
{
}

void PluginManager::Free()
{
    m_plugins.erase( m_plugins.begin(), m_plugins.end() );
}

PluginManager *PluginManager::ms_instance = NULL;

PluginManager *PluginManager::Get()
{
    if ( !ms_instance )
        ms_instance = new PluginManager;

    return ms_instance;
}

void PluginManager::LoadPlugins()
{
    wxDir pluginsDir( m_dir );
    if ( !pluginsDir.IsOpened() ) return;

    // Plugin directory (e.g.'toplevel')
    wxString pluginName;
    bool moreDirs = pluginsDir.GetFirst(&pluginName, wxEmptyString,
                                        wxDIR_DIRS | wxDIR_HIDDEN);

    // Iterate through plugin directories and load them,
    // validating each control found
    while ( moreDirs )
    {
        // Plugin directory (e.g. 'path/to/xrc/plugins/toplevel')
        wxString nextPluginPath = m_dir + pluginName;

        // Icons directory (e.g. 'path/to/xrc/plugins/toplevel/icons')
        wxString nextPluginIconPath = nextPluginPath + wxFILE_SEP_PATH + "icons";

        // Be sure all required directories exists
        if ( wxDirExists( nextPluginIconPath ) )
        {
            wxDir pluginDir( nextPluginPath );
            if ( pluginDir.IsOpened() )
            {
                Plugin *plugin = new Plugin();

                // Plugin bitmap path
                // (e.g. 'path/to/xrc/plugins/toplevel/icons/toplevel.png')
                wxString plugBmpPath = nextPluginIconPath + wxFILE_SEP_PATH +
                                       pluginName + ".png";

                // Plugin name for the palette (e.g. 'Toplevel')
                plugin->SetName( pluginName.Capitalize() );

                // A default image if some bitmap is missing
                wxBitmap bmp = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE,
                                                        wxART_OTHER,
                                                        wxSize( 16, 16 ));
                if ( wxFileExists( plugBmpPath ) )
                {
                    wxBitmap b = wxBitmap( plugBmpPath, wxBITMAP_TYPE_PNG );
                    if ( b.IsOk() )
                        bmp = b;
                }

                plugin->SetBitmap( bmp );

                // Plugin path (e.g. 'path/to/xrc/plugins/toplevel/toplevel.xrc')
                wxXmlDocument doc;
                wxString      pluginPath = nextPluginPath + wxFILE_SEP_PATH +
                                           pluginName + ".xrc";
                if ( !doc.Load( pluginPath ) )
                {
                    moreDirs = pluginsDir.GetNext( &pluginName );
                    continue;
                }

                wxXmlNode   *pluginNode = doc.GetRoot()->GetChildren();
                PluginItem  *item       = NULL;

                while ( pluginNode )
                {
                    wxXmlNode *childNode = pluginNode->GetChildren();
                    
                    while ( childNode )
                    {
                        if ( pluginNode->GetName() == "property" )
                        {
                            if ( !item )
                            {
                                item = new PluginItem();
                                plugin->AddItem( item );
                            }

                            if ( childNode->GetName() == "label" )
                            {
                                wxXmlNode *lblNode = childNode->GetChildren();

                                if ( lblNode )
                                    item->SetName( lblNode->GetContent() );
                            }
                            else if ( childNode->GetName() == "bitmap" )
                            {
                                wxXmlNode *bmpNode = childNode->GetChildren();
                                bmp = wxArtProvider::GetBitmap
                                        (
                                            wxART_MISSING_IMAGE, wxART_OTHER,
                                            wxSize( 22, 22 )
                                        );
                                if ( bmpNode )
                                {
                                    wxString itemBmpPath = nextPluginPath +
                                                            wxFILE_SEP_PATH +
                                                            bmpNode->GetContent();

                                    if ( wxFileExists( itemBmpPath ) )
                                    {
                                        wxBitmap b = wxBitmap( itemBmpPath,
                                                                wxBITMAP_TYPE_PNG );
                                        if ( b.IsOk() )
                                            bmp = b;
                                    }
                                }

                                item->SetBitmap( bmp );
                            }
                        }
                        else if ( pluginNode->GetName() == "description" )
                        {
                            plugin->SetDescription( childNode->GetContent() );
                        }

                        childNode = childNode->GetNext();
                    }

                    item        = NULL;
                    pluginNode  = pluginNode->GetNext();
                }

                m_plugins.push_back( plugin );

                PluginEvent *event = new PluginEvent( wxEVT_PLUGIN_LOADED, wxID_ANY );
                event->SetEventObject( plugin );

                for ( wxVector< wxEvtHandler * >::iterator it = m_handlers.begin();
                        it != m_handlers.end(); ++it )
                {
                    wxEvtHandler *handler = (*it);
                    wxQueueEvent( handler, event );
                }
            }
        }

        moreDirs = pluginsDir.GetNext( &pluginName );
    }
}
