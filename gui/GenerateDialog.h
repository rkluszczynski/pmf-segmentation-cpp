#ifndef GENERATEDIALOG_H
#define GENERATEDIALOG_H

//(*Headers(GenerateDialog)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class GenerateDialog: public wxDialog
{
	public:

		GenerateDialog(wxWindow* parent);
		virtual ~GenerateDialog();

		bool isOk();

		//(*Declarations(GenerateDialog)
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxStaticText* StaticText1;
		wxButton* Button2;
		wxTextCtrl* TextCtrl2;
		wxTextCtrl* TextCtrl1;
		//*)

	protected:

		//(*Identifiers(GenerateDialog)
		//*)

	private:
        bool clickedOK;

		//(*Handlers(GenerateDialog)
		void OnButton2Click(wxCommandEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
