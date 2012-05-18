///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/manager.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/01/13
// Revision:    $Hash$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __CORE_MANAGER_H__
#define __CORE_MANAGER_H__

#include "core/dllimpexp.h"

#include <boost/tr1/memory.hpp>

#include "core/ipc.h"

class wxFrame;
class wxString;
class wxWindow;

using namespace std::tr1;

class DLLIMPEXP_CORE wxGUIDesigner
{
public:
    static wxGUIDesigner *Get();
    static void Free();

    // GUIManager
    wxFrame     *GetMainFrame( wxWindow *parent );

    // Project
    void        NewProject();
    bool        LoadProject( const wxString &filePath, bool check = true );
    bool        SaveProject( const wxString &filePath );
    wxString    GetProjectFileName() const  { return m_currPrj; }
    wxString    GetProjectFilePath() const  { return m_currDir; }
    bool        IsProjectModified()         { return m_isChanged; }
    bool        CheckSingleInstance( const wxString &filePath,
                                        bool switchTo = true );
    void        GenerateCode();

private:
    wxGUIDesigner();
    ~wxGUIDesigner();

    static wxGUIDesigner    *ms_instance;

    shared_ptr< IPCFile >   m_ipcFile;
    wxString                m_currPrj;
    wxString                m_currDir;
    bool                    m_isChanged;
};

#endif //__CORE_MANAGER_H__
