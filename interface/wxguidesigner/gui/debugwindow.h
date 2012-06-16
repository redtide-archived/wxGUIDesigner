///////////////////////////////////////////////////////////////////////////////
// Name:        wxguidesigner/gui/debugwindow.h
// Purpose:     
// Author:      Jefferson González
// Modified by:
// Created:     2012/06/07
// Revision:    $Hash$
// Copyleft:    (ɔ) Andrea Zanellato
// Licence:     GNU General Public License Version 3
///////////////////////////////////////////////////////////////////////////////
/**
 * Custom wxTectCtrl that serves as a log for 3 kinds of messages:
 * normal, warning, errors.
 */
class TextLogCtrl : public wxTextCtrl
{
public:
    TextLogCtrl(wxWindow* parent);
    virtual ~TextLogCtrl();

    /**
     * Appends normal message with prefix "Ok: " in green colour.
     * @param message String to append.
     */
    void AddMessage(const wxString& message);

    /**
     * Appends warning message with prefix "Warning: " in yellow colour.
     * @param message String to append.
     */
    void AddWarning(const wxString& message);

    /**
     * Appends error message with prefix "Error: " in red colour.
     * @param message String to append.
     */
    void AddError(const wxString& message);
};
