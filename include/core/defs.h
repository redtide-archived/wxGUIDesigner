///////////////////////////////////////////////////////////////////////////////
// Name:        core/defs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/12/22
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_DEFS_H__
#define __CORE_DEFS_H__

#include "core/dllimpexp.h"

#include <boost/tr1/memory.hpp>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include <wx/bitmap.h>
#include <wx/colour.h>
#include <wx/defs.h>

using namespace std;
using namespace tr1;

enum ClassType
{
    CLASS_TYPE_UNKNOWN = 0,
    CLASS_TYPE_ROOT,
    CLASS_TYPE_WIDGET,
    CLASS_TYPE_CONTAINER,
    CLASS_TYPE_CUSTOM,
    CLASS_TYPE_WINDOW = (CLASS_TYPE_WIDGET|CLASS_TYPE_CONTAINER|CLASS_TYPE_CUSTOM),
    CLASS_TYPE_TOPLEVEL,
    CLASS_TYPE_LAYOUT,
    CLASS_TYPE_SIZER,
    CLASS_TYPE_ABSTRACT,
    CLASS_TYPE_ITEM
};

enum ObjectEventType
{
    EVT_OBJECT_CREATED,
    EVT_OBJECT_DELETED,
    EVT_OBJECT_EXPANDED,
    EVT_OBJECT_SELECTED
};

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

class EventInfoNode;
class PropertyInfoNode;
class ChildInfoNode;
class ClassNode;

class EventNode;
class PropertyNode;
class ObjectNode;

class wxArrayString;
class wxString;
//*****************************************************************************
// ClassInfoDB
//*****************************************************************************

typedef shared_ptr< ChildInfoNode >     ChildInfo;
typedef shared_ptr< EventInfoNode >     EventInfo;
typedef shared_ptr< PropertyInfoNode >  PropertyInfo;
typedef shared_ptr< ClassNode >         ClassInfo;

// Event type name + event type description
typedef pair< wxString, wxString >      EventType;

typedef vector< ChildInfo >             ChildInfos;
typedef vector< EventType >             EventTypes;
typedef vector< EventInfo >             EventInfos;
typedef vector< PropertyInfo >          PropertyInfos;
typedef vector< ClassInfo >             ClassInfos;

typedef map< wxString, PropertyType >   PropertyTypeMap;
typedef map< wxString, ClassInfo >      ClassInfoMap;
//*****************************************************************************
// ObjectTree
//*****************************************************************************

const wxInt32 ColourCustom = 0xFFFFFF;

struct Colour
{
    wxInt32  type;
    wxColour colour;
};

typedef pair< wxString, wxString >      Attribute;
typedef vector< Attribute >             Attributes;

typedef shared_ptr< EventNode >         Event;
typedef shared_ptr< PropertyNode >      Property;
typedef shared_ptr< ObjectNode >        Object;

typedef vector< Event >                 Events;
typedef vector< Property >              Properties;
typedef vector< Object >                Objects;

#endif //__CORE_DEFS_H__
