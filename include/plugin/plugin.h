///////////////////////////////////////////////////////////////////////////////
// Name:        plugin/plugin.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGD_PLUGIN_H__
#define __WXGD_PLUGIN_H__

#include <wx/bitmap.h>
#include <wx/event.h>
#include <wx/vector.h>

#include "defs.h"

class DLLIMPEXP_PLUGIN PluginItem
{
public:
    PluginItem() {}
    virtual ~PluginItem() {}

    const wxString &GetName()       const { return m_name; }
    const wxBitmap &GetBitmap()     const { return m_bmp; }
    const wxSize   &GetBitmapSize() const { return m_bmpSize; }

    void SetName( const wxString &name = wxEmptyString )
    {
        m_name = name;
    }
    void SetBitmap( const wxBitmap &bmp = wxNullBitmap )
    {
        m_bmp = bmp;
    }
    void SetBitmapSize( const wxSize &size = wxSize( 16, 16 ) )
    {
        m_bmpSize = size;
    }

protected:
    wxString m_name;
    wxBitmap m_bmp;
    wxSize   m_bmpSize;
};

class DLLIMPEXP_PLUGIN Plugin : public wxObject
{
    wxDECLARE_DYNAMIC_CLASS( Plugin );

public:
    Plugin( const wxString &category = wxEmptyString );
    virtual ~Plugin();

    void AddItem( PluginItem *item ) { m_items.push_back( item ); }

    const wxString &GetCategory()    const { return m_category; }
    const wxString &GetLabel()       const { return m_label; }
    const wxString &GetName()        const { return m_name; }
    const wxString &GetDescription() const { return m_desc; }
    const wxBitmap &GetBitmap()      const { return m_bmp; }
    const wxSize   &GetBitmapSize()  const { return m_bmpSize; }

    void SetLabel( const wxString &label = wxEmptyString )
    {
        m_label = label;
    }
    void SetName( const wxString &name = wxEmptyString )
    {
        m_name = name;
    }
    void SetDescription( const wxString &desc = wxEmptyString )
    {
        m_desc = desc;
    }
    void SetBitmap( const wxBitmap &bmp = wxNullBitmap )
    {
        m_bmp = bmp;
    }
    void SetBitmapSize( const wxSize &size = wxSize( 16, 16 ) )
    {
        m_bmpSize = size;
    }

    wxVector< PluginItem * > GetItems() { return m_items; }

protected:
    wxString m_category;
    wxString m_label;
    wxString m_name;
    wxString m_desc;
    wxBitmap m_bmp;
    wxSize   m_bmpSize;

    wxVector< PluginItem * > m_items;
};

class DLLIMPEXP_PLUGIN PluginEvent : public wxEvent
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

class DLLIMPEXP_PLUGIN PluginHandler
{
public:
    PluginHandler() { }
    virtual ~PluginHandler() { }

    virtual void OnPluginLoaded( Plugin *plugin ) = 0;
};

class DLLIMPEXP_PLUGIN PluginManager
{
public:
    static PluginManager *Get();
    void Free();

    void LoadPlugins( const wxString &category );
    void AddHandler( PluginHandler *handler ) { m_handlers.push_back( handler ); }

private:
    PluginManager();
    virtual ~PluginManager();

    PluginManager( const PluginManager& );
    PluginManager& operator=( PluginManager const& );

    bool CheckDir( const wxString &path );

    wxString                    m_ctrlDir;
    wxString                    m_editDir;
    wxVector< Plugin * >        m_plugins;
    wxVector< PluginHandler * > m_handlers;
    static PluginManager       *ms_instance;
};

#endif //__WXGD_PLUGIN_H__
