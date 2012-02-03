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

enum ClassType
{
    CLASS_TYPE_UNKNOWN = 0,
    CLASS_TYPE_ABSTRACT,
    CLASS_TYPE_CONTAINER,
    CLASS_TYPE_CUSTOM,
    CLASS_TYPE_ITEM,
    CLASS_TYPE_LAYOUT,
    CLASS_TYPE_ROOT,
    CLASS_TYPE_TOPLEVEL,
    CLASS_TYPE_WIDGET
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
class ClassNode;

class EventNode;
class PropertyNode;
class ObjectNode;

class wxString;
class wxArrayString;

typedef class std::tr1::shared_ptr< EventInfoNode >     EventInfo;
typedef class std::tr1::shared_ptr< PropertyInfoNode >  PropertyInfo;
typedef class std::tr1::shared_ptr< ClassNode >         ClassInfo;

typedef class std::tr1::shared_ptr< EventNode >         Event;
typedef class std::tr1::shared_ptr< PropertyNode >      Property;
typedef class std::tr1::shared_ptr< ObjectNode >        Object;

typedef std::vector< Event >                            Events;
typedef std::vector< Property >                         Properties;
typedef std::vector< Object >                           Objects;

typedef std::vector< EventInfo >                        EventInfos;
typedef std::vector< PropertyInfo >                     PropertyInfos;
typedef std::vector< ClassInfo >                        ClassInfos;

// Event type name + event type description
typedef std::pair< wxString, wxString >                 EventType;

// Class type (widget, container, item) + max. instances allowed 
typedef std::pair< ClassType, int >                     AllowedChildType;

// E.g. class name "wxSplitterWindow", max = 2 
typedef std::pair< wxString, int >                      AllowedChildName;

typedef std::map< wxString, PropertyType >              PropertyTypeMap;
//typedef std::map< wxString, PropertyInfo >              PropertyInfoMap;
typedef std::map< wxString, ClassInfo >                 ClassInfoMap;
typedef std::map< wxString, Event >                     EventMap;

typedef std::vector< ClassType >                        ClassTypes;
typedef std::vector< EventType >                        EventTypes;

typedef std::vector< AllowedChildType >                 AllowedChildTypes;
typedef std::vector< AllowedChildName >                 AllowedChildNames;

#endif //__CORE_DEFS_H__
