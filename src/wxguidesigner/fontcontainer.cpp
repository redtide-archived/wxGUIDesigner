///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/fontcontainer.cpp
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/07/15
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
#include "wxguidesigner/fontcontainer.h"

wxFontContainer::wxFontContainer()
{
    Init();
}

wxFontContainer::wxFontContainer( const wxFont &font )
{
    if( !font.IsOk() )
    {
        Init();
    }
    else
    {
        m_face       = font.GetFaceName();
        m_family     = font.GetFamily();
        m_size       = font.GetPointSize();
        m_style      = font.GetStyle();
        m_weight     = font.GetWeight();
        m_underlined = font.GetUnderlined();
        m_encoding   = font.GetEncoding();
    }
}

wxFontContainer::wxFontContainer  ( int size, int family, int style, int weight,
                                    bool underlined, const wxString &face,
                                    wxFontEncoding encoding )
:
m_size      ( size ),
m_family    ( family ),
m_style     ( style ),
m_weight    ( weight ),
m_underlined( underlined ),
m_face      ( face ),
m_encoding  ( encoding )
{
}

wxFontContainer::~wxFontContainer()
{
}

void wxFontContainer::Init()
{
    m_size       = -1;
    m_family     = wxFONTFAMILY_DEFAULT;
    m_style      = wxFONTSTYLE_NORMAL;
    m_weight     = wxFONTWEIGHT_NORMAL;
    m_underlined = false;
    m_face       = wxEmptyString;
    m_encoding   = wxFONTENCODING_DEFAULT;
}

wxFont wxFontContainer::GetAsFont() const
{
    int size = m_size <= 0 ? wxNORMAL_FONT->GetPointSize() : m_size;
    return
    wxFont( size, m_family, m_style, m_weight, m_underlined, m_face,
            (wxFontEncoding)m_encoding );
}
//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------
int wxFontContainer::GetPointSize() const
{
    return m_size;
}

int wxFontContainer::GetFamily() const
{
    return m_family;
}

int wxFontContainer::GetStyle() const
{
    return m_style;
}

int wxFontContainer::GetWeight() const
{
    return m_weight;
}

bool wxFontContainer::GetUnderlined() const
{
    return m_underlined;
}

wxString wxFontContainer::GetFaceName() const
{
    return m_face;
}

int wxFontContainer::GetEncoding() const
{
    return m_encoding;
}
//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------
void wxFontContainer::SetPointSize( int pointSize )
{
    m_size = pointSize;
}

void wxFontContainer::SetFamily( int family )
{
    m_family = family;
}

void wxFontContainer::SetStyle( int style )
{
    m_style = style;
}

void wxFontContainer::SetWeight( int weight )
{
    m_weight = weight;
}

void wxFontContainer::SetUnderlined( bool underlined )
{
    m_underlined = underlined;
}

void wxFontContainer::SetFaceName( const wxString &faceName )
{
    m_face = faceName;
}

void wxFontContainer::SetEncoding( int encoding )
{
    m_encoding = encoding;
}
