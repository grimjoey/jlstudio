#ifndef _JLSCONSOLEPROC_H_
#define _JLSCONSOLEPROC_H_

#include <windows.h>

#include <vector>

template<class T>
using vector = std::vector<T>;

class jlsConsoleProc
{
public:
	jlsConsoleProc();
	~jlsConsoleProc();

	void Spawn();
	const PROCESS_INFORMATION *GetPInfo(size_t n) const;
	bool Attach(size_t n);

private:
	vector<PROCESS_INFORMATION> m_procInfos;
};

#endif // _JLSCONSOLEPROC_H_