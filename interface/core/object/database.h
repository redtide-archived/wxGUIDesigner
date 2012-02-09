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
    @class ChildInfo

    Stores metadata informations for a given class name or type as allowed
    child for the current class.
*/
class ChildInfo
{
public:
/**
    Constructor
    @param type Child ClassType.
    @param name Child class name.
    @param max Maximum amount of child class instances allowed.
    @param option The child class is part of an option group.
*/
    ChildInfo( ClassType        type      = CLASS_TYPE_UNKNOWN,
                const wxString &name      = wxEmptyString,
                int             max       = 0,
                bool            option    = false );
/**
    Destructor
*/
    ~ChildInfo();
/**
    Return the child class type.
*/
    ClassType GetType() const;
/**
    Return the child class name.
*/
    wxString GetName() const;
/**
    Return the maximum amount of instances allowed.
*/
    int GetMax() const;
/**
    Return if this child class is part of an option group.
*/
    bool IsOption() const;
/**
    Return if this child is defined by its type.
*/
    bool IsType() const;
};
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
    Return the event class name.
*/
    wxString GetName() const;
/**
    Return the event class description.
*/
    wxString GetDescription() const;
/**
    Return the event type name at the given index.
*/
    wxString GetTypeName( size_t index ) const;
/**
    Return the event type description at the given index.
*/
    wxString GetTypeDescription( size_t index ) const;
/**
    Return the event type count.
*/
    size_t GetTypeCount() const;

private:
/**
    Add an event type to this event class.
    @param name Event type name (e.g. "wxEVT_CLOSE")
    @param description A description about the event class
*/
    void AddType( const wxString &name, const wxString &description );
/**
    Sets the description.
*/
    void SetDescription( const wxString &description );
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
    PropertyInfo( PropertyType    type,
                  const wxString &name,
                  const wxString &label );
/**
    Destructor
*/
    ~PropertyInfo();
/**
    Return the property type.
*/
    PropertyType GetType() const;
/**
    Return the property name.
*/
    wxString GetName() const;
/**
    Return the property label.
*/
    wxString GetLabel() const;
/**
    Return the property default value if any.
*/
    wxString GetDefaultValue() const;
/**
    Return the property description (if any).
*/
    wxString GetDescription() const;
/**
    Return how many children are contained in this property info.
*/
    size_t GetChildCount() const;
/**
    Get a child from specified index.
*/
    PropertyInfo GetChild( size_t index );
/**
    Add a property child to this property.
    Used for PROPERTY_TYPE_CATEGORY.
    @param name Property info name for the child
    @param info child property to add
*/
    void AddChild( const wxString &name, PropertyInfo info );

private:
    void SetDefaultValue( const wxString &value );
    void SetDescription( const wxString &description );
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
    Return the class name.
*/
    wxString GetName() const;
/**
    Return the class description.
*/
    wxString GetDescription() const;
/**
    Return the class type.
*/
    wxString GetType() const;
/**
    Return if this class inherits the specified class name.
    E.g. wxDialog inherits from wxTopLevelWindow and so wxWindow
*/
    bool IsKindOf( const wxString &className );
/**
    Return if this class equals the specified class type.
    E.g. wxFrame is a CLASS_TYPE_TOPLEVEL
*/
    bool IsTypeOf( ClassType type );
/**
    Return how many classes are inherited from this class.
*/
    size_t GetBaseCount();
/**
    Return how many class types are granted to be children
    of an instance of this class.
*/
    size_t GetChildInfoCount();
/**
    Return how many events are emitted from this class.
*/
    size_t GetEventInfoCount();
/**
    Return how many properties are defined in this class.
*/
    size_t GetPropertyInfoCount();
/**
    Return the name of the inherited class name at specified index.
*/
    wxString GetBaseName( size_t index );
/**
    Return the class information about an allowed child for the current class.
*/
    ChildInfo GetChildInfo( size_t index );
/**
    Return the event info object stored at the specified index.
*/
    EventInfo GetEventInfo( size_t index );
/**
    Return the property info object stored at the specified index.
*/
    PropertyInfo GetPropertyInfo( size_t index );
/**
    Return if the specified class name is allowed as child of the current class,
    specifying the current amount of children of the same type.
*/
    bool IsChildInfoOk( const wxString &name, size_t count );
/**
    Return if a property with the given name exists in the property list.
*/
    bool PropertyInfoExists( const wxString &name );

private:
    void AddBaseName( const wxString &name );
    void AddChildInfo( ChildInfo info );
    void AddEventInfo( EventInfo info );
    void AddPropertyInfo( PropertyInfo info );
};
/**
    @class ClassInfoDB

    ClassInfo Database.
*/
class ClassInfoDB
{
public:
    static ClassInfoDB *Get();
    static void Free();

    bool         ClassInfoExists( const wxString &name );
    ClassInfo    GetClassInfo   ( const wxString &name ) const;
    PropertyType GetPropertyType( const wxString &name ) const;
};
