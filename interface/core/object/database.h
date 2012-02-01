///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/database.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/06
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
/**
    @class EventInfo

    Stores metadata informations for a given Event.
*/
class EventInfo
{
public:
/**
    Constructor
    @param name Event class name (e.g. "wxCommandEvent")
    @param description A description about the event class
*/
    EventInfo( const wxString &name,
               const wxString &description = wxEmptyString );
/**
    Destructor
*/
    ~EventInfo();
/**
    Returns the event class name.
*/
    wxString GetClassName();
/**
    Returns the event class description.
*/
    wxString GetClassDescription();
/**
    Returns the event type name at the given index.
*/
    wxString GetTypeName( size_t index );
/**
    Returns the event type description at the given index.
*/
    wxString GetTypeDescription( size_t index );
/**
    Returns the event type count.
*/
    size_t   GetTypeCount();
/**
    Add an event type to this event class.
    @param name Event type name (e.g. "wxEVT_CLOSE")
    @param description A description about the event class
*/
    void AddType( const wxString &name, const wxString &description );
};
/**
    @class PropertyInfo

    Stores metadata informations for a given Property.
*/
class PropertyInfo
{
public:
/**
    Constructor
    @param type Property type.
    @param name Property name.
    @param label Translatable Property label.
*/
    PropertyInfo( PropertyType type,
                  const wxString &name,
                  const wxString &label );
/**
    Destructor
*/
    ~PropertyInfo();
/**
    Returns the property type.
*/
    PropertyType GetType()         const;
/**
    Returns the property name.
*/
    wxString     GetName()         const;
/**
    Returns the property label.
*/
    wxString     GetLabel()        const;
/**
    Returns the property default value if any.
*/
    wxString     GetDefaultValue() const;
/**
    Returns the property description (if any).
*/
    wxString     GetDescription()  const;
/**
    Returns how many children are contained in this property info.
*/
    size_t       GetChildCount();
/**
    Add a property child to this property.
    Used for PROPERTY_TYPE_CATEGORY.
    @param name Property info name for the child
    @param info child property to add
*/
    void AddChild( const wxString &name, PropertyInfo info );
};
/**
    @class ClassInfo

    Stores metadata informations about an Object.
*/
class ClassInfo
{
public:
/**
    Constructor
*/
    ClassInfo( const wxString &className, ClassType type = CLASS_TYPE_WIDGET );
/**
    Destructor
*/
    ~ClassInfo();
/**
    Returns the class name.
*/
    wxString        GetClassName() const;
/**
    Returns if this class is a possible child of the specified class name.
    E.g. a wxButton can be a child of a toplevel (wxTopLevelWindow)
         or layout (wxSizer or sizeritem) class type
*/
    bool            GetMaxAllowedBy( const wxString &className );
/**
    Returns if this class inherits the specified class name.
    E.g. wxDialog inherits from wxTopLevelWindow and so wxWindow
*/
    bool            IsKindOf( const wxString &className );
/**
    Returns if this class equals the specified class type.
    E.g. wxFrame is a CLASS_TYPE_TOPLEVEL
*/
    bool            IsTypeOf( ClassType type );
/**
    Returns how many classes are inherited from this class.
*/
    size_t          GetBaseCount();
/**
    Returns how many class types are granted to be children
    of an instance of this class.
*/
    size_t          GetChildTypeCount();
/**
    Returns how many events are emitted from this class.
*/
    size_t          GetEventInfoCount();
/**
    Returns how many properties are defined in this class.
*/
    size_t          GetPropertyInfoCount();
/**
    Returns the name of the inherited class name at specified index.
*/
    wxString        GetBaseName( size_t index );
/**
    Returns the child type information stored at the specified index.
*/
    AllowedChildType       GetAllowedChildType( size_t index ) const;
/**
    Returns the event info object stored at the specified index.
*/
    EventInfo       GetEventInfo( size_t index );
/**
    Returns the property info object stored at the specified index.
*/
    PropertyInfo    GetPropertyInfo( size_t index );
/**
    Returns if a property with the given name exists in the property list.
*/
    bool            PropertyInfoExists ( const wxString &name );
};

class ClassInfoDB
{
public:
    static ClassInfoDB *Get();
    static void Free();

    bool         ClassInfoExists( const wxString &name );
    ClassInfo    GetClassInfo   ( const wxString &name ) const;
    PropertyType GetPropertyType( const wxString &name ) const;
};
