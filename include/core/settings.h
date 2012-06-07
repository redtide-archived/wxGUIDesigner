///////////////////////////////////////////////////////////////////////////////
// Name:        core/settings.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/06
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGD_SETTINGS_H__
#define __WXGD_SETTINGS_H__

class wxAny;
class wxString;

#include <map>

using namespace std;

class Settings
{
public:
    Settings();
    ~Settings();

    // Delete all changes
    void DiscardChanges();

    // Return a changed or original entry on success, default value otherwise
    wxAny GetAny( const wxString &key, const wxAny &defaultValue = wxAny() ) const;

    wxString GetString( const wxString &key,
                        const wxString &defaultValue = wxEmptyString ) const;

    bool GetBool( const wxString &key, bool defaultValue = false ) const;

    int GetInt( const wxString &key, int defaultValue = 0 ) const;

    long GetLong( const wxString &key, long defaultValue = 0 ) const;

    // Set a new/previous entry without commit it
    bool SetEntry( const wxString &key, const wxAny &value );

    // Retrieve if the configuration has changed
    bool IsDirty() const;

    // Commit the changes to wxConfig
    bool Update();

private:
    // Load wxConfig groups recursively
    void LoadGroup( wxString &group, wxString &key,
                    long groupIndex, long keyIndex );

    // Load / Save wxConfig entry
    void LoadEntry( const wxString &key );
    void SaveEntry( const wxString &key, const wxAny &value );

    typedef map< wxString, wxAny > SettingsMap;
    SettingsMap m_values;  // Original values
    SettingsMap m_changes; // Only changed values
};

#endif //__WXGD_SETTINGS_H__
