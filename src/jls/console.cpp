#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/defs.h"
#include "jls/console.h"
#include "jls/process.h"

wxBEGIN_EVENT_TABLE(jlsConsole, wxTextCtrl)
	EVT_KEY_DOWN(jlsConsole::OnKeyDown)
	EVT_CHAR(jlsConsole::OnChar)
	EVT_TEXT_ENTER(jlsID_CONSOLE, jlsConsole::OnTextEnter)
	EVT_IDLE(jlsConsole::OnIdle)
wxEND_EVENT_TABLE()

jlsConsole::jlsConsole(wxWindow *parent)
	: wxTextCtrl(parent, jlsID_CONSOLE, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxTE_PROCESS_ENTER | wxTE_MULTILINE | wxTE_RICH2)
{
	wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
	SetFont(font);

	//SpawnProcess();
	m_conProc = unique_ptr<jlsConsoleProc>(new jlsConsoleProc());
	m_conProc->Spawn();

	// break'able scope (... while(0))
	do {
		Sleep(1000);

		if (m_conProc->Attach(0)) {
			CONSOLE_SCREEN_BUFFER_INFOEX csbi;
			ZeroMemory(&csbi, sizeof(csbi));
			csbi.cbSize = sizeof(csbi);

			HANDLE conout = GetStdHandle(STD_OUTPUT_HANDLE);

			if (!GetConsoleScreenBufferInfoEx(
				conout,
				&csbi))
			{
				wxLogMessage("Failed to get console info (%d)", GetLastError());
				if (INVALID_HANDLE_VALUE != conout)
					CloseHandle(conout);
				break;
			}

			wxLogMessage("%d %d", csbi.dwSize.X, csbi.dwSize.Y);

			unique_ptr<CHAR_INFO> buf =
				unique_ptr<CHAR_INFO>(new CHAR_INFO[csbi.dwSize.X * csbi.dwSize.Y]);

			COORD bufCoord;
			bufCoord.X = 0;
			bufCoord.Y = 0;

			SMALL_RECT readRect;
			readRect.Left = 0;
			readRect.Top = 0;
			readRect.Right = csbi.dwSize.X;
			readRect.Bottom = csbi.dwSize.Y;

			if (!ReadConsoleOutput(
				conout,
				buf.get(),
				csbi.dwSize,
				bufCoord,
				&readRect))
			{
				wxLogMessage("Failed to read console output (%d)", GetLastError());
				if (INVALID_HANDLE_VALUE != conout)
					CloseHandle(conout);
				break;
			}

			wxLogMessage("Read %d %d %d %d",
				readRect.Left, readRect.Top,
				readRect.Right, readRect.Bottom);

			HANDLE conin = GetStdHandle(STD_INPUT_HANDLE);

			unique_ptr<INPUT_RECORD> ir =
				unique_ptr<INPUT_RECORD>(new INPUT_RECORD[6]);

			ir.get()[0].EventType = KEY_EVENT;
			ir.get()[0].Event.KeyEvent.bKeyDown = TRUE;
			ir.get()[0].Event.KeyEvent.wRepeatCount = 1;
			ir.get()[0].Event.KeyEvent.wVirtualKeyCode = 0x31;
			ir.get()[0].Event.KeyEvent.wVirtualScanCode = 0x02;
			ir.get()[0].Event.KeyEvent.uChar.UnicodeChar = L'1';
			ir.get()[0].Event.KeyEvent.dwControlKeyState = 0;

			ir.get()[1].EventType = KEY_EVENT;
			ir.get()[1].Event.KeyEvent.bKeyDown = TRUE;
			ir.get()[1].Event.KeyEvent.wRepeatCount = 1;
			ir.get()[1].Event.KeyEvent.wVirtualKeyCode = VK_SPACE;
			ir.get()[1].Event.KeyEvent.wVirtualScanCode = 0x39;
			ir.get()[1].Event.KeyEvent.uChar.UnicodeChar = L' ';
			ir.get()[1].Event.KeyEvent.dwControlKeyState = 0;

			ir.get()[2].EventType = KEY_EVENT;
			ir.get()[2].Event.KeyEvent.bKeyDown = TRUE;
			ir.get()[2].Event.KeyEvent.wRepeatCount = 1;
			ir.get()[2].Event.KeyEvent.wVirtualKeyCode = 0x31;
			ir.get()[2].Event.KeyEvent.wVirtualScanCode = 0x0d;
			ir.get()[2].Event.KeyEvent.uChar.UnicodeChar = L'+';
			ir.get()[2].Event.KeyEvent.dwControlKeyState = 0;

			ir.get()[3].EventType = KEY_EVENT;
			ir.get()[3].Event.KeyEvent.bKeyDown = TRUE;
			ir.get()[3].Event.KeyEvent.wRepeatCount = 1;
			ir.get()[3].Event.KeyEvent.wVirtualKeyCode = VK_SPACE;
			ir.get()[3].Event.KeyEvent.wVirtualScanCode = 0x39;
			ir.get()[3].Event.KeyEvent.uChar.UnicodeChar = L' ';
			ir.get()[3].Event.KeyEvent.dwControlKeyState = 0;

			ir.get()[4].EventType = KEY_EVENT;
			ir.get()[4].Event.KeyEvent.bKeyDown = TRUE;
			ir.get()[4].Event.KeyEvent.wRepeatCount = 1;
			ir.get()[4].Event.KeyEvent.wVirtualKeyCode = 0x31;
			ir.get()[4].Event.KeyEvent.wVirtualScanCode = 0x02;
			ir.get()[4].Event.KeyEvent.uChar.UnicodeChar = L'1';
			ir.get()[4].Event.KeyEvent.dwControlKeyState = 0;

			ir.get()[5].EventType = KEY_EVENT;
			ir.get()[5].Event.KeyEvent.bKeyDown = TRUE;
			ir.get()[5].Event.KeyEvent.wRepeatCount = 1;
			ir.get()[5].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
			ir.get()[5].Event.KeyEvent.wVirtualScanCode = 0x1C;
			ir.get()[5].Event.KeyEvent.uChar.UnicodeChar = L'\n';
			ir.get()[5].Event.KeyEvent.dwControlKeyState = 0;

			DWORD evtsWritten = 0;

			WriteConsoleInput(
				conin,
				ir.get(),
				6,
				&evtsWritten);

			CloseHandle(conin);
			CloseHandle(conout);
		}
	} while (0);
}

