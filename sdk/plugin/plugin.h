///////////////////////////////////////////////////////////////////////////////
// Name:        sdk/plugin/plugin.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGD_PLUGIN_H__
#define __WXGD_PLUGIN_H__

#include <wx/bitmap.h>
#include <wx/event.h>
#include <wx/vector.h>

class PluginItem
{
public:
    PluginItem() {}
    virtual ~PluginItem() {}

    const wxString& GetName()   const       { return m_name; }
    const wxBitmap& GetBitmap() const       { return m_bmp; }

    void SetName( const wxString &name )    { m_name = name; }
    void SetBitmap( const wxBitmap &bmp )   { m_bmp = bmp; }

protected:
    wxString m_name;
    wxBitmap m_bmp;
};

class Plugin : public wxObject
{
    wxDECLARE_DYNAMIC_CLASS( Plugin );

public:
    Plugin();
    virtual ~Plugin();

    void            AddItem( PluginItem *item ) { m_items.push_back( item ); }

    const wxString& GetName()           const   { return m_name; }
    const wxString& GetDescription()    const   { return m_desc; }
    const wxBitmap& GetBitmap()         const   { return m_bmp; }

    void SetName( const wxString &name )        { m_name = name; }
    void SetDescription( const wxString &desc ) { m_desc = desc; }
    void SetBitmap( const wxBitmap &bmp )       { m_bmp = bmp; }

    wxVector< PluginItem * > GetItems()         { return m_items; }

protected:
    wxString                 m_name;
    wxString                 m_desc;
    wxBitmap                 m_bmp;
    wxVector< PluginItem * > m_items;
};

class PluginEvent : public wxEvent
{
public:
    PluginEvent( wxEventType type = wxEVT_NULL, int id = 0 );

#ifndef SWIG
    PluginEvent( const PluginEvent &event );
#endif
    ~PluginEvent();

    virtual wxEvent *Clone() const { return new PluginEvent( *this ); }

private:
#ifndef SWIG
    wxDECLARE_DYNAMIC_CLASS( PluginEvent );
#endif
};

#ifndef SWIG
    wxDECLARE_EVENT( wxEVT_PLUGIN_LOADED,   PluginEvent );
    wxDECLARE_EVENT( wxEVT_PLUGIN_UNLOADED, PluginEvent );
#else
    enum
    {
        wxEVT_PLUGIN_LOADED,
        wxEVT_PLUGIN_UNLOADED
    };
#endif

#ifndef SWIG
typedef void ( wxEvtHandler::*PluginEventFunction )( PluginEvent & );

#define PluginEventHandler( func ) wxEVENT_HANDLER_CAST( PluginEventFunction, func )

#define EVT_PLUGIN_LOADED( id, fn )     wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_PLUGIN_LOADED, id, wxID_ANY, \
                                            PluginEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#define EVT_PLUGIN_UNLOADED( id, fn )    wxDECLARE_EVENT_TABLE_ENTRY \
                                        ( \
                                            wxEVT_OBJECT_CREATED, id, wxID_ANY, \
                                            PluginEventHandler( fn ), \
                                            (wxObject *) NULL \
                                        )
#endif // SWIG

class PluginManager
{
public:
    static PluginManager *Get();
    void Free();

    const wxString &GetPluginDirectory() { return m_dir; }

    void LoadPlugins();
    void AddHandler( wxEvtHandler *handler ) { m_handlers.push_back( handler ); }

protected:
    PluginManager();
    virtual ~PluginManager();

    wxString                    m_dir;
    wxVector< Plugin * >        m_plugins;
    wxVector< wxEvtHandler * >  m_handlers;
    static PluginManager        *ms_instance;
};

#endif //__WXGD_PLUGIN_H__
