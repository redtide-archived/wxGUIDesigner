///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/tree.h
// Purpose:     interface of Widget
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/12
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    @class Property

    Widget property
*/
class Property
{
public:
    Property( PropertyInfo info, const wxAny& value );
    ~Property();

    const wxString&     GetLabel()    const;
    const wxString&     GetName()     const;
    const PropertyType  GetType()     const;

    wxBitmap            GetAsBitmap() const;
    bool                GetAsBool()   const;
    wxColour            GetAsColour() const;
    float               GetAsFloat()  const;
    wxFont              GetAsFont()   const;
    int                 GetAsInt()    const;
    wxPoint             GetAsPoint()  const;
    wxSize              GetAsSize()   const;
    wxString            GetAsString() const;
    long                GetAsStyle()  const;
    wxString            GetAsText()   const;
    wxString            GetAsURL()    const;
};

/**
    @class Widget

    it keeps informations used by GUIManager to manage wxObject
    derived class instances.

    @beginEventEmissionTable
    @event{EVT_WIDGET_CREATING(func)}
        Process a @c wxEVT_WIDGET_CREATE event. See WidgetEvent.
    @event{EVT_WIDGET_CREATED(func)}
        Process a @c wxEVT_WIDGET_CREATED event. See WidgetEvent.
    @event{EVT_WIDGET_SELECTED(func)}
        Process a @c wxEVT_WIDGET_SELECTED event. See WidgetEvent.
    @event{EVT_WIDGET_DELETE(func)}
        Process a @c wxEVT_WIDGET_DELETE event. See WidgetEvent.
    @event{EVT_WIDGET_DELETED(func)}
        Process a @c wxEVT_WIDGET_DELETED event. See WidgetEvent.
    @endEventTable

    @library{core}
*/
class Widget : public IWidgetNode
{
public:
    /**
        Default constructor.
    */
    Widget();
    /**
        Virtual destructor.
    */
    virtual ~Widget();
    /**
        Sets the parent object.
    */
    void SetParent( Widget *parent );
    /**
        Sets the object to be contained.
    */
    void SetControl( wxObject *object );
    /**
        Adds a child object to the children vector.
    */
    void AddChild( Widget *child );
    /**
        Adds a Property object to the Properties vector.
    */
    void AddProperty( Property property );
    /**
        Adds a Event object to the Events vector.
    */
    void AddEvent( Event event );
    /**
        Adds a child object to the children vector.
    */
    Widget *GetParent();
    /**
        Returns the wxObject contained.
    */
    wxObject *GetControl();
    /**
        Returns all the children objects.
    */
    wxVector< Widget * > GetChildren();
};
/**
    @class WidgetTree

    Singleton class: GUI object's management.

    @library{core}
*/
class WidgetTree
{
public:
    /**
        Gets the static instance of this class.
    */
    static WidgetTree *Get();
    /**
        Free resources used by this class and destroy it.
    */
    void Free();
    /**
        Create a new control object.
    */
    void CreateObject( const wxString &className );
};
/**
    @class WidgetEvent

    @library{core}
*/
class WidgetEvent
{
public:
    /**
        Default constructor.
    */
    WidgetEvent( wxEventType type = wxEVT_NULL, int id = 0 );
    /**
        Destructor.
    */
    ~WidgetEvent();

    
};