jlsConsole::~jlsConsole() {}

void jlsConsole::Execute(const wxString &command) {}

/* static */
void jlsConsole::OnReadStdout(void *buffer, size_t n, void *receiver) {
	static_cast<jlsConsole*>(receiver)->OnReadStdout(buffer, n);
}

/* static */
void jlsConsole::OnReadStderr(void *buffer, size_t n, void *receiver) {
	static_cast<jlsConsole*>(receiver)->OnReadStderr(buffer, n);
}

void jlsConsole::OnReadStdout(void *buffer, size_t n) {
	char *buf = (char*)buffer;
	wxString s(buf, buf + n);
	AppendText(s);
}

void jlsConsole::OnReadStderr(void *buffer, size_t n) {
	char *buf = (char*)buffer;
	wxString s(buf, buf + n);
	AppendText(s);
}

void jlsConsole::SpawnProcess()
{
	m_process = unique_ptr<jlsProcess>(
		new jlsProcess(
			&jlsConsole::OnReadStdout,
			&jlsConsole::OnReadStderr,
			static_cast<void*>(this)));

	//TCHAR path[] = _T("C:\\Windows\\System32\\cmd.exe");
	//TCHAR path[] = _T("C:\\Python27\\python.exe");
	TCHAR path[] = _T("C:\\Users\\grimjoey\\AppData\\Local\\Julia-0.6.2\\bin\\julia.exe");


	m_process->Execute(path);
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

void jlsConsole::OnChar(wxKeyEvent &event)
{
	wxChar c = event.GetUnicodeKey();

	if (c == WXK_BACK) {
		m_cmdBuffer.RemoveLast(1);
	}
	else {
		if (c == '\r')
			m_cmdBuffer << '\n';
		else
			m_cmdBuffer << c;
	}

	event.Skip();
}

void jlsConsole::OnTextEnter(wxCommandEvent &event)
{
	wxScopedCharBuffer buf(m_cmdBuffer.ToAscii());
	m_process->Write(buf.data(), buf.length());
	m_cmdBuffer.clear();
}

void jlsConsole::OnIdle(wxIdleEvent &event)
{
	if (m_process)
		m_process->HighjackEventLoop();
}