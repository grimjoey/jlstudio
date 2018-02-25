#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/txtstrm.h>

#include "jls/defs.h"
#include "jls/console.h"
#include "jls/process.h"

wxBEGIN_EVENT_TABLE(jlsConsole, wxTextCtrl)
	EVT_KEY_DOWN(jlsConsole::OnKeyDown)
	EVT_TEXT_ENTER(jlsID_CONSOLE, jlsConsole::OnTextEnter)
	EVT_IDLE(jlsConsole::OnIdle)
wxEND_EVENT_TABLE()

jlsConsole::jlsConsole(wxWindow *parent)
	: wxTextCtrl(parent, jlsID_CONSOLE, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxTE_PROCESS_ENTER | wxTE_MULTILINE | wxTE_RICH2)
{
	m_process = new jlsProcess(&jlsConsole::OnReadStdout, &jlsConsole::OnReadStderr, (void*)this);
	
	TCHAR path[] = L"C:\\Windows\\System32\\cmd.exe";
	m_process->Execute(path);

	wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
	SetFont(font);
}

jlsConsole::~jlsConsole() {
	if (nullptr != m_process)
		delete m_process;
}

void jlsConsole::Execute(const wxString &command)
{
}

/* static */
void jlsConsole::OnReadStdout(void *buffer, size_t n, void *receiver) {
	// TODO(joare): Find better way to do this
	((jlsConsole*)receiver)->OnReadStdout(buffer, n);
}

/* static */
void jlsConsole::OnReadStderr(void *buffer, size_t n, void *receiver) {
	// TODO(joare): Find better way to do this
	((jlsConsole*)receiver)->OnReadStderr(buffer, n);
}

void jlsConsole::OnReadStdout(void *buffer, size_t n) {
	char *buf = (char*)buffer;
	wxString s(buf, buf + n);
	wxLogMessage(wxString("out: ") << s);
}

void jlsConsole::OnReadStderr(void *buffer, size_t n) {
	char *buf = (char*)buffer;
	wxString s(buf, buf + n);
	wxLogMessage(wxString("err: ") << s);
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
}

void jlsConsole::OnIdle(wxIdleEvent &event)
{
	m_process->WaitForObjectsOrMsg();
}