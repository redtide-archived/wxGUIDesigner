///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/plugin/plugin.h
// Purpose:     Plugin Interface
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/12
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
/**
    @class PluginItem

    Keeps informations regarding the nature of an object contained
    inside this class as part of a group of items inside the main
    Plugin class container.

    @library{plugin}
*/
class PluginItem
{
public:
    /**
        Default constructor.
    */
    PluginItem();
    /**
        Virtual destructor.
    */
    virtual ~PluginItem();
    /**
        Gets the name of the plugin.
    */
    const wxString &GetName() const;
    /**
        Gets the bitmap that represents this plugin item.
    */
    const wxBitmap &GetBitmap() const;
    /**
        Gets the size of the bitmap that represents this plugin item.
    */
    const wxSize &GetBitmapSize() const;
    /**
        Sets the name of the plugin.
    */
    void SetName( const wxString &name = wxEmptyString );
    /**
        Sets the bitmap that represents this plugin.
    */
    void SetBitmap( const wxBitmap &bmp = wxNullBitmap );
    /**
        Sets the size of the bitmap that represents this plugin.
    */
    void SetBitmapSize( const wxSize &size = wxSize( 16, 16 ) );
};

/**
    @class Plugin

    Keeps informations regarding the nature of an object category
    that groups items inside this class.

    @library{plugin}
*/
class IPlugin
{
public:
    /**
        Virtual destructor.
    */
    virtual ~IPlugin() = 0;
    /**
        Adds a PluginItem object to the collection inside this class object.
    */
    void AddItem( Item item );
    /**
        Gets the category name that describes the nature of the plugin.
    */
    const wxString &GetCategory() const;
    /**
        Gets the label that describes the nature of the plugin.
    */
    const wxString &GetLabel() const;
    /**
        Gets the plugin name.
    */
    const wxString &GetName() const;
    /**
        Gets the plugin description.
    */
    const wxString &GetDescription() const;
    /**
        Gets the bitmap that represents this plugin.
    */
    const wxBitmap &GetBitmap() const;
    /**
        Gets the size of the bitmap that represents this plugin.
    */
    const wxSize &GetBitmapSize() const;
    /**
        Sets the label that describes the nature of the plugin.
    */
    void SetLabel( const wxString &label = wxEmptyString );
    /**
        Sets the plugin name, used as identifier.
    */
    void SetName( const wxString &name = wxEmptyString );
    /**
        Sets the plugin description.
    */
    void SetDescription( const wxString &desc = wxEmptyString );
    /**
        Sets the bitmap that represents this plugin.
    */
    void SetBitmap( const wxBitmap &bmp = wxNullBitmap );
    /**
        Sets the size of the bitmap that represents this plugin.
    */
    void SetBitmapSize( const wxSize &size = wxSize( 16, 16 ) );
    /**
        Returns the item collection inside this plugin.
    */
    wxVector< PluginItem * > GetItems();
};
/**
    @class IPluginHandler

    Plugin handler interface:
    All event handlers that deals with Plugin objects must implement this
    small class.

    @library{plugin}
*/
class IPluginHandler
{
public:
    /**
        Default constructor.
    */
    IPluginHandler();
    /**
        Virtual destructor.
    */
    virtual ~IPluginHandler();
    /**
        The handler used when a plugin was loaded by the application.
    */
    virtual void OnPluginLoaded( Plugin *plugin ) = 0;
};

/**
    @class PluginManager

    Plugins management singleton class.

    @library{plugin}
*/
class PluginManager
{
public:
    /**
        Gets the static instance of this class.
    */
    static PluginManager *Get();
    /**
        Free resources used by this class and destroy it.
    */
    static void Free();
    /**
        Loads all plugins described in a XML file with the specified category.
    */
    void LoadPlugins( const wxString &category );
    /**
        Adds an event handler to the handlers managed in this class,
        which must implement the IPluginHandler interface.
    */
    void AddHandler( IPluginHandler *handler );
};
