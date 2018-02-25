#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/editor.h"

jlsEditor::jlsEditor(wxWindow *parent, wxWindowID id, const wxString filename)
	: wxStyledTextCtrl(parent, id),
	m_filename(filename)
{
	wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
	StyleSetFont(0, font);

	Bind(wxEVT_STC_CHANGE, &jlsEditor::OnChange, this);
}

jlsEditor::~jlsEditor() {}

bool jlsEditor::LoadFile(const wxString &filename)
{
	// Load the actual file.
	bool result = wxStyledTextCtrl::LoadFile(filename);

	// TODO(joare): Keep track of long lines, and adjust scrollwidth.

	return result;
}

const wxString jlsEditor::GetFilename()
{
	return m_filename;
}

bool jlsEditor::HasTempFilename()
{
	return !wxFileExists(m_filename);
}

void jlsEditor::OnChange(wxStyledTextEvent &event)
{
}