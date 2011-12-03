///////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/20
// Revision:    $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "main.h"

#include <manager.h>

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/stdpaths.h>

#ifdef __WXMSW__
    #include <wx/sysopt.h>
#endif

// Abnormal Termination Handling
#if wxUSE_ON_FATAL_EXCEPTION && wxUSE_STACKWALKER
    #include <wx/stackwalk.h>
    #include <wx/utils.h>
#elif defined(_WIN32) && defined(__MINGW32__)
    #include "debug/stack_trace/stack.hpp"
    #include <sstream>
    #include <excpt.h>

    EXCEPTION_DISPOSITION StructuredExceptionHandler
                            (
                                struct _EXCEPTION_RECORD *ExceptionRecord,
                                void  * EstablisherFrame,
                                struct _CONTEXT *ContextRecord,
                                void  * DispatcherContext
                            );
#endif
void LogStack();

static const wxCmdLineEntryDesc s_cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, "g", "generate", wxString(_("Generate code from passed file.") ) },
    { wxCMD_LINE_OPTION, "l", "language", wxString(_("Override the code_generation property from the passed file and generate the passed languages. Separate multiple languages with commas.") ) },
    { wxCMD_LINE_SWITCH, "h", "help",     wxString(_("Show this help message.") ), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_HELP  },
    { wxCMD_LINE_PARAM, NULL, NULL,       wxString(_("File to open.") ),           wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_NONE }
};

IMPLEMENT_APP( wxGUIDesignerApp )

int wxGUIDesignerApp::OnRun()
{
    // Abnormal Termination Handling
    #if wxUSE_ON_FATAL_EXCEPTION && wxUSE_STACKWALKER
        ::wxHandleFatalExceptions( true );
    #elif defined(_WIN32) && defined(__MINGW32__)
        // Structured Exception handlers are stored in a linked list at FS:[0]
        // THIS MUST BE A LOCAL VARIABLE - windows won't use an object outside of the thread's stack frame
        EXCEPTION_REGISTRATION ex;
        ex.handler = StructuredExceptionHandler;
        asm volatile ("movl %%fs:0, %0" : "=r" (ex.prev));
        asm volatile ("movl %0, %%fs:0" : : "r" (&ex));
    #endif

    // Get the data directory
    wxStandardPathsBase& stdPaths = wxStandardPaths::Get();
    wxString             dataDir  = stdPaths.GetDataDir();
    dataDir.Replace( GetAppName(), "wxguidesigner" );

    // Parse command line
    wxCmdLineParser parser( s_cmdLineDesc, argc, argv );
    if ( 0 != parser.Parse() )
    {
        return 1;
    }

    // Get project to load
    wxString projectToLoad = wxEmptyString;
    if ( parser.GetParamCount() > 0 )
    {
        projectToLoad = parser.GetParam();
    }

    bool generatingCode = false;
    wxString language;
    bool hasLanguage = parser.Found( "l", &language );
    if ( parser.Found("g") )
    {
        if ( projectToLoad.empty() )
        {
            wxLogError( _("You must pass a path to a project file. Nothing to generate.") );
            return 2;
        }

        if ( hasLanguage )
        {
            if ( language.empty() )
            {
                wxLogError(_("Empty language option. Nothing generated.") );
                return 3;
            }
            language.Replace( ",", "|", true );
        }

        // generate code
        generatingCode = true;
    }

#ifdef __WXMSW__
    wxSystemOptions::SetOption( "msw.remap", 0 );
    wxSystemOptions::SetOption( "msw.staticbox.optimized-paint", 0 );
#endif

    m_frame = NULL;

    wxYield();

    m_frame = wxGUIDesigner::Get()->GetMainFrame( NULL );

    if ( !m_frame )
    {
        wxLogError(_("Error while loading the main frame.") ); return 1;
    }

    wxSetWorkingDirectory( dataDir );

    if ( generatingCode )
    {
        return 6;
    }
    else
    {
        m_frame->Show( true );
        SetTopWindow( m_frame );
    }
/*
    wxGUIDesigner::Get()->NewProject();
    PluginManager::Get()->LoadPlugins();
*/
    return wxApp::OnRun();
}

bool wxGUIDesignerApp::OnInit()
{
    // Initialization is done in OnRun, so MinGW SEH works for all code
    // (it needs a local variable, OnInit is called before OnRun)
    return true;
}

int wxGUIDesignerApp::OnExit()
{
    wxGUIDesigner::Get()->Free();

    return wxApp::OnExit();
}

wxGUIDesignerApp::~wxGUIDesignerApp()
{
}

#if wxUSE_ON_FATAL_EXCEPTION && wxUSE_STACKWALKER
    class StackLogger : public wxStackWalker
    {
    protected:
        void OnStackFrame( const wxStackFrame& frame )
        {
            // Build param string
            wxString params;
            size_t paramCount = frame.GetParamCount();
            if ( paramCount > 0 )
            {
                params << "( ";

                for ( size_t i = 0; i < paramCount; ++i )
                {
                    wxString type, name, value;
                    if ( frame.GetParam( i, &type, &name, &value) )
                    {
                        params << type << " " << name << " = " << value << wxT(", ");
                    }
                }

                params << wxT(")");
            }

            wxString source;
            if ( frame.HasSourceLocation() )
            {
                source.Printf( "%s@%i", frame.GetFileName().c_str(), frame.GetLine() );
            }

            wxLogError( "%03i %i %s %s %s %s",
                        frame.GetLevel(),
                        frame.GetAddress(),
                        frame.GetModule().c_str(),
                        frame.GetName().c_str(),
                        params.c_str(),
                        source.c_str() );
        }
    };

    void wxGUIDesignerApp::OnFatalException()
    {
        LogStack();
    }
#elif defined(_WIN32) && defined(__MINGW32__)
    static _CONTEXT* context = 0;
    EXCEPTION_DISPOSITION StructuredExceptionHandler( struct _EXCEPTION_RECORD *ExceptionRecord,
                                                      void * EstablisherFrame,
                                                      struct _CONTEXT *ContextRecord,
                                                      void * DispatcherContext )
    {
        context = ContextRecord;
        LogStack();
        return ExceptionContinueSearch;
    }

    class StackLogger
    {
    public:
        void WalkFromException()
        {
            try
            {
                std::stringstream output;
                dbg::stack s( 0, context );
                dbg::stack::const_iterator frame;
                for ( frame = s.begin(); frame != s.end(); ++frame )
                {
                    output << *frame;
                    wxLogError( wxString( output.str().c_str(), *wxConvCurrent ) );
                    output.str("");
                }
            }
            catch ( std::exception &ex )
            {
                wxLogError( wxString( ex.what(), *wxConvCurrent ) );
            }
        }
    };
#endif

#if (wxUSE_ON_FATAL_EXCEPTION && wxUSE_STACKWALKER) || (defined(_WIN32) && defined(__MINGW32__))
class LoggingStackWalker : public StackLogger
{
public:
    LoggingStackWalker()
    :
    StackLogger()
    {
        wxLog::Suspend();
    }

    ~LoggingStackWalker()
    {
        wxLogError(_("A Fatal Error Occurred. Click Details for a backtrace.") );
        wxLog::Resume();
        wxLog* logger = wxLog::GetActiveTarget();
        if ( 0 != logger )
        {
            logger->Flush();
        }
        exit(1);
    }
};

void LogStack()
{
    LoggingStackWalker walker;
    walker.WalkFromException();
}
#endif
