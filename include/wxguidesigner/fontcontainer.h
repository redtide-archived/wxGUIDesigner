///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/fontcontainer.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/07/15
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGUIDESIGNER_FONTCONTAINER_H__
#define __WXGUIDESIGNER_FONTCONTAINER_H__

#include <wx/font.h>

class wxFontContainer : public wxObject
{
public:
    wxFontContainer();
    wxFontContainer( const wxFont &font );
    wxFontContainer(int             size,
                    int             family      = wxFONTFAMILY_DEFAULT,
                    int             style       = wxFONTSTYLE_NORMAL,
                    int             weight      = wxFONTWEIGHT_NORMAL,
                    bool            underlined  = false,
                    const wxString &face        = wxEmptyString,
                    wxFontEncoding  encoding    = wxFONTENCODING_DEFAULT );
    ~wxFontContainer();

    wxFont          GetAsFont()     const;

    int             GetPointSize()  const;
    int             GetFamily()     const;
    int             GetStyle()      const;
    int             GetWeight()     const;
    bool            GetUnderlined() const;
    wxString        GetFaceName()   const;
    int             GetEncoding()   const;

    void            SetPointSize    ( int size );
    void            SetFamily       ( int family );
    void            SetStyle        ( int style );
    void            SetWeight       ( int weight );
    void            SetUnderlined   ( bool underlined );
    void            SetFaceName     ( const wxString &face );
    void            SetEncoding     ( int encoding );

    operator wxFont() const { return GetAsFont(); }

private:
    void            Init();

    int             m_size;
    int             m_family;
    int             m_style;
    int             m_weight;
    bool            m_underlined;
    wxString        m_face;
    int             m_encoding;
};

#endif //__WXGUIDESIGNER_FONTCONTAINER_H__
