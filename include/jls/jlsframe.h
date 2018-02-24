#ifndef _JLSFRAME_H_
#define _JLSFRAME_H_

#include <wx/frame.h>

class jlsFrame : public wxFrame
{
public:
	jlsFrame();
	~jlsFrame();

private:
	void OnExit(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event);
	wxDECLARE_EVENT_TABLE();
};

#endif // _JLSFRAME_H_