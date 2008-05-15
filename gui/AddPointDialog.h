#ifndef ADDPOINTDIALOG_H
#define ADDPOINTDIALOG_H

//(*Headers(AddPointDialog)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
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
        void SetPointCoordinates(double, double);

		//(*Declarations(AddPointDialog)
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl4;
		wxPanel* Panel5;
		wxStaticText* StaticText2;
		wxPanel* Panel4;
		wxButton* Button1;
		wxStaticText* StaticText6;
		wxStaticText* StaticText8;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* Button2;
		wxPanel* Panel3;
		wxTextCtrl* BlockSizeTextCtrl;
		wxTextCtrl* RadianAngleTextCtrl;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxTextCtrl* CoordinateYTextCtrl;
		wxCheckBox* UseBlocksCheckBox;
		wxPanel* Panel2;
		wxStaticText* StaticText4;
		wxTextCtrl* CoordinateXTextCtrl;
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
		void OnRadianAngleTextCtrlText(wxCommandEvent& event);
		void OnCheckBox1Click(wxCommandEvent& event);
		//*)
        void PaintScrolledWindowBackground(wxDC& );

    DECLARE_EVENT_TABLE()
};

#endif
