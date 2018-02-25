#ifndef _JLSAPPRAME_H_
#define _JLSAPPRAME_H_

#include <wx/frame.h>

#include "jls/tabbededitor.h"
#include "jls/preferences.h"
#include "jls/console.h"

class jlsAppFrame : public wxFrame
{
	friend class jlsPreferences;

public:
	jlsAppFrame();
	~jlsAppFrame();

	const jlsConsole *GetConsole() const;

	void OnExit(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event);

	void OnFileNew(wxCommandEvent &event);
	void OnFileOpen(wxCommandEvent &event);
	void OnFileSave(wxCommandEvent &event);
	void OnFileSaveAs(wxCommandEvent &event);
	void OnFileSaveAll(wxCommandEvent &event);
	void OnFileClose(wxCommandEvent &event);

	void OnPreferences(wxCommandEvent &event);

	void OnRun(wxCommandEvent &event);

private:
	jlsTabbedEditor *m_tabbedEditor;
	jlsConsole *m_console;
	wxString m_juliaPath;

	void CreateMenu();

	wxDECLARE_EVENT_TABLE();
};

#endif // _JLSAPPRAME_H_