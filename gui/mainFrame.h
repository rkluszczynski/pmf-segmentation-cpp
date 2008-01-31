#ifndef MAINFRAME_H
#define MAINFRAME_H

//(*Headers(mainFrame)
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/menu.h>
#include <wx/splitter.h>
#include <wx/frame.h>
#include <wx/html/htmlwin.h>
#include <wx/statusbr.h>
//*)

class mainFrame: public wxFrame
{
	public:

		mainFrame(wxWindow* parent);
		virtual ~mainFrame();

		//(*Declarations(mainFrame)
		wxMenu* Menu3;
		wxMenuItem* MenuItem1;
		wxMenu* Menu1;
		wxMenuItem* aboutMenuItem;
		wxHtmlWindow* myHtmlWindow;
		wxStatusBar* StatusBar1;
		wxScrolledWindow* myScrolledWindow;
		wxSplitterWindow* mySplitterWindow;
		wxMenuItem* generateMenuItem;
		wxMenuBar* MenuBar1;
		wxNotebook* myNotebook;
		wxMenu* Menu2;
		wxMenuItem* quitMenuItem;
		//*)

	protected:

		//(*Identifiers(mainFrame)
		//*)

	private:

		//(*Handlers(mainFrame)
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnMenuItem1Selected(wxCommandEvent& event);
		void OnGenerateMenuItemSelected(wxCommandEvent& event);
		void OnmyNotebookPageChanged(wxNotebookEvent& event);
		void OnMyScrolledWindowPaint(wxPaintEvent& event);
		//*)

        void PaintScrolledWindowBackground(wxDC& dc);

		DECLARE_EVENT_TABLE()
};

#endif
