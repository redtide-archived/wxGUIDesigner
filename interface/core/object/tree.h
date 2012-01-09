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
    @class Object

    it keeps informations used by GUIManager to manage wxObject
    derived class instances.

    @beginEventEmissionTable
    @event{EVT_OBJECT_CREATING(func)}
        Process a @c wxEVT_OBJECT_CREATE event. See ObjectEvent.
    @event{EVT_OBJECT_CREATED(func)}
        Process a @c wxEVT_OBJECT_CREATED event. See ObjectEvent.
    @event{EVT_OBJECT_SELECTED(func)}
        Process a @c wxEVT_OBJECT_SELECTED event. See ObjectEvent.
    @event{EVT_OBJECT_DELETE(func)}
        Process a @c wxEVT_OBJECT_DELETE event. See ObjectEvent.
    @event{EVT_OBJECT_DELETED(func)}
        Process a @c wxEVT_OBJECT_DELETED event. See ObjectEvent.
    @endEventTable

    @library{core}
*/
class Object : public PluginItem
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
    void Free();
    /**
        Create a new control object.
    */
    void CreateObject( const wxString &classname );
};
/**
    @class ObjectEvent

    @library{core}
*/
class ObjectEvent
{
public:
    /**
        Default constructor.
    */
    ObjectEvent( wxEventType type = wxEVT_NULL, int id = 0 );
    /**
        Destructor.
    */
    ~ObjectEvent();

    
};
