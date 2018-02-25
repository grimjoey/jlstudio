#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <strsafe.h>

#include "jls/process.h"

#define PIPE_NAME_MAX 256

jlsProcess::jlsProcess() {}

jlsProcess::~jlsProcess() {
	// TODO: kill process

	TerminateHandle(&m_remote_io);
	TerminateHandle(&m_remote_err);
	TerminateHandle(&m_io);
	TerminateHandle(&m_err);
}

bool jlsProcess::Execute(LPTSTR cmd)
{
	m_cmd = cmd;

	if (!MakePipes() || !MakeProcess())
		return false;

	ReadFileEx(m_io, m_readStdinBuf, 4096, &m_readStdinOverlapped, &jlsProcess::OnReadStdin);
	ReadFileEx(m_err, m_readStderrBuf, 4096, &m_readStderrOverlapped, &jlsProcess::OnReadStderr);

	// TODO(joare): Implement callbacks and call read again from them
	//              with 'this' pointer attached to a derived OVERLAPPED
	//              struct.
	//              Also implement non-static callbacks and call them
	//              via 'this'.

	return true;
}

void jlsProcess::Write() {}

VOID WINAPI jlsProcess::OnReadStdin(
	DWORD dwError,
	DWORD dwNBytes,
	LPOVERLAPPED lpOverlapped)
{

}

VOID WINAPI jlsProcess::OnReadStderr(
	DWORD dwError,
	DWORD dwNBytes,
	LPOVERLAPPED lpOverlapped)
{

}

bool jlsProcess::MakePipes()
{
	static ULONG pipeSerial = 0;

	// Buffer size from msdn:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365150(v=vs.85).aspx
	TCHAR pipeNameBuf[PIPE_NAME_MAX];
	StringCbPrintf(
		pipeNameBuf,
		PIPE_NAME_MAX * sizeof(TCHAR),
		_T("\\\\.\\Pipe\\RemoteExeAnon.%08x.%08x"),
		GetCurrentProcessId(),
		InterlockedIncrement(&pipeSerial));

	m_io = CreateNamedPipe(
		pipeNameBuf,
		PIPE_ACCESS_DUPLEX,
		FILE_FLAG_OVERLAPPED,
		1,
		4096,
		4096,
		5000,
		NULL);

	if (INVALID_HANDLE_VALUE == m_io)
		return false;

	SECURITY_ATTRIBUTES securityAttribs;
	ZeroMemory(&securityAttribs, sizeof(securityAttribs));
	securityAttribs.bInheritHandle = TRUE;
	
	m_remote_io = CreateFile(
		pipeNameBuf,
		GENERIC_READ | GENERIC_WRITE,
		0,
		&securityAttribs,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == m_remote_io) {
		TerminateHandle(&m_io);
		return false;
	}

	StringCbPrintf(
		pipeNameBuf,
		PIPE_NAME_MAX * sizeof(TCHAR),
		_T("\\\\.\\Pipe\\RemoteExeAnon.%08x.%08x"),
		GetCurrentProcessId(),
		InterlockedIncrement(&pipeSerial));

	m_err = CreateNamedPipe(
		pipeNameBuf,
		PIPE_ACCESS_INBOUND,
		FILE_FLAG_OVERLAPPED,
		1,
		4096,
		4096,
		5000,
		NULL);

	if (INVALID_HANDLE_VALUE == m_err) {
		TerminateHandle(&m_io);
		TerminateHandle(&m_remote_io);
		return false;
	}

	m_remote_err = CreateFile(
		pipeNameBuf,
		GENERIC_WRITE,
		0,
		&securityAttribs,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == m_remote_err) {
		TerminateHandle(&m_io);
		TerminateHandle(&m_remote_io);
		TerminateHandle(&m_err);
		return false;
	}

	return true;
}

bool jlsProcess::MakeProcess()
{
	STARTUPINFO si;

	ZeroMemory(&m_procInfo, sizeof(m_procInfo));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = m_remote_io;
	si.hStdOutput = m_remote_io;
	si.hStdError = m_remote_err;

	return CreateProcess(
		NULL, m_cmd, NULL, NULL,
		TRUE, 0, NULL, NULL,
		&si, &m_procInfo);
}

/* static */
void jlsProcess::TerminateHandle(LPHANDLE handle)
{
	if (INVALID_HANDLE_VALUE != *handle) {
		CloseHandle(*handle);
		*handle = INVALID_HANDLE_VALUE;
	}
}
