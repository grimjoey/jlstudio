#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/stc/stc.h>

#include "jls/defs.h"
#include "jls/appframe.h"
#include "jls/editor.h"
#include "jls/tabbededitor.h"
#include "jls/console.h"

wxBEGIN_EVENT_TABLE(jlsAppFrame, wxFrame)
	EVT_CLOSE(jlsAppFrame::OnClose)

	// file
	EVT_MENU(wxID_NEW, jlsAppFrame::OnFileNew)
	EVT_MENU(wxID_OPEN, jlsAppFrame::OnFileOpen)
	EVT_MENU(wxID_SAVE, jlsAppFrame::OnFileSave)
	EVT_MENU(wxID_SAVEAS, jlsAppFrame::OnFileSaveAs)
	EVT_MENU(jlsID_SAVEALL, jlsAppFrame::OnFileSaveAll)
	EVT_MENU(wxID_CLOSE, jlsAppFrame::OnFileClose)
	EVT_MENU(wxID_EXIT, jlsAppFrame::OnExit)

	// tools
	EVT_MENU(wxID_PREFERENCES, jlsAppFrame::OnPreferences)

	// run
	EVT_MENU(jlsID_MENURUN_RUN, jlsAppFrame::OnRun)
wxEND_EVENT_TABLE()

jlsAppFrame::jlsAppFrame()
	: wxFrame(NULL, wxID_ANY, "JLStudio", wxDefaultPosition, wxSize(800, 600)),
	m_juliaPath("C:\\windows\\system32\\cmd.exe")// /K C:\\users\\grimjoey\\appdata\\local\\julia-0.6.2\\bin\\julia.exe")
{
	CreateMenu();

	wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
	splitter->SetWindowStyleFlag(wxSP_LIVE_UPDATE);

	m_tabbedEditor = new jlsTabbedEditor(splitter, wxID_ANY);

	m_console = new jlsConsole(splitter);
	if (wxFileExists(m_juliaPath))
		m_console->Execute(m_juliaPath);

	splitter->SplitVertically(m_tabbedEditor, m_console);
	splitter->SetSashGravity(0.5);
}

jlsAppFrame::~jlsAppFrame() {}

const jlsConsole *jlsAppFrame::GetConsole() const
{
	return m_console;
}

void jlsAppFrame::CreateMenu()
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

	wxMenu *menuRun = new wxMenu;
	menuRun->Append(jlsID_MENURUN_RUN, _("Run"));

	wxMenu *menuTools = new wxMenu;
	menuTools->Append(wxID_PREFERENCES, _("Preferences"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuRun, _("Run"));
	menuBar->Append(menuTools, _("Tools"));

	SetMenuBar(menuBar);
}

void jlsAppFrame::OnExit(wxCommandEvent &event)
{
	Close(true);
}

void jlsAppFrame::OnClose(wxCloseEvent &event)
{
	// Hide cleanup delay
	Hide();
	event.Skip();
}

void jlsAppFrame::OnFileNew(wxCommandEvent &event)
{
	m_tabbedEditor->NewFile();
}

void jlsAppFrame::OnFileOpen(wxCommandEvent &event)
{
	m_tabbedEditor->LoadFile();
}

void jlsAppFrame::OnFileSave(wxCommandEvent &event)
{
	wxMessageBox("Saving is not implemented yet");
}

void jlsAppFrame::OnFileSaveAs(wxCommandEvent &event)
{
	wxMessageBox("Saving is not implemented yet");
}

void jlsAppFrame::OnFileSaveAll(wxCommandEvent &event)
{
	wxMessageBox("Saving is not implemented yet");
}

void jlsAppFrame::OnFileClose(wxCommandEvent &event)
{
	// TODO(joare): Ask for confirmation if editor has unsaved changes.

	wxWindow *currentPage = m_tabbedEditor->GetCurrentPage();
	int currentPageIndex = m_tabbedEditor->GetPageIndex(currentPage);
	m_tabbedEditor->DeletePage(currentPageIndex);
}

void jlsAppFrame::OnPreferences(wxCommandEvent &event)
{
	jlsPreferences prefs(this, wxID_ANY);
	prefs.ShowModal();
}

void jlsAppFrame::OnRun(wxCommandEvent &event)
{
	jlsEditor *currentEditor = static_cast<jlsEditor*>(m_tabbedEditor->GetCurrentPage());
	wxString filename(currentEditor->GetFilename());
	filename.Replace("\\", "\\\\");
	m_console->AppendText(wxString::Format("include(\"%s\")", filename));
	m_console->EmulateKeyPress(CreateKeyEvent(wxEVT_KEY_DOWN, WXK_RETURN));
}