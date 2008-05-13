#ifndef ADDPOINTDIALOG_H
#define ADDPOINTDIALOG_H

//(*Headers(AddPointDialog)
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class AddPointDialog: public wxDialog
{
	public:

		AddPointDialog(wxWindow* parent);
		virtual ~AddPointDialog();

        bool isOk();

		//(*Declarations(AddPointDialog)
		wxPanel* Panel4;
		wxButton* Button1;
		wxPanel* Panel1;
		wxButton* Button2;
		wxPanel* Panel3;
		wxTextCtrl* TextCtrl1;
		wxPanel* Panel2;
		//*)

	protected:

		//(*Identifiers(AddPointDialog)
		//*)

	private:

        bool clickedOK;
        int x0, y0, x, y;
        double angle;

		//(*Handlers(AddPointDialog)
		void OnPanel4LeftUp(wxMouseEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnPanel4Paint(wxPaintEvent& event);
		//*)
        void PaintScrolledWindowBackground(wxDC& );

    DECLARE_EVENT_TABLE()
};

#endif
