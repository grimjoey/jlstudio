#ifndef _JLSCONSOLE_H_
#define _JLSCONSOLE_H_

#include <wx/textctrl.h>
#include <wx/process.h>
#include <wx/timer.h>

#include "jls/process.h"

class jlsConsole : public wxTextCtrl
{
public:
	jlsConsole(wxWindow *parent);
	~jlsConsole();

	void Execute(const wxString &command);

	// Note(joare): Not sure if i can pass these as callbacks.
	static void OnReadStdout(void *buffer, size_t n, void *receiver);
	static void OnReadStderr(void *buffer, size_t n, void *receiver);

	void OnReadStdout(void *buffer, size_t n);
	void OnReadStderr(void *buffer, size_t n);

private:
	jlsProcess *m_process;

	void OnKeyDown(wxKeyEvent &event);
	void OnTextEnter(wxCommandEvent &event);
	void OnIdle(wxIdleEvent &event);
	wxDECLARE_EVENT_TABLE();
};

#endif // _JLSCONSOLE_H_
