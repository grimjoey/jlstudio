#ifndef _JLSEDITOR_H_
#define _JLSEDITOR_H_

#include <wx/stc/stc.h>

class jlsEditor : public wxStyledTextCtrl
{
public:
	jlsEditor(wxWindow *parent, wxWindowID id, const wxString filename);
	~jlsEditor();

	bool LoadFile(const wxString &filename);

	const wxString GetFilename();
	bool HasTempFilename();

private:
	wxString m_filename;

	void OnChange(wxStyledTextEvent &event);
};

#endif // _JLSEDITOR_H_
