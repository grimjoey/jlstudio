#ifndef _JLSFRAME_H_
#define _JLSFRAME_H_

#include <wx/frame.h>

#include "jlstabbededitor.h"

class jlsFrame : public wxFrame
{
public:
	jlsFrame();
	~jlsFrame();

	void OnExit(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event);

	void OnFileNew(wxCommandEvent &event);
	void OnFileOpen(wxCommandEvent &event);
	void OnFileSave(wxCommandEvent &event);
	void OnFileSaveAs(wxCommandEvent &event);
	void OnFileSaveAll(wxCommandEvent &event);
	void OnFileClose(wxCommandEvent &event);

private:
	jlsTabbedEditor *m_tabbedEditor;

	void CreateMenu();

	wxDECLARE_EVENT_TABLE();
};

#endif // _JLSFRAME_H_