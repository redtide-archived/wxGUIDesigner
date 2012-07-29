///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/rtti/tree.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/11/21
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_RTTI_TREE_H__
#define __WXGUIDESIGNER_RTTI_TREE_H__

#include "wxguidesigner/rtti.h"

class wxBitmap;
class wxColour;
class wxFontContainer;
class wxPoint;
class wxSize;
class wxString;
class wxXmlDocument;

namespace wxGD
{
namespace RTTI
{
//=============================================================================
// EventNode Class
//=============================================================================
class EventNode
{
public:
    EventNode( EventInfo eventInfo );
    ~EventNode();

    wxString        GetName()                                   const;
    wxString        GetDescription()                            const;
    wxString        GetHandlerName    ( size_t typeIndex )      const;
    wxString        GetTypeName       ( size_t typeIndex )      const;
    wxString        GetTypeDescription( size_t typeIndex )      const;
    size_t          GetTypeCount()                              const;
    Handlers        GetHandlers()                               const;
    bool            HasHandlers();

    void            SetHandlerName    ( size_t typeIndex,
                                        const wxString &funcName );
    void            SetHandlerName    ( const wxString &typeName,
                                        const wxString &funcName );
private:
    EventInfo       m_eventInfo;
    Handlers        m_handlers;
};
//=============================================================================
// PropertyNode Class
//=============================================================================
class PropertyNode
{
public:
    PropertyNode( PropertyInfo info );
    ~PropertyNode();

    wxString        GetName()                                   const;
    wxString        GetLabel()                                  const;
    wxString        GetDescription()                            const;
    PropertyType    GetType()                                   const;
    PropertyInfo    GetInfo()                                   const;
    bool            IsCategory()                                const;
//-----------------------------------------------------------------------------
// Children
//-----------------------------------------------------------------------------
    void            AddChild( Property property );
    Property        GetChild( size_t index )                    const;
    Properties      GetChildren()                               const;
    size_t          GetChildCount()                             const;
//-----------------------------------------------------------------------------
// Attributes
//-----------------------------------------------------------------------------
    void            AddAttribute      ( const wxString &name,
                                        const wxString &value );
    Attribute       GetAttribute        ( size_t index )        const;
    wxString        GetAttributeName    ( size_t index )        const;
    wxString        GetAttributeValue   ( size_t index )        const;
    size_t          GetAttributeCount()                         const;
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
    int             GetAsInteger()                              const;
    int             GetAsStyle()                                const;
    int             GetAsSystemColour()                         const;
    bool            GetAsBool()                                 const;
    double          GetAsDouble()                               const;
    wxArrayString   GetAsArrayString()                          const;
    wxColour        GetAsColour()                               const;
    wxFontContainer GetAsFont()                                 const;
    wxPoint         GetAsPoint()                                const;
    wxSize          GetAsSize()                                 const;
    wxString        GetAsString()                               const;
    wxString        GetAsText()                                 const;
    wxString        GetAsURL()                                  const;
//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------
    void            SetValue( int                   value );
    void            SetValue( bool                  value );
    void            SetValue( double                value );
    void            SetValue( const wxArrayString   &value );
    void            SetValue( const wxFontContainer &value );
    void            SetValue( const wxPoint         &value );
    void            SetValue( const wxSize          &value );
    void            SetValue( const wxString        &value );
    void            SetValue( const wxColour        &value, int type = 0xFFFFFF );

private:
    Attributes      m_attributes;
    PropertyInfo    m_propertyInfo;
    Properties      m_children;
    wxString        m_value;
};
//=============================================================================
// ObjectNode Class
//=============================================================================
class ObjectNode : public std::tr1::enable_shared_from_this< ObjectNode >
{
public:
    ObjectNode( ClassInfo classInfo, Object parent,
                bool expanded = true, bool isReference = false );
    ~ObjectNode();

    wxString        GetName()                                   const;
    wxString        GetDescription()                            const;

    bool            IsReference()                               const;
    bool            IsRoot()                                    const;
    bool            IsTopLevel()                                const;
//-----------------------------------------------------------------------------
// State into the tree
//-----------------------------------------------------------------------------
    bool            IsExpanded()                                const;
    void            Collapse();
    void            Expand();
//-----------------------------------------------------------------------------
// ClassInfos
//-----------------------------------------------------------------------------
    ClassInfo       GetClassInfo()                              const;
    wxString        GetClassName()                              const;
//-----------------------------------------------------------------------------
// EventProperties
//-----------------------------------------------------------------------------
    void            AddEvent        ( EventProperty event );
    EventProperty   GetEvent        ( size_t index )            const;
    EventProperty   GetEvent        ( const wxString &name )    const;
    EventProperties GetEvents()                                 const;
    size_t          GetEventCount()                             const;
//-----------------------------------------------------------------------------
// Properties
//-----------------------------------------------------------------------------
    void            AddProperty     ( Property prop );
    Property        GetProperty     ( const wxString &name )    const;
    Property        GetProperty     ( size_t index )            const;
    Property        GetChildProperty( Property parent,
                                        const wxString &name )  const;
    Properties      GetProperties()                             const;
    size_t          GetPropertyCount()                          const;
//-----------------------------------------------------------------------------
// Parent / children objects
//-----------------------------------------------------------------------------
    void            AddChild( Object child );
    Object          GetChild( size_t index )                    const;
    Object          GetParent()                                 const;
    Object          GetTopLevelParent();
    Objects         GetChildren()                               const;
    size_t          GetChildCount()                             const;

private:
    ClassInfo       m_classInfo;
    Object          m_parent;
    Objects         m_children;
    bool            m_expanded;
    bool            m_isRef;
    EventProperties m_events;
    Properties      m_properties;
};
//=============================================================================
// ObjectTree Class
//=============================================================================
class ObjectTree
{
public:
    ObjectTree();
    ~ObjectTree();

    Object          CreateObject  ( const wxString &name,
                                    Object parent = Object() );
    void            SelectObject  ( Object object );

    Object          GetRootObject()                             const;
    Object          GetSelectedObject()                         const;
    size_t          GetSiblingsCount  ( Object parent,
                                        ClassInfo classInfo )   const;
    bool            IsChildOk         ( Object parent,
                                        ClassInfo classInfo )   const;
private:
    Object          m_root;
    Object          m_selected;

}; // ObjectTree
}; // namespace RTTI
}; // namespace wxGD

#endif //__WXGUIDESIGNER_RTTI_TREE_H__
