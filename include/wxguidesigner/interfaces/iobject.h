///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/interfaces/iobject.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/01/06
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_INTERFACES_IOBJECT_H__
#define __WXGUIDESIGNER_INTERFACES_IOBJECT_H__

#include "wxguidesigner/defs.h"

class wxString;

class wxWindow;
class wxDialog;
class wxMenuBar;
class wxNotebook;
class wxFrame;
class wxPanel;
class wxTreeCtrl;

class IGUIDesigner
{
public:
    IGUIDesigner() {}
    virtual ~IGUIDesigner() {}
    virtual wxDialog        *GetAboutDialog      ( wxWindow * ) = 0;
    virtual wxNotebook      *GetEditorBook       ( wxWindow * ) = 0;
    virtual wxNotebook      *GetBook     ( wxWindow * ) = 0;
    virtual wxNotebook      *GetPaletteBook      ( wxWindow * ) = 0;
    virtual wxTreeCtrl      *GetTreeView         ( wxWindow * ) = 0;
};
//=============================================================================
// IObject Interface
//=============================================================================
class IObject
{
public:
    IObject() {}
    virtual ~IObject() {}

    virtual wxString GetClassName()   const = 0;
    virtual wxString GetDescription() const = 0;
//  virtual wxString GetName()        const = 0;
};
//=============================================================================
// IObjectManager Interface
//=============================================================================
class IObjectManager
{
public:
    virtual Object  CreateObject   ( const wxString &, Object ) = 0;
    virtual Object  GetSelectObject() const = 0;

    virtual void    SelectObject   ( Object, bool ) = 0;

    virtual bool    Load           ( const wxString & ) = 0;
    virtual bool    Serialize      ( const wxString & ) = 0;

protected:
    IObjectManager() {}
    virtual ~IObjectManager() {}
};

#endif //__WXGUIDESIGNER_INTERFACES_IOBJECT_H__
