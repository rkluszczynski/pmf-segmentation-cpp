#ifndef GENERATEDIALOG_H
#define GENERATEDIALOG_H

//(*Headers(GenerateDialog)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class GenerateDialog : public wxDialog
{
	public:

		GenerateDialog(wxWindow* parent);
		virtual ~GenerateDialog();

		bool isOk();

		//(*Declarations(GenerateDialog)
		wxButton* GenerateDialogCancelButton;
		wxTextCtrl* FieldSizeTextCtrl;
		wxStaticText* StaticText2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* GenerateDialogOKButton;
		wxTextCtrl* BlockSizeTextCtrl;
		wxCheckBox* UseBlocksCheckBox;
		wxStaticText* StaticText4;
		wxTextCtrl* ScaleTextCtrl;
		//*)

	protected:

		//(*Identifiers(GenerateDialog)
		//*)

	private:

        bool clickedOK;

		//(*Handlers(GenerateDialog)
		void OnButton2Click(wxCommandEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnUseBlocksCheckBoxClick(wxCommandEvent& event);
		//*)

    DECLARE_EVENT_TABLE()
};

#endif
