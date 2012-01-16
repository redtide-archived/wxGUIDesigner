///////////////////////////////////////////////////////////////////////////////
// Name:        core/defs.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/12/22
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_DEFS_H__
#define __WXGDCORE_DEFS_H__

#include "core/dllimpexp.h"

#include <boost/tr1/memory.hpp>
#include <list>
#include <map>
#include <vector>

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

class EventInfoBase;
class PropertyInfoBase;
class ClassInfoBase;

class EventBase;
class PropertyBase;
class ObjectBase;

class wxString;

typedef class std::tr1::shared_ptr< EventInfoBase >     EventInfo;
typedef class std::tr1::shared_ptr< PropertyInfoBase >  PropertyInfo;
typedef class std::tr1::shared_ptr< ClassInfoBase >     ClassInfo;

typedef class std::tr1::shared_ptr< EventBase >         Event;
typedef class std::tr1::shared_ptr< PropertyBase >      Property;
typedef class std::tr1::shared_ptr< ObjectBase >        Object;

typedef std::map< wxString, EventInfo >                 EventInfoMap;
typedef std::map< wxString, PropertyInfo >              PropertyInfoMap;
typedef std::map< wxString, ClassInfo >                 ClassInfoMap;

typedef std::map< wxString, wxString >                  EventTypeMap;
typedef std::map< wxString, PropertyType >              PropertyTypeMap;

typedef std::vector< Property >                         Properties;
typedef std::vector< Event >                            Events;
typedef std::list< Object >                             Objects;

#endif //__WXGDCORE_DEFS_H__
