///////////////////////////////////////////////////////////////////////////////
// Name:        core/object/database.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/01/06
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

enum PropertyType
{
    PROPERTY_TYPE_UNKNOWN = 0,
    PROPERTY_TYPE_BITMAP,
    PROPERTY_TYPE_BOOL,
    PROPERTY_TYPE_CATEGORY,
    PROPERTY_TYPE_COLOUR,
    PROPERTY_TYPE_DIMENSION,
    PROPERTY_TYPE_DOUBLE,
    PROPERTY_TYPE_ENUM,
    PROPERTY_TYPE_EVENT,
    PROPERTY_TYPE_FLOAT,
    PROPERTY_TYPE_FLAG,
    PROPERTY_TYPE_FONT,
    PROPERTY_TYPE_INT,
    PROPERTY_TYPE_NAME,
    PROPERTY_TYPE_POINT,
    PROPERTY_TYPE_SIZE,
    PROPERTY_TYPE_STRING,
    PROPERTY_TYPE_STYLE,
    PROPERTY_TYPE_TEXT,
    PROPERTY_TYPE_URL
};

/**
    @class PropertyInfo

    Stores metadata informations for a given Property.
*/
class PropertyInfo
{
public:
    PropertyInfo( PropertyType type,
                const wxString &name, const wxString &label,
                const wxString &defaultValue = wxEmptyString,
                const wxString &description  = wxEmptyString );

    ~PropertyInfo();

    PropertyType GetType()         const;
    wxString     GetName()         const;
    wxString     GetLabel()        const;
    wxString     GetDefaultValue() const;
    wxString     GetDescription()  const;

    void AddChild( const wxString &name, PropertyInfo info );

    void SetChildren( PropertyInfoMap children ) { m_children = children; }
};
