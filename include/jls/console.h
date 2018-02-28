#ifndef _JLSCONSOLE_H_
#define _JLSCONSOLE_H_

#include <memory>

#include <wx/textctrl.h>
#include <wx/process.h>
#include <wx/timer.h>

#include "jls/process.h"
#include "jls/consoleproc.h"

using namespace std;

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
	unique_ptr<jlsProcess> m_process;
	wxString m_cmdBuffer;

	unique_ptr<jlsConsoleProc> m_conProc;

	void SpawnProcess();

	void OnKeyDown(wxKeyEvent &event);
	void OnChar(wxKeyEvent &event);
	void OnTextEnter(wxCommandEvent &event);
	void OnIdle(wxIdleEvent &event);
	wxDECLARE_EVENT_TABLE();
};

#endif // _JLSCONSOLE_H_
