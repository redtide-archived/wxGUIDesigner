///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/ipc.cpp
// Purpose:     Ensure that a file is opened only in a single app instance
// Author:      Andrea Zanellato
// Modified by:
// Created:     2012/05/16
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/ipc.h"

#include <wx/filename.h>
#include <wx/frame.h>

bool wxGD::IPC::File::CheckSingleInstance( const wxString &filePath, bool switchTo )
{
    wxFileName fileName( filePath );
    if( !fileName.IsOk() )
    {
        wxLogError( "IPC::File: Invalid path: %s", filePath );
        return false;
    }

    if( !fileName.IsAbsolute() )
    {
        if( !fileName.MakeAbsolute() )
        {
            wxLogError( "IPC::File: Invalid absolute path: %s", filePath );
            return false;
        }
    }

    // Create lockfile/mutex name
    wxString lockFileName =
    wxString::Format( "wxguidesigner-%s-%s", wxGetUserId(),
                                            fileName.GetFullPath() );
    // Get forbidden characters
    wxString forbidden = wxFileName::GetForbiddenChars();

    // Replace forbidded characters
    for( size_t c = 0; c < forbidden.Length(); ++c )
    {
        wxString bad( forbidden.GetChar( c ) );
        lockFileName.Replace( bad, "_" );
    }

    // Paths are not case sensitive in windows
#ifdef __WXMSW__
    lockFileName = lockFileName.MakeLower();
#else
    // GetForbiddenChars is missing "/" in unix.
    // Prepend '.' to make lockfiles hidden
    lockFileName.Replace( wxFILE_SEP_PATH, "_" );
    lockFileName.Prepend(".");
#endif
    // Check to see if I already have a server with this name
    // if so, no need to make another.
    if( m_server )
    {
        if( m_server->GetFilePath() == lockFileName )
            return true;
    }

    wxScopedPtr< wxSingleInstanceChecker > checker;
    {
        checker.reset( new wxSingleInstanceChecker( lockFileName ) );
    }

    if( !checker->IsAnotherRunning() )
    {
        // This is the first instance of this project,
        // so setup a server and save the single instance checker
        if( CreateServer( lockFileName ) )
        {
            m_checker.swap( checker );
            return true;
        }
        else
        {
            return false;
        }
    }
    else if( switchTo )
    {
        // There is another app, so connect and send the expression

        // Cannot have a client and a server at the same time,
        // due to the implementation of wxTCPServer and wxTCPClient,
        // so temporarily drop the server if there is one
        bool hadServer = false;
        wxString oldName;
        if( m_server )
        {
            oldName = m_server->GetFilePath();
            m_server.reset();
            hadServer = true;
        }

        // Create the client
        wxScopedPtr< FileClient > client( new FileClient );

        // Create the connection
        wxScopedPtr< wxConnectionBase > connection;

#ifdef __WXMSW__
        connection.reset( client->MakeConnection( "localhost", lockFileName, lockFileName ) );
#else
        bool connected = false;
        for( int i = m_port; i < m_port + 20; ++i )
        {
            wxString sPort = wxString::Format( "%i", i );

            connection.reset( client->MakeConnection( "localhost", sPort, lockFileName ) );

            if( connection )
            {
                connected = true;
                wxChar* pid = (wxChar*)connection->Request( "PID", NULL );
                if( pid )
                {
                    wxLogStatus( "%s already open in process %s", lockFileName, pid );
                }
                break;
            }
        }
        if( !connected )
        {
            wxLogError( "There is a lockfile named '%s', but unable to make a connection to that instance.", lockFileName );
        }
#endif
        // Drop the connection and client
        connection.reset();
        client.reset();

        // Create the server again, if necessary
        if( hadServer )
            CreateServer( oldName );
    }

    return false;
}

bool wxGD::IPC::File::CreateServer( const wxString &serverName )
{
    wxScopedPtr< FileServer > server( new FileServer( serverName ) );

#ifdef __WXMSW__
    if( server->Create( serverName ) )
    {
        m_server.swap( server );

        wxLogDebug("Server %s started", serverName);
#if wxUSE_DDE_FOR_IPC
        wxLogDebug("Server uses DDE");
#else
        wxLogDebug("Server uses TCP");
#endif
        return true;
    }
#else
    for( int i = m_port; i < m_port + 20; ++i )
    {
        wxString nameWithPort = wxString::Format( "%i%s", i, serverName );
        if( server->Create( nameWithPort ) )
        {
            m_server.swap( server );
            return true;
        }
    }
#endif

    wxLogError( "Failed to create an IPC service with name %s", serverName );
    return false;
}

void wxGD::IPC::File::Reset()
{
    m_server.reset();
    m_checker.reset();
}

wxConnectionBase* wxGD::IPC::FileServer::OnAcceptConnection( const wxString &topic )
{
    if( topic == m_filePath )
    {
        wxFrame *frame = wxDynamicCast( wxTheApp->GetTopWindow(), wxFrame );
        if( !frame )
            return NULL;

        frame->Enable();

        if( frame->IsIconized() )
            frame->Iconize( false );

        frame->Raise();

        return new Connection;
    }

    return NULL;
}

wxConnectionBase* wxGD::IPC::FileClient::OnMakeConnection()
{
    return new Connection;
}
