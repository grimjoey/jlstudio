#ifndef _JLSPREFERENCES_H_
#define _JLSPREFERENCES_H_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class jlsPreferences : public wxDialog
{
public:
	jlsPreferences(wxWindow *parent, wxWindowID id);
	~jlsPreferences();

private:
	wxTextCtrl *m_juliaPathControl;

	void OnStartJulia(wxCommandEvent &event);
};

#endif // _JLSPREFERENCES_H_