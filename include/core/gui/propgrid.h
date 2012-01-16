///////////////////////////////////////////////////////////////////////////////
// Name:        core/gui/propgrid.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by:
// Created:     2011/12/10
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXGDCORE_PROPGRID_H__
#define __WXGDCORE_PROPGRID_H__

class wxNotebook;
class wxSizeEvent;

class PropBookHandler
{
    friend class GUIManager;

public:
    PropBookHandler( wxNotebook *owner ) : m_propBook( owner ) {}

private:
    void OnSize( wxSizeEvent &event );

    wxNotebook *m_propBook;
};

#endif //__WXGDCORE_PROPGRID_H__
