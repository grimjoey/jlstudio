#ifndef _JLSPROCESS_H_
#define _JLSPROCESS_H_

#include <windows.h>

class jlsProcess
{
public:
	jlsProcess();
	~jlsProcess();

	bool Execute(LPTSTR cmd);
	void Write();

	static VOID WINAPI OnReadStdin(DWORD dwError, DWORD dwNBytes, LPOVERLAPPED lpOverlapped);
	static VOID WINAPI OnReadStderr(DWORD dwError, DWORD dwNBytes, LPOVERLAPPED lpOverlapped);

private:
	LPTSTR m_cmd;
	PROCESS_INFORMATION m_procInfo;
	HANDLE m_remote_io;
	HANDLE m_remote_err;
	HANDLE m_io;
	HANDLE m_err;

	TCHAR m_readStdinBuf[4096];
	OVERLAPPED m_readStdinOverlapped;

	TCHAR m_readStderrBuf[4096];
	OVERLAPPED m_readStderrOverlapped;

	TCHAR m_writeBuf[4096];
	OVERLAPPED m_writeOverlapped;

	bool MakePipes();
	bool MakeProcess();
	static void TerminateHandle(LPHANDLE handle);
};

#endif // _JLSPROCESS_H_