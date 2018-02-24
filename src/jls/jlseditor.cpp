#include "jls/jlseditor.h"

jlsEditor::jlsEditor(wxWindow *parent, wxWindowID id, const wxString filename)
	: wxStyledTextCtrl(parent, id),
	m_filename(filename)
{
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