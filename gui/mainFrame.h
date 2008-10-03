#ifndef MAINFRAME_H
#define MAINFRAME_H

//(*Headers(mainFrame)
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/menu.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/frame.h>
#include <wx/html/htmlwin.h>
#include <wx/statusbr.h>
//*)

#include "PMFPanel.h"


class mainFrame : public wxFrame
{
	public:

		mainFrame(wxWindow* parent);
		virtual ~mainFrame();

		void AddPointAction(wxCommandEvent&, double, double);

		//(*Declarations(mainFrame)
		wxMenu* Menu3;
		wxStaticBitmap* StaticBitmap1;
		wxMenuItem* MenuItem1;
		wxMenu* Menu1;
		wxMenuItem* savePMFMenuItem;
		wxHtmlWindow* myHtmlWindow;
		wxStatusBar* StatusBar1;
		wxScrolledWindow* myScrolledWindow;
		wxSplitterWindow* mySplitterWindow;
		wxMenuBar* MenuBar1;
		wxNotebook* myNotebook;
		wxMenu* Menu2;
		wxMenu* Menu5;
		wxMenu* Menu4;
		//*)

	protected:

		//(*Identifiers(mainFrame)
		//*)

	private:

        double defaultX, defaultY;
        int htmlWindowHeight;

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
		void OnAddPointMenuItemSelected(wxCommandEvent& event);
		void OnRegenerateMenuItemSelected(wxCommandEvent& event);
		void OnSavePMFMenuItemSelected(wxCommandEvent& event);
		void OnLoadPMFMenuItemSelected(wxCommandEvent& event);
		void OnMySplitterWindowDClick(wxSplitterEvent& event);
		void OnAddSegmentMenuItemSelected(wxCommandEvent& event);
		void OnUpdatePointMenuItemSelected(wxCommandEvent& event);
		void OnRemovePointMenuItemSelected(wxCommandEvent& event);

		void OnShowProgressMenuItemSelected(wxCommandEvent& event);
		//*)
		void OnMyScrolledWindowSize(wxSizeEvent& event);

        void PaintScrolledWindowBackground(wxDC&);
        void GeneratingPMFAction(wxCommandEvent&, PMFPanel *);

    DECLARE_EVENT_TABLE()
};

#endif
