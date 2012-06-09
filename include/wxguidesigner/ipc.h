///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/ipc.h
// Purpose:     Ensure that a file is opened only in a single app instance
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/16
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_IPC_H__
#define __WXGUIDESIGNER_IPC_H__

#include <wx/ipc.h>
#include <wx/snglinst.h>
#include <wx/scopedptr.h>

class IPCFileServer;

class IPCFile
{
public:
    IPCFile() : m_port( 4242 ) {}

    bool CheckSingleInstance( const wxString &filePath, bool switchTo = true );
    void Reset();

private:
    bool CreateServer( const wxString &serverName );

    wxScopedPtr< wxSingleInstanceChecker >  m_checker;
    wxScopedPtr< IPCFileServer >            m_server;
    const int                               m_port;
};

class IPCConnection: public wxConnection
{
public:
    IPCConnection() {}
    ~IPCConnection() {}
    
private:
    wxString m_data;
};

class IPCFileClient: public wxClient
{
public:
    IPCFileClient(){}

    wxConnectionBase *OnMakeConnection();
};

class IPCFileServer: public wxServer
{
public:
    IPCFileServer( const wxString& filePath ) : m_filePath( filePath ) {}

    wxConnectionBase* OnAcceptConnection( const wxString &topic );

    wxString GetFilePath() const { return m_filePath; }

private:
    const wxString m_filePath;
};

#endif //__WXGUIDESIGNER_IPC_H__
