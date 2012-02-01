///////////////////////////////////////////////////////////////////////////////
// Name:        core/defs.h
// Purpose:     interface of Object
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/21
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    Class type (default: widget type).
*/
enum ClassType
{
    CLASS_TYPE_UNKNOWN = 0,
    CLASS_TYPE_WIDGET,
    CLASS_TYPE_ABSTRACT,
    CLASS_TYPE_CONTAINER,
    CLASS_TYPE_CUSTOM,
    CLASS_TYPE_ITEM,
    CLASS_TYPE_LAYOUT,
    CLASS_TYPE_ROOT,
    CLASS_TYPE_TOPLEVEL
};

/**
    ObjectTree event types.
*/
enum ObjectEventType
{
    EVT_OBJECT_CREATED,
    EVT_OBJECT_DELETED,
    EVT_OBJECT_EXPANDED,
    EVT_OBJECT_SELECTED
};

/**
    PropertyInfo types.
*/
enum PropertyType
{
    PROPERTY_TYPE_UNKNOWN = 0,
    PROPERTY_TYPE_ARRAYSTRING,
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
    PROPERTY_TYPE_ITEMS,
    PROPERTY_TYPE_LISTCOLFMT,
    PROPERTY_TYPE_NAME,
    PROPERTY_TYPE_POINT,
    PROPERTY_TYPE_SIZE,
    PROPERTY_TYPE_STRING,
    PROPERTY_TYPE_STYLE,
    PROPERTY_TYPE_TEXT,
    PROPERTY_TYPE_URL
};
