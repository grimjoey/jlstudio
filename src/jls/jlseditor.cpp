#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/jlseditor.h"

jlsEditor::jlsEditor(wxWindow *parent, wxWindowID id, const wxString filename)
	: wxStyledTextCtrl(parent, id),
	m_filename(filename)
{
	wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
	StyleSetFont(0, font);
}

jlsEditor::~jlsEditor() {}

const wxString jlsEditor::GetFilename()
{
	return m_filename;
}

bool jlsEditor::HasTempFilename()
{
	return !wxFileExists(m_filename);
}