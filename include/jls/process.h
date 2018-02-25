#ifndef _JLSPROCESS_H_
#define _JLSPROCESS_H_

#include <windows.h>

// Pipe name size from msdn:
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365150(v=vs.85).aspx
// There are references to MAX_PATH too, which is 260.
#define PIPE_NAME_MAX 256

typedef void (*jlsProcessCb)(void *buffer, size_t n, void *receiver);

class jlsProcess;

struct jlsIO : public OVERLAPPED
{
	jlsProcess *process;
};

class jlsProcess
{
public:
	jlsProcess(jlsProcessCb out, jlsProcessCb err, void *receiver);
	~jlsProcess();

	// Execute an external program
	bool Execute(TCHAR *cmd);

	// Write to the process' stdin
	// Returns number of bytes written.
	DWORD Write(LPVOID lpBuffer, DWORD nBytes);

	// Call this when Idle
	//
	// This function calls MsgWaitForMultipleObjects
	// which puts the calling thread in "an alertable state"
	// ready to recieve async i/o notifications.
	// It will cancel when another event is waiting in the
	// message queue.
	DWORD WaitForObjectsOrMsg();

	// Static callback for stdout (calls non-static equivalent)
	static VOID WINAPI OnReadStdout(DWORD dwError, DWORD dwNBytes, LPOVERLAPPED lpOverlapped);

	// Static callback for stderr (calls non-static equivalent)
	static VOID WINAPI OnReadStderr(DWORD dwError, DWORD dwNBytes, LPOVERLAPPED lpOverlapped);

	void OnReadStdout(DWORD nBytes);
	void OnReadStderr(DWORD nBytes);

	LPTSTR m_cmd;

private:
	jlsProcessCb m_cb_out;
	jlsProcessCb m_cb_err;

	// TODO(joare): Find a better name.
	void *m_receiver;

	enum Direction { IO, ERR };

	PROCESS_INFORMATION m_procInfo;
	HANDLE m_remote_io;
	HANDLE m_remote_err;

	HANDLE m_handles[2];

	TCHAR m_readStdoutBuf[4096];
	jlsIO m_readStdoutOverlapped;

	TCHAR m_readStderrBuf[4096];
	jlsIO m_readStderrOverlapped;

	// Make async pipes
	bool MakePipes();

	// Make the process
	bool MakeProcess();

	// Close handle and set it to INVALID_HANDLE_VALUE
	// Does nothing if the handle is already invalid.
	static void TerminateHandle(LPHANDLE handle);
};

#endif // _JLSPROCESS_H_