///////////////////////////////////////////////////////////////////////////////
// Name:        sdk/plugin.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "plugin.h"

#include <wx/artprov.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/xml/xml.h>

#include <wx/log.h>

//-----------------------------------------------------------------------------
//  Plugin
//-----------------------------------------------------------------------------
wxIMPLEMENT_DYNAMIC_CLASS( Plugin, wxObject );

Plugin::Plugin( const wxString &category ) : wxObject()
{
    m_category = category;
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
    if ( !CheckDir( wxGD_PLUGIN_DIR ) )
    {
        m_ctrlDir = m_editDir = "";
        return;
    }

    wxString ctrlDir = wxGD_PLUGIN_DIR + "controls" + wxFILE_SEP_PATH;
    wxString editDir = wxGD_PLUGIN_DIR + "codegens" + wxFILE_SEP_PATH;

    if ( CheckDir( ctrlDir ) ) m_ctrlDir = ctrlDir;
    if ( CheckDir( editDir ) ) m_editDir = editDir;
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

bool PluginManager::CheckDir( const wxString &path )
{
    if ( !wxDirExists( path ) )
    {
        wxString msg = _("Plugins directory") + " " + path + " " +
                       _("doesn't exists!");
        wxMessageBox( msg, "wxGUIDesigner", wxOK | wxICON_EXCLAMATION, NULL );
        return false;
    }

    return true;
}

void PluginManager::LoadPlugins( const wxString &category )
{
    wxString plugDirPath = wxGD_PLUGIN_DIR + category + wxFILE_SEP_PATH;

    wxDir pluginsDir( plugDirPath );
    if ( !pluginsDir.IsOpened() ) return;

    wxXmlDocument doc;
    if ( !doc.Load( pluginsDir.GetName() + wxFILE_SEP_PATH + category + ".xml" ) ) return;

    wxXmlNode *rootNode = doc.GetRoot();

    if ( rootNode->GetName() != "plugins" ) return;

    wxXmlNode *pluginNode = rootNode->GetChildren();
    wxString  strSize     = rootNode->GetAttribute("bmpsize");
    int       size;
    wxSize    itemBmpSize = wxSize( 16, 16 );
    wxSize    bmpSize     = wxSize( 16, 16 );
    wxBitmap  bmp         = wxNullBitmap;

    if ( !strSize.empty() )
    {
        size    = wxAtoi( strSize );
        bmpSize = wxSize( size, size );
    }

    strSize = rootNode->GetAttribute("itembmpsize");
    if ( !strSize.empty() )
    {
        size        = wxAtoi( strSize );
        itemBmpSize = wxSize( size, size );
    }

    while ( pluginNode )
    {
        wxString nodeName = pluginNode->GetName();
        if ( nodeName == "plugin" )
        {
            // Plugin label (e.g.'Top level') and directory (e.g.'toplevel')
            wxString pluginLabel = pluginNode->GetAttribute("label");
            wxString plugDirName = pluginNode->GetAttribute("name");

            if ( plugDirName.empty() && !pluginLabel.empty() )
            {
                plugDirName = pluginLabel.Lower();
                plugDirName.Replace(" ", "");
            }
            else if ( pluginLabel.empty() && !plugDirName.empty() )
            {
                pluginLabel = plugDirName.Capitalize();
            }

            // Plugin directory (e.g. 'path/to/plugins/toplevel')
            wxString pluginPath    = plugDirPath + plugDirName;

            // Icons directory (e.g. 'path/to/plugins/toplevel/icons')
            wxString plugIconsPath = pluginPath + wxFILE_SEP_PATH + "icons";

            // Plugin bitmap path
            // (e.g. 'path/to/plugins/toplevel/icons/toplevel.png')
            wxString plugBmpPath   = plugIconsPath + wxFILE_SEP_PATH +
                                        plugDirName + ".png";

            // Be sure all required directories exists
            if ( wxDirExists( plugIconsPath ) )
            {
                Plugin    *plugin   = new Plugin( category );
                wxXmlNode *itemNode = pluginNode->GetChildren();

                // A default image if some bitmap is missing
                wxBitmap pluginBmp = wxArtProvider::GetBitmap( wxART_MISSING_IMAGE,
                                                                wxART_OTHER,
                                                                bmpSize );
                if ( wxFileExists( plugBmpPath ) )
                {
                    bmp = wxBitmap( plugBmpPath, wxBITMAP_TYPE_PNG );
                    if ( bmp.IsOk() )
                        pluginBmp = bmp;
                }

                plugin->SetLabel( pluginLabel );
                plugin->SetName( plugDirName );
                plugin->SetBitmap( pluginBmp );

                while ( itemNode )
                {
                    if ( itemNode->GetName() == "item" )
                    {
                        wxString itemName  = itemNode->GetAttribute("name");
                        wxString itemLabel = itemNode->GetAttribute("label");

                        if ( itemName.empty() )
                        {
                            itemNode = itemNode->GetNext();
                            continue;
                        }

                        if ( itemLabel.empty() )
                        {
							itemLabel = itemName.Lower();
							itemLabel.Replace("wx", "");
						}

                        PluginItem *item = new PluginItem();

                        // Plugin item bitmap path
                        // (e.g. 'path/to/plugins/toplevel/icons/frame.png')
                        plugBmpPath = plugIconsPath + wxFILE_SEP_PATH + itemLabel + ".png";
                        pluginBmp   = wxArtProvider::GetBitmap( wxART_MISSING_IMAGE,
                                                                wxART_OTHER,
                                                                itemBmpSize );
                        if ( wxFileExists( plugBmpPath ) )
                        {
                            bmp = wxBitmap( plugBmpPath, wxBITMAP_TYPE_PNG );
                            if ( bmp.IsOk() )
                                pluginBmp = bmp;
                        }

                        item->SetBitmap( pluginBmp );
                        item->SetName( itemName );

                        plugin->AddItem( item );
                    }
                    else if ( itemNode->GetName() == "separator" )
                    {
                        PluginItem *item = new PluginItem();
                        item->SetName("<separator>");
                        plugin->AddItem( item );
                    }

                    itemNode = itemNode->GetNext();
                }

                m_plugins.push_back( plugin );

                PluginEvent *event = new PluginEvent( wxEVT_PLUGIN_LOADED, wxID_ANY );
                event->SetEventObject( plugin );
                SendEvent( event );
            }

            pluginNode = pluginNode->GetNext();
        }
    }
}

void PluginManager::SendEvent( PluginEvent *event )
{
    for ( wxVector< wxEvtHandler * >::iterator it = m_handlers.begin();
            it != m_handlers.end(); ++it )
    {
        wxEvtHandler *handler = (*it);
        wxQueueEvent( handler, event->Clone() );
    }
}
