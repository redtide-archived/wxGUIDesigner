///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/ipc.h
// Purpose:     Ensure that a file is opened only in a single app instance
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/05/16
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_IPC_H__
#define __WXGUIDESIGNER_IPC_H__

#include <wx/ipc.h>
#include <wx/snglinst.h>
#include <wx/scopedptr.h>

namespace wxGD
{
namespace IPC
{
class FileServer;

class File
{
public:
    File() : m_port( 4242 ) {}

    bool CheckSingleInstance( const wxString &filePath, bool switchTo = true );
    void Reset();

private:
    bool CreateServer( const wxString &serverName );

    wxScopedPtr< wxSingleInstanceChecker >  m_checker;
    wxScopedPtr< FileServer >               m_server;
    const int                               m_port;
};

class Connection: public wxConnection
{
public:
    Connection() {}
    ~Connection() {}
    
private:
    wxString m_data;
};

class FileClient: public wxClient
{
public:
    FileClient(){}

    wxConnectionBase *OnMakeConnection();
};

class FileServer: public wxServer
{
public:
    FileServer( const wxString& filePath ) : m_filePath( filePath ) {}

    wxConnectionBase* OnAcceptConnection( const wxString &topic );

    wxString GetFilePath() const { return m_filePath; }

private:
    const wxString m_filePath;
};
}; // namespace IPC
}; // namespace wxGD

#endif //__WXGUIDESIGNER_IPC_H__
