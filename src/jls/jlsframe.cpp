#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/stc/stc.h>

#include "jls/jlsframe.h"
#include "jls/jlsconsole.h"

wxBEGIN_EVENT_TABLE(jlsFrame, wxFrame)
	EVT_CLOSE(jlsFrame::OnClose)
wxEND_EVENT_TABLE()

jlsFrame::jlsFrame()
	: wxFrame(NULL, wxID_ANY, "JLStudio")
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");

	SetMenuBar(menuBar);

	wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
	splitter->SetWindowStyleFlag(wxSP_LIVE_UPDATE);

	wxNotebook *notebook = new wxNotebook(splitter, wxID_ANY);

	wxStyledTextCtrl *edit = new wxStyledTextCtrl(notebook, wxID_ANY);
	edit->SetMarginType(0, wxSTC_MARGIN_NUMBER);
	edit->SetMarginWidth(0, 30);

	notebook->AddPage(edit, "Editor");

	jlsConsole *console = new jlsConsole(splitter);
	console->Execute("C:\\Users\\grimjoey\\appdata\\local\\Julia-0.6.2\\bin\\julia.exe");

	splitter->SplitVertically(notebook, console);
	splitter->SetSashGravity(0.5);

	Bind(wxEVT_MENU, &jlsFrame::OnExit, this, wxID_EXIT);
}

jlsFrame::~jlsFrame() {}

void jlsFrame::OnExit(wxCommandEvent &event)
{
	Close(true);
}

void jlsFrame::OnClose(wxCloseEvent &event)
{
	// Hide cleanup delay
	Hide();
	event.Skip();
}