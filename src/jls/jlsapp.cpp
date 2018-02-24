#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/jlsapp.h"
#include "jls/jlsframe.h"

wxIMPLEMENT_APP(jlsApp);

bool jlsApp::OnInit()
{
	jlsFrame *frame = new jlsFrame();
	frame->Show(true);
	return true;
}