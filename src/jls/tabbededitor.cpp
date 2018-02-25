#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/tabbededitor.h"
#include "jls/editor.h"

jlsTabbedEditor::jlsTabbedEditor(wxWindow *parent, wxWindowID id)
	: wxAuiNotebook(parent, id, wxDefaultPosition, wxDefaultSize,
		wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE
		| wxAUI_NB_TAB_FIXED_WIDTH | wxAUI_NB_CLOSE_ON_ALL_TABS)
{
	NewFile();
}

jlsTabbedEditor::~jlsTabbedEditor() {}

void jlsTabbedEditor::NewFile()
{
	wxString filename = GetNextTempFilename();
	AddPage(new jlsEditor(this, wxID_ANY, filename), filename, true);
}

bool jlsTabbedEditor::LoadFile()
{
	wxFileDialog dlg(this, wxT("Open file"), wxEmptyString, wxEmptyString,
		wxT("Any file (*)|*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
	if (dlg.ShowModal() != wxID_OK) return false;
	wxString filename = dlg.GetPath();

	jlsEditor *edit = new jlsEditor(this, wxID_ANY, filename);
	if (!edit->LoadFile(filename)) {
		edit->Destroy();
		return false;
	}

	AddPage(edit, wxFileNameFromPath(filename), true);
	return true;
}

void jlsTabbedEditor::SaveFile()
{
}

void jlsTabbedEditor::SaveFileAs()
{
}

void jlsTabbedEditor::SaveAll()
{
}

const wxString jlsTabbedEditor::GetNextTempFilename()
{
	int nextTempFilenameNum = 0;
	for (size_t i = 0; i < GetPageCount(); ++i) {
		jlsEditor *e = static_cast<jlsEditor*>(GetPage(i));
		if (e->HasTempFilename()) {
			wxString filename = e->GetFilename();
			filename.Remove(0, 8);
			long n = 0;
			if (filename.ToLong(&n) && n >= nextTempFilenameNum)
				nextTempFilenameNum = n + 1;
		}
	}
	return wxString::Format("untitled%d", nextTempFilenameNum);
}