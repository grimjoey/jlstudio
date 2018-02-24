#ifndef _JLSCONSOLE_H_
#define _JLSCONSOLE_H_

#include <wx/textctrl.h>
#include <wx/process.h>
#include <wx/timer.h>

class jlsConsole : public wxTextCtrl
{
public:
	jlsConsole(wxWindow *parent);
	~jlsConsole();

	bool CanExecute() const;
	void Execute(const wxString &command);

private:
	wxProcess *m_process = nullptr;
	long m_pid = 0;
	bool m_running = false;
	wxTimer m_pollProcessOutputTimer;
	int m_pollCounter = 0;

	void OnTimer(wxTimerEvent &event);
	void OnEndProcess(wxProcessEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnTextEnter(wxCommandEvent &event);
	wxDECLARE_EVENT_TABLE();
};

#endif // _JLSCONSOLE_H_
