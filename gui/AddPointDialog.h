#ifndef ADDPOINTDIALOG_H
#define ADDPOINTDIALOG_H

//(*Headers(AddPointDialog)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class AddPointDialog: public wxDialog
{
	public:

		AddPointDialog(wxWindow* parent);
		AddPointDialog(wxWindow* parent, double x, double y);
		virtual ~AddPointDialog();

        bool isOk();

		//(*Declarations(AddPointDialog)
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl4;
		wxPanel* Panel5;
		wxStaticText* StaticText2;
		wxPanel* Panel4;
		wxButton* Button1;
		wxStaticText* StaticText8;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* Button2;
		wxPanel* Panel3;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxTextCtrl* TextCtrl2;
		wxTextCtrl* TextCtrl1;
		wxPanel* Panel2;
		wxStaticText* StaticText4;
		wxTextCtrl* TextCtrl3;
		//*)

	protected:

		//(*Identifiers(AddPointDialog)
		//*)

	private:

        bool clickedOK;
        int x0, y0, x, y;
        double angle, sinus, cosinus;
        int circleRadius;

		//(*Handlers(AddPointDialog)
		void OnPanel4LeftUp(wxMouseEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnPanel4Paint(wxPaintEvent& event);
		void OnTextCtrl4Text(wxCommandEvent& event);
		void OnTextCtrl1Text(wxCommandEvent& event);
		//*)
        void PaintScrolledWindowBackground(wxDC& );

    DECLARE_EVENT_TABLE()
};

#endif
