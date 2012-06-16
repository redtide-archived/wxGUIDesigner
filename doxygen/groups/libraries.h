///////////////////////////////////////////////////////////////////////////////
// Name:        libraries.h
// Purpose:     Application description page of the Doxygen manual
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/12/11
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
/**

@defgroup group_libraries Libraries

wxGUIDesigner comes in 3 parts:

@htmlonly
<div align="center"><img src="dotgraph.png" alt="dotgraph"/></div>
@endhtmlonly

@section page_app_main Main application

The implementation of the main core library.

Requires @ref page_libs_core and @ref page_libs_plugin

@section page_libs_core Core library

The application core is a singleton class that contains the main GUI and
the main application management.
As a library, it can be used as a plugin to be interfaced
with external applications.

Requires @ref page_libs_plugin

@section page_libs_plugin Plugin library

This library is used by wxGUIDesigner to handle both built in and external
custom plugins.
There are 2 plugin categories: wx controls and code generators.

*/
