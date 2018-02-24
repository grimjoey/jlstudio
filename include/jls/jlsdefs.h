#ifndef _JLSDEFS_H_
#define _JLSDEFS_H_

#include <wx/defs.h>

#define jlsCONSOLE_POLL_PROCESS_OUTPUT_FREQUENCY_HZ 200
#define jlsCONSOLE_POLL_PROCESS_OUTPUT_TIMEOUT_SEC 5

enum {
	jlsID_NOT_USED = wxID_HIGHEST,
	jlsID_SAVEALL,
	jlsID_CONSOLE,
	jlsID_CONSOLE_PROCESS,
	jlsID_CONSOLE_TIMER,
	jlsID_PREFS_START_JL
};

#endif // _JLSDEFS_H_
