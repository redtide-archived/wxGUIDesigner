///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/tree.h
// Purpose:     interface of Object
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/12
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    @class Property

    Object property
*/
class Property
{
public:
    Property( PropertyInfo info, const wxAny& value );
    ~Property();

    const wxString&     GetLabel()          const;
    const wxString&     GetName()           const;
    const PropertyType  GetType()           const;

    wxArrayString       GetAsArrayString()  const;
    wxBitmap            GetAsBitmap()       const;
    bool                GetAsBool()         const;
    Colour              GetAsColour()       const;
    double              GetAsDouble()       const;
    wxFont              GetAsFont()         const;
    int                 GetAsInt()          const;
    wxPoint             GetAsPoint()        const;
    wxSize              GetAsSize()         const;
    wxString            GetAsString()       const;
    long                GetAsStyle()        const;
    wxString            GetAsText()         const;
    wxString            GetAsURL()          const;
};

/**
    @class Object

    it keeps informations used by GUIManager to manage wxObject
    derived class instances.

    @beginEventEmissionTable
    @event{EVT_OBJECT_CREATING(func)}
        Process a @c wxEVT_OBJECT_CREATE event. See WidgetEvent.
    @event{EVT_OBJECT_CREATED(func)}
        Process a @c wxEVT_OBJECT_CREATED event. See WidgetEvent.
    @event{EVT_OBJECT_SELECTED(func)}
        Process a @c wxEVT_OBJECT_SELECTED event. See WidgetEvent.
    @event{EVT_OBJECT_DELETE(func)}
        Process a @c wxEVT_OBJECT_DELETE event. See WidgetEvent.
    @event{EVT_OBJECT_DELETED(func)}
        Process a @c wxEVT_OBJECT_DELETED event. See WidgetEvent.
    @endEventTable

    @library{core}
*/
class Object : public IObjectNode
{
public:
    /**
        Default constructor.
    */
    Object();
    /**
        Virtual destructor.
    */
    virtual ~Object();
    /**
        Sets the parent object.
    */
    void SetParent( Object *parent );
    /**
        Sets the object to be contained.
    */
    void SetControl( wxObject *object );
    /**
        Adds a child object to the children vector.
    */
    void AddChild( Object *child );
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
    Object *GetParent();
    /**
        Returns the wxObject contained.
    */
    wxObject *GetControl();
    /**
        Returns all the children objects.
    */
    wxVector< Object * > GetChildren();
};
/**
    @class ObjectTree

    Singleton class: GUI object's management.

    @library{core}
*/
class ObjectTree
{
public:
    /**
        Gets the static instance of this class.
    */
    static ObjectTree *Get();
    /**
        Free resources used by this class and destroy it.
    */
    static void Free();
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
