///////////////////////////////////////////////////////////////////////////////
// Name:        core/handlers/editor.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/11/30
// RCS-ID:      $Id$
// Copyright:   (c) Andrea Zanellato
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "handlers/editor.h"
#include "manager.h"

EditorHandler::EditorHandler( wxNotebook *owner )
{
    m_editor = owner;
}
