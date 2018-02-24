#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/jlsdefs.h"
#include "jls/jlspreferences.h"
#include "jls/jlsframe.h"
#include "jls/jlsconsole.h"

jlsPreferences::jlsPreferences(wxWindow *parent, wxWindowID id)
	: wxDialog(parent, id, _("Preferences"))
{
	// TODO(joare): Avoid circular dependency.
	
	wxString juliaPath = static_cast<jlsFrame*>(parent)->m_juliaPath;

	m_juliaPathControl = new wxTextCtrl(this, wxID_ANY, juliaPath, wxDefaultPosition, wxSize(200, wxDefaultCoord));

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(10, 0);
	sizer->Add(m_juliaPathControl,
		0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	sizer->Add(new wxButton(this, jlsID_PREFS_START_JL, _("Start julia")),
		0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

	Bind(wxEVT_BUTTON, &jlsPreferences::OnStartJulia, this);

	SetSizerAndFit(sizer);
}

jlsPreferences::~jlsPreferences() {}

void jlsPreferences::OnStartJulia(wxCommandEvent &event)
{
	// TODO(joare): Avoid circular dependency.
	
	jlsFrame *p = static_cast<jlsFrame*>(GetParent());
	p->m_juliaPath = m_juliaPathControl->GetValue();
	if (wxFileExists(p->m_juliaPath))
		p->m_console->Execute(p->m_juliaPath);

	if (p->m_console->IsRunning())
		wxMessageBox("Julia is running!");
	else
		wxMessageBox("Julia failed to run (check path)!");
}