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
		wxMenuBar* PMFApplicationMenuBar;
		wxHtmlWindow* myHtmlWindow;
		wxScrolledWindow* myScrolledWindow;
		wxSplitterWindow* mySplitterWindow;
		wxNotebook* myNotebook;
		wxStatusBar* ApplicationStatusBar;
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
		void OnMyScrolledWindowPaint(wxPaintEvent& event);
		void OnViewInfosMenuItemSelected(wxCommandEvent& event);
		void OnLoadImageMenuItemSelected(wxCommandEvent& event);
		void OnMyNotebookPageChanged(wxNotebookEvent& event);
		void OnCloseImageMenuItemSelected(wxCommandEvent& event);
		//*)

        void PaintScrolledWindowBackground(wxDC& dc);

		DECLARE_EVENT_TABLE()
};

#endif
