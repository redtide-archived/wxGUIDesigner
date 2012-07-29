///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/12/22
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_DEFS_H__
#define __WXGUIDESIGNER_DEFS_H__

#include "wxguidesigner/dllimpexp.h"

#include <map>
#include <utility>
#include <vector>

#include <boost/tr1/memory.hpp>

#include <wx/defs.h>

class wxString;

namespace wxGD
{
typedef std::vector< wxString > Strings;

namespace RTTI
{
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
//=============================================================================
// ClassTree
//
// Note: unique_ptr not available and scoped_ptr can't be used
//       inside STL containers
//=============================================================================
class EventInfoNode;
class EventTypeInfoNode;
class PropertyInfoNode;
class ChildInfoNode;
class ClassNode;

typedef std::tr1::shared_ptr< ChildInfoNode >       ChildInfo;
typedef std::tr1::shared_ptr< EventInfoNode >       EventInfo;
typedef std::tr1::shared_ptr< EventTypeInfoNode >   EventTypeInfo;
typedef std::tr1::shared_ptr< PropertyInfoNode >    PropertyInfo;
typedef std::tr1::shared_ptr< ClassNode >           ClassInfo;

typedef std::vector< ChildInfo >                    ChildInfos;
typedef std::vector< EventTypeInfo >                EventTypeInfos;
typedef std::vector< EventInfo >                    EventInfos;
typedef std::vector< PropertyInfo >                 PropertyInfos;
typedef std::vector< ClassInfo >                    ClassInfos;

typedef std::map< wxString, PropertyType >          PropertyTypeMap;
typedef std::map< wxString, ClassInfo >             ClassInfoMap;
//=============================================================================
// ObjectTree
//=============================================================================
class EventNode;
class PropertyNode;
class ObjectNode;
class ObjectTree;

typedef std::pair< wxString, wxString >             Attribute;
typedef std::tr1::shared_ptr< EventNode >           EventProperty;
typedef std::tr1::shared_ptr< PropertyNode >        Property;
typedef std::tr1::shared_ptr< ObjectNode >          Object;
typedef std::tr1::shared_ptr< ObjectTree >          Tree;

typedef std::vector< Attribute >                    Attributes;
typedef std::vector< EventProperty >                EventProperties;
typedef std::vector< wxString >                     Handlers;
typedef std::vector< Property >                     Properties;
typedef std::vector< Object >                       Objects;

}; // namespace RTTI
}; // namespace wxGD

#endif //__WXGUIDESIGNER_DEFS_H__
