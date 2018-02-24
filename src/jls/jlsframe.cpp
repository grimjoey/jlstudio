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
	EVT_MENU(wxID_EXIT, jlsFrame::OnExit)

	// tools
	EVT_MENU(wxID_PREFERENCES, jlsFrame::OnPreferences)
wxEND_EVENT_TABLE()

jlsFrame::jlsFrame()
	: wxFrame(NULL, wxID_ANY, "JLStudio", wxDefaultPosition, wxSize(800, 600)),
	m_juliaPath("wrong path")
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

jlsFrame::~jlsFrame() {}

const jlsConsole *jlsFrame::GetConsole() const
{
	return m_console;
}

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

	wxMenu *menuTools = new wxMenu;
	menuTools->Append(wxID_PREFERENCES, _("Preferences"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuTools, _("Tools"));

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
	wxMessageBox("Saving is not implemented yet");
}

void jlsFrame::OnFileSaveAs(wxCommandEvent &event)
{
	wxMessageBox("Saving is not implemented yet");
}

void jlsFrame::OnFileSaveAll(wxCommandEvent &event)
{
	wxMessageBox("Saving is not implemented yet");
}

void jlsFrame::OnFileClose(wxCommandEvent &event)
{
	// TODO(joare): Ask for confirmation if editor has unsaved changes.

	wxWindow *currentPage = m_tabbedEditor->GetCurrentPage();
	int currentPageIndex = m_tabbedEditor->GetPageIndex(currentPage);
	m_tabbedEditor->DeletePage(currentPageIndex);
}

void jlsFrame::OnPreferences(wxCommandEvent &event)
{
	jlsPreferences prefs(this, wxID_ANY);
	prefs.ShowModal();
}