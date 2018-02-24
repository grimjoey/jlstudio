#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/process.h>
#include <wx/txtstrm.h>

#include "jls/jlsdefs.h"
#include "jls/jlsconsole.h"

wxBEGIN_EVENT_TABLE(jlsConsole, wxTextCtrl)
	EVT_TIMER(jlsID_CONSOLE_TIMER, jlsConsole::OnTimer)
	EVT_END_PROCESS(jlsID_CONSOLE_PROCESS, jlsConsole::OnEndProcess)
	EVT_KEY_DOWN(jlsConsole::OnKeyDown)
	EVT_TEXT_ENTER(jlsID_CONSOLE, jlsConsole::OnTextEnter)
wxEND_EVENT_TABLE()

jlsConsole::jlsConsole(wxWindow *parent)
	: wxTextCtrl(parent, jlsID_CONSOLE, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxTE_PROCESS_ENTER | wxTE_MULTILINE | wxTE_RICH2),
	m_pollProcessOutputTimer(this, jlsID_CONSOLE_TIMER)
{
	wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
	SetFont(font);
}

jlsConsole::~jlsConsole() {
	if (m_process != nullptr) {
		if (m_running && m_pid)
			m_process->Kill(m_pid);
		delete m_process;
	}
}

bool jlsConsole::CanExecute() const
{
	return m_process == nullptr;
}

bool jlsConsole::IsRunning() const
{
	return m_running;
}

void jlsConsole::Execute(const wxString &command)
{
	if (CanExecute()) {
		m_process = new wxProcess(this, jlsID_CONSOLE_PROCESS);
		m_process->Redirect();
		m_pid = wxExecute(command, wxEXEC_ASYNC, m_process);

		if (m_pid == 0) {
			delete m_process;
			m_process = nullptr;
		}
		else {
			m_running = true;
		}
	}
}

void jlsConsole::OnTimer(wxTimerEvent &event)
{
	++m_pollCounter;

	if (m_process == nullptr) return;

	if (m_process->IsErrorAvailable() || m_process->IsInputAvailable()) {
		wxInputStream *es = m_process->GetErrorStream();
		if (es != nullptr) {
			wxTextInputStream tes(*es);

			while (m_process->IsErrorAvailable())
				AppendText(tes.ReadLine() << "\n");
		}

		wxInputStream *is = m_process->GetInputStream();
		if (is != nullptr) {
			wxTextInputStream tis(*is);

			while (m_process->IsInputAvailable())
				AppendText(tis.ReadLine() << "\n");
		}

		m_pollCounter = 0;
	}

	if (m_pollCounter >= jlsCONSOLE_POLL_PROCESS_OUTPUT_FREQUENCY_HZ * jlsCONSOLE_POLL_PROCESS_OUTPUT_TIMEOUT_SEC) {
		m_pollProcessOutputTimer.Stop();
		m_pollCounter = 0;
	}
}

void jlsConsole::OnEndProcess(wxProcessEvent &event)
{
	delete m_process;
	m_process = nullptr;
	m_pid = 0;
	m_running = false;

	wxLogMessage("Process %d terminated with code: %d", event.GetPid(), event.GetExitCode());
}

void jlsConsole::OnKeyDown(wxKeyEvent &event)
{
	if (m_process == nullptr)
		return;

	switch (event.GetKeyCode())
	{
		case WXK_UP:
		case WXK_DOWN:
			break;
		default:
			event.Skip();
	}
}


void jlsConsole::OnTextEnter(wxCommandEvent &event)
{
	if (m_process == nullptr) return;

	wxOutputStream *os = m_process->GetOutputStream();
	if (os == nullptr) return;

	wxTextOutputStream tos(*os);
	tos.WriteString(GetLineText(GetNumberOfLines() - 2) << '\n');

	m_pollProcessOutputTimer.Start(1000 / jlsCONSOLE_POLL_PROCESS_OUTPUT_FREQUENCY_HZ);
}