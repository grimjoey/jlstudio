#ifndef _JLSTABBEDEDITOR_H_
#define _JLSTABBEDEDITOR_H_

#include <wx/notebook.h>

class jlsTabbedEditor : public wxNotebook
{
public:
	jlsTabbedEditor(wxWindow *parent, wxWindowID id);
	~jlsTabbedEditor();

	void NewFile();
	bool LoadFile();
	void SaveFile();
	void SaveFileAs();
	void SaveAll();

private:
	const wxString GetNextTempFilename();
};

#endif // _JLSTABBEDEDITOR_H_
