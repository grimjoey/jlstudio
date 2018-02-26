#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <strsafe.h>

#include "jls/process.h"

jlsProcess::jlsProcess(jlsProcessCb out, jlsProcessCb err, void *receiver)
	: m_cb_out(out), m_cb_err(err), m_receiver(receiver),
	m_remote_io(INVALID_HANDLE_VALUE),
	m_remote_err(INVALID_HANDLE_VALUE),
	m_handles{INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE}
{
	ZeroMemory(&m_readStdoutOverlapped, sizeof(m_readStdoutOverlapped));
	m_readStdoutOverlapped.process = this;

	ZeroMemory(&m_readStderrOverlapped, sizeof(m_readStderrOverlapped));
	m_readStderrOverlapped.process = this;

	// Idea(joare): Pass receiver directly to windows through jlsIO.
}

jlsProcess::~jlsProcess() {
	// TODO(joare): Test and review.

	TerminateHandle(&m_remote_io);
	TerminateHandle(&m_remote_err);
	TerminateHandle(&m_handles[IO]);
	TerminateHandle(&m_handles[ERR]);

	TerminateProcess(m_procInfo.hProcess, 1);
}

bool jlsProcess::Execute(TCHAR *cmd)
{
	m_cmd = cmd;

	if (!MakePipes() || !MakeProcess())
		return false;

	ReadFileEx(
		m_handles[IO],
		m_readStdoutBuf,
		4096,
		&m_readStdoutOverlapped,
		&jlsProcess::OnReadStdout);

	ReadFileEx(
		m_handles[ERR],
		m_readStderrBuf,
		4096,
		&m_readStderrOverlapped,
		&jlsProcess::OnReadStderr);

	return true;
}

DWORD jlsProcess::Write(LPVOID lpBuffer, DWORD nBytes) {
	DWORD nBytesWritten = 0;
	bool wf = WriteFile(m_handles[IO], lpBuffer, nBytes, &nBytesWritten, NULL);
	FlushFileBuffers(m_handles[IO]);

	//wxLogMessage("result: %d\nlast error: %d\nrequested: %d\nwritten: %d",
	//	wf, GetLastError(), nBytes, nBytesWritten);

	return nBytesWritten;
}

DWORD jlsProcess::WaitForObjectsOrMsg()
{
	return MsgWaitForMultipleObjectsEx(
		2,
		m_handles,
		INFINITE,
		QS_ALLINPUT,
		MWMO_ALERTABLE);
}

VOID WINAPI jlsProcess::OnReadStdout(
	DWORD dwError,
	DWORD dwNBytes,
	LPOVERLAPPED lpOverlapped)
{
	jlsProcess *process = static_cast<jlsIO*>(lpOverlapped)->process;
	process->OnReadStdout(dwNBytes);
}

VOID WINAPI jlsProcess::OnReadStderr(
	DWORD dwError,
	DWORD dwNBytes,
	LPOVERLAPPED lpOverlapped)
{
	jlsProcess *process = static_cast<jlsIO*>(lpOverlapped)->process;
	process->OnReadStderr(dwNBytes);
}

void jlsProcess::OnReadStdout(DWORD nBytes)
{
	if (nullptr != m_cb_out)
		m_cb_out(m_readStdoutBuf, nBytes, m_receiver);

	ReadFileEx(
		m_handles[IO],
		m_readStdoutBuf,
		4096,
		&m_readStdoutOverlapped,
		&jlsProcess::OnReadStdout);
}

void jlsProcess::OnReadStderr(DWORD nBytes)
{
	if (nullptr != m_cb_err)
		m_cb_out(m_readStderrBuf, nBytes, m_receiver);

	ReadFileEx(
		m_handles[ERR],
		m_readStderrBuf,
		4096,
		&m_readStderrOverlapped,
		&jlsProcess::OnReadStderr);
}

bool jlsProcess::MakePipes()
{
	static ULONG pipeSerial = 0;

	TCHAR pipeNameBuf[PIPE_NAME_MAX];
	StringCbPrintf(
		pipeNameBuf,
		PIPE_NAME_MAX * sizeof(TCHAR),
		_T("\\\\.\\Pipe\\RemoteExeAnon.%08x.%08x"),
		GetCurrentProcessId(),
		InterlockedIncrement(&pipeSerial));

	m_handles[IO] = CreateNamedPipe(
		pipeNameBuf,
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE,
		1,
		4096,
		4096,
		5000,
		NULL);

	if (INVALID_HANDLE_VALUE == m_handles[IO]) {
		wxLogMessage("CreateNamedPipe error: %d", GetLastError());
		return false;
	}

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
		TerminateHandle(&m_handles[IO]);
		wxLogMessage("CreateFile error: %d", GetLastError());
		return false;
	}

	StringCbPrintf(
		pipeNameBuf,
		PIPE_NAME_MAX * sizeof(TCHAR),
		_T("\\\\.\\Pipe\\RemoteExeAnon.%08x.%08x"),
		GetCurrentProcessId(),
		InterlockedIncrement(&pipeSerial));

	m_handles[ERR] = CreateNamedPipe(
		pipeNameBuf,
		PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE,
		1,
		4096,
		4096,
		5000,
		NULL);

	if (INVALID_HANDLE_VALUE == m_handles[ERR]) {
		TerminateHandle(&m_handles[IO]);
		TerminateHandle(&m_remote_io);
		wxLogMessage("CreateNamedPipe error: %d", GetLastError());
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
		TerminateHandle(&m_handles[IO]);
		TerminateHandle(&m_remote_io);
		TerminateHandle(&m_handles[ERR]);
		wxLogMessage("CreateFile error: %d", GetLastError());
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
