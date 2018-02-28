#include <windows.h>
#include <tchar.h>

#include "jls\consoleproc.h"

jlsConsoleProc::jlsConsoleProc() {}

jlsConsoleProc::~jlsConsoleProc() {
	while (m_procInfos.size()) {
		PROCESS_INFORMATION pi = m_procInfos.back();
		TerminateProcess(pi.hProcess, 1);
		m_procInfos.pop_back();
	}
}

void jlsConsoleProc::Spawn()
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	//TCHAR cmd[] = _T("C:\\Windows\\System32\\cmd.exe");
	//TCHAR cmd[] = _T("C:\\Python34\\python.exe");
	TCHAR cmd[] = _T("C:\\Users\\grimjoey\\AppData\\Local\\Julia-0.6.2\\bin\\julia.exe");

	CreateProcess(
		NULL,
		cmd,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);

	m_procInfos.push_back(pi);
}

const PROCESS_INFORMATION *jlsConsoleProc::GetPInfo(size_t n) const
{
	if (n < m_procInfos.size())
		return &m_procInfos[n];
	else
		return nullptr;
}

bool jlsConsoleProc::Attach(size_t n)
{
	if (n >= m_procInfos.size())
		return false;

	// FIXME: Can't attach right away. Waiting seems to work,
	//        but is wasteful and probably non-deterministic.
	Sleep(100);

	return AttachConsole(m_procInfos[n].dwProcessId);
}