///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/settings.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/06
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include <wx/any.h>
#include <wx/app.h>
#include <wx/config.h>
#include <wx/string.h>

#include "wxguidesigner/settings.h"

// TODO: handle global and/or local configs?
Settings::Settings()
{
    delete wxConfigBase::Set( new wxConfig( "wxGUIDesigner" ) );

    wxString key;
    wxString oldPath  = wxConfigBase::Get()->GetPath();
    long     keyIndex = 0;

    wxConfigBase::Get()->SetPath("/");

    // Get top level entries
    bool hasEntries = wxConfigBase::Get()->GetFirstEntry( key, keyIndex );
    while( hasEntries )
    {
        LoadEntry( key );
        hasEntries = wxConfigBase::Get()->GetNextEntry( key, keyIndex );
    }

    // Reset
    wxString group = key = ""; long groupIndex = keyIndex = 0;

    // Get groups recursively
    bool hasGroups = wxConfigBase::Get()->GetFirstGroup( group, groupIndex );
    while( hasGroups )
    {
        LoadGroup( group, key, groupIndex, keyIndex );
        hasGroups = wxConfigBase::Get()->GetNextGroup( group, groupIndex );
    }

    // Restore the original path
    wxConfigBase::Get()->SetPath( oldPath );
}

Settings::~Settings()
{
    m_values.clear();
    m_changes.clear();
}

void Settings::DiscardChanges()
{
    m_changes.clear();
}

wxAny Settings::GetAny( const wxString &key, const wxAny &defaultValue ) const
{
    // Return the new key if present
    SettingsMap::const_iterator changed = m_changes.find( key );
    if( changed != m_changes.end() )
        return changed->second;

    // Return the original key if present
    changed = m_values.find( key );
    if( changed != m_values.end() )
        return changed->second;

    // No key, return default value
    return defaultValue;
}

wxString Settings::GetString( const wxString &key,
                              const wxString &defaultValue ) const
{
    wxAny any = GetAny( key, defaultValue );

    if( any.CheckType< wxString >() )
        return any.As< wxString >();

    return defaultValue;
}

bool Settings::GetBool( const wxString &key, bool defaultValue ) const
{
    wxAny any = GetAny( key, defaultValue );

    if( any.CheckType< bool >() )
    {
        return any.As< bool >();
    }
    else if( any.CheckType< wxString >() )
    {
        wxString value = any.As< wxString >();

        if( !value.empty() && (value != "false") && (value != "0") )
            return true;

        return false;
    }

    return defaultValue;
}

int Settings::GetInt( const wxString &key, int defaultValue ) const
{
    wxAny any = GetAny( key, defaultValue );

    if( any.CheckType< int >() )
    {
        return any.As< int >();
    }
    else if( any.CheckType< wxString >() )
    {
        wxString value = any.As< wxString >();
        if( !value.empty() && value.IsNumber() )
            return wxAtoi( value );

        return 0;
    }

    return defaultValue;
}

long Settings::GetLong( const wxString &key, long defaultValue ) const
{
    wxAny any = GetAny( key, defaultValue );

    if( any.CheckType< long >() )
    {
        return any.As< long >();
    }
    else if( any.CheckType< wxString >() )
    {
        wxString value = any.As< wxString >();
        if( !value.empty() && value.IsNumber() )
            return wxAtol( value );

        return 0;
    }

    return defaultValue;
}

bool Settings::SetEntry( const wxString &key, const wxAny &value )
{
    // Check if the key is known...
    SettingsMap::iterator found = m_values.find( key );

    // ...and/or new but already present in m_changes
    SettingsMap::iterator changed = m_changes.find( key );

    // Is value changed?
    bool isDirty = false;
    if( found != m_values.end() )
        isDirty = !(m_values[key].HasSameType(value));

    // There was already an uncommitted change?
    if( changed != m_changes.end() )
    {
        if( isDirty )
            m_changes[key] = value;     // Update the uncommitted value.
        else
            m_changes.erase( changed ); // The new value is same as the original:
                                        // erase the previous changes.
        return true;
    }
    else // Brand new value
    {
        m_changes.insert( SettingsMap::value_type( key, value ) );
        return true;
    }

    return false;
}

bool Settings::IsDirty() const
{
    return (m_changes.size() != 0);
}

bool Settings::Update()
{
    SettingsMap::iterator original, changed;

    // Update the original values
    for( changed = m_changes.begin(); changed != m_changes.end(); ++changed )
    {
        wxString key      = changed->first;
        wxAny    newValue = changed->second;
                 original = m_values.find( key );

        if( original != m_values.end() )
            m_values[key] = newValue;
        else
            m_values.insert( SettingsMap::value_type( key, newValue ) );
    }

    // Reset changes
    m_changes.clear();

    // Write the configuration
    for( original = m_values.begin(); original != m_values.end(); ++original )
        SaveEntry( original->first, original->second );

    // Done
    return wxConfigBase::Get()->Flush();
}

void Settings::LoadEntry( const wxString &key )
{
    if( !wxConfigBase::Get()->HasEntry( key ) )
        return;

    wxString path  = wxConfigBase::Get()->GetPath() + "/" + key;
    wxString value = wxConfigBase::Get()->Read( path );

    // Store the key as full path, the caller will specify it
    m_values.insert( SettingsMap::value_type( path, value ) );
//  wxLogDebug( "Loading key:" + path + " value:" + value );
}

void Settings::SaveEntry( const wxString &key, const wxAny &value )
{
    switch( wxConfigBase::Get()->GetEntryType( key ) )
    {
        case wxConfigBase::Type_Boolean:
            wxConfigBase::Get()->Write( key, GetBool( key ) );
            break;

        case wxConfigBase::Type_Float:
//          wxConfigBase::Get()->Write( key, GetFloat( key ); // TODO
            break;

        case wxConfigBase::Type_Integer:
            wxConfigBase::Get()->Write( key, GetLong( key ) );
            break;

        case wxConfigBase::Type_String:
            wxConfigBase::Get()->Write( key, GetString( key ) );
            break;

        default:
            break; // TODO: Type_Unknown handling
    }
}

void Settings::LoadGroup( wxString &group, wxString &key,
                            long groupIndex, long keyIndex )
{
    // Change path to the current group
    wxString path = wxConfigBase::Get()->GetPath() + "/" + group;
    wxConfigBase::Get()->SetPath( path );

    // Load group entries
    bool hasEntries = wxConfigBase::Get()->GetFirstEntry( key, keyIndex );
    while( hasEntries )
    {
        LoadEntry( key );
        hasEntries = wxConfigBase::Get()->GetNextEntry( key, keyIndex );
    }

    // Count the subgroups and recurse if any
    size_t nGroups = wxConfigBase::Get()->GetNumberOfGroups( true );
    if( nGroups )
    {
        bool hasGroups = wxConfigBase::Get()->GetFirstGroup( group, groupIndex );
        while( hasGroups )
        {
            LoadGroup( group, key, groupIndex, keyIndex );
            hasGroups = wxConfigBase::Get()->GetNextGroup( group, groupIndex );
        }
    }

    wxConfigBase::Get()->SetPath(".."); // CWD
}
