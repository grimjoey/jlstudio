#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/app.h"
#include "jls/appframe.h"

wxIMPLEMENT_APP(jlsApp);

bool jlsApp::OnInit()
{
	jlsAppFrame *frame = new jlsAppFrame();
	frame->Show(true);
	return true;
}