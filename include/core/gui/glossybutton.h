///////////////////////////////////////////////////////////////////////////////
// Name:        glossybutton.h
// Purpose:     
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2012/06/03
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXGLOSSYBUTTON_H__
#define __WXGLOSSYBUTTON_H__

#include <wx/settings.h>

namespace DrawUtils
{
    const bool IsDark( const wxColour &colour );

    void DrawGlossyGradient( wxDC &dc, wxRect &rect, wxColour &topStart,
                             wxColour &bottomStart,  wxColour &bottomEnd,
                             wxColour &colour, bool hover = false );

    void DrawBitmap( wxDC &dc, const wxBitmap &bitmap, const wxRect &rect,
                    const wxString &text );
};

class wxGlossyButton : public wxWindow
{    
public:
    wxGlossyButton()    { Init(); }
    ~wxGlossyButton()   {}

    wxGlossyButton( wxWindow* parent, wxWindowID id = wxID_ANY,
                    const wxString &label = wxEmptyString,
                    const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize,
                    long style = wxSIMPLE_BORDER|wxFULL_REPAINT_ON_RESIZE )
    {
        Init();
        Create( parent, id, wxNullBitmap, label, pos, size, style );
    }

    wxGlossyButton( wxWindow* parent, wxWindowID id = wxID_ANY,
                    const wxBitmap &bitmap = wxNullBitmap,
                    const wxString &label = wxEmptyString,
                    const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize,
                    long style = wxSIMPLE_BORDER|wxFULL_REPAINT_ON_RESIZE )
    {
        Init();
        Create( parent, id, bitmap, label, pos, size, style );
    }

    bool Create( wxWindow* parent, wxWindowID id = wxID_ANY,
                    const wxBitmap &bitmap = wxNullBitmap,
                    const wxString &label = wxEmptyString,
                    const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize,
                    long style = wxSIMPLE_BORDER|wxFULL_REPAINT_ON_RESIZE )
    {
        wxWindow::Create( parent, id, pos, size, style );

        SetBitmap( bitmap );
        SetLabel( label );

        return true;
    }

    wxBitmap    GetBitmap() const;
    void        SetBitmap( const wxBitmap &bitmap );

    wxString    GetLabel() const;
    void        SetLabel( const wxString &label );

    wxColour    GetGradientColourTopStart()    const;
    wxColour    GetGradientColourTopEnd()      const;
    wxColour    GetGradientColourBottomStart() const;
    wxColour    GetGradientColourBottomEnd()   const;
    wxColour    GetColourPressedTop()          const;
    wxColour    GetColourPressedBottom()       const;

    void        SetGradientColourTopStart      ( const wxColour &colour );
    void        SetGradientColourTopEnd        ( const wxColour &colour );
    void        SetGradientColourBottomStart   ( const wxColour &colour );
    void        SetGradientColourBottomEnd     ( const wxColour &colour );
    void        SetColourPressedTop            ( const wxColour &colour );
    void        SetColourPressedBottom         ( const wxColour &colour );

private:
    void        OnPaint             ( wxPaintEvent  &event );
    void        OnEraseBackground   ( wxEraseEvent  &event );
    void        OnHover             ( wxMouseEvent  &event );
    void        OnLeftDown          ( wxMouseEvent  &event );
    void        OnLeftUp            ( wxMouseEvent  &event );

    void        Init();

    wxSize      DoGetBestSize() const;

    wxBitmap m_bitmap;
    wxString m_label;
    wxColour m_topStart;
    wxColour m_topEnd;
    wxColour m_topPressed;
    wxColour m_bottomStart;
    wxColour m_bottomEnd;
    wxColour m_bottomPressed;
    bool     m_hover;

    DECLARE_DYNAMIC_CLASS( wxGlossyButton )
    DECLARE_EVENT_TABLE();
};

#endif //__WXGLOSSYBUTTON_H__
