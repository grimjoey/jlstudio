#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/stc/stc.h>

#include "jls/jlsdefs.h"
#include "jls/jlsframe.h"
#include "jls/jlstabbededitor.h"
#include "jls/jlsconsole.h"

wxBEGIN_EVENT_TABLE(jlsFrame, wxFrame)
	EVT_CLOSE(jlsFrame::OnClose)

	// file
	EVT_MENU(wxID_NEW, jlsFrame::OnFileNew)
	EVT_MENU(wxID_OPEN, jlsFrame::OnFileOpen)
	EVT_MENU(wxID_SAVE, jlsFrame::OnFileSave)
	EVT_MENU(wxID_SAVEAS, jlsFrame::OnFileSaveAs)
	EVT_MENU(jlsID_SAVEALL, jlsFrame::OnFileSaveAll)
	EVT_MENU(wxID_CLOSE, jlsFrame::OnFileClose)
wxEND_EVENT_TABLE()

jlsFrame::jlsFrame()
	: wxFrame(NULL, wxID_ANY, "JLStudio", wxDefaultPosition, wxSize(800, 600))
{
	CreateMenu();

	wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
	splitter->SetWindowStyleFlag(wxSP_LIVE_UPDATE);

	m_tabbedEditor = new jlsTabbedEditor(splitter, wxID_ANY);

	jlsConsole *console = new jlsConsole(splitter);
	console->Execute("C:\\Users\\grimjoey\\appdata\\local\\Julia-0.6.2\\bin\\julia.exe");

	splitter->SplitVertically(m_tabbedEditor, console);
	splitter->SetSashGravity(0.5);

	Bind(wxEVT_MENU, &jlsFrame::OnExit, this, wxID_EXIT);
}

jlsFrame::~jlsFrame() {}

void jlsFrame::CreateMenu()
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_NEW, _("&New\tCtrl+N"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_OPEN, _("&Open\tCtrl+O"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_SAVE, _("&Save\tCtrl+S"));
	menuFile->Append(wxID_SAVEAS, _("Save &as...\tCtrl+Shift+S"));
	menuFile->Append(jlsID_SAVEALL, _("Save all"));
	menuFile->Append(wxID_CLOSE, _("Close"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");

	SetMenuBar(menuBar);
}

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

void jlsFrame::OnFileNew(wxCommandEvent &event)
{
	m_tabbedEditor->NewFile();
}

void jlsFrame::OnFileOpen(wxCommandEvent &event)
{
	m_tabbedEditor->LoadFile();
}

void jlsFrame::OnFileSave(wxCommandEvent &event)
{
}

void jlsFrame::OnFileSaveAs(wxCommandEvent &event)
{
}

void jlsFrame::OnFileSaveAll(wxCommandEvent &event)
{
}

void jlsFrame::OnFileClose(wxCommandEvent &event)
{
	// TODO(joare): ask for comfirmation if editor has unsaved changes

	wxWindow *currentPage = m_tabbedEditor->GetCurrentPage();
	int currentPageIndex = m_tabbedEditor->GetPageIndex(currentPage);
	m_tabbedEditor->DeletePage(currentPageIndex);
}
