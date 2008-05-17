#include "GenerateDialog.h"
#include <ctime>

//(*InternalHeaders(GenerateDialog)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(GenerateDialog)
//*)

BEGIN_EVENT_TABLE(GenerateDialog,wxDialog)
	//(*EventTable(GenerateDialog)
	//*)
END_EVENT_TABLE()

GenerateDialog::GenerateDialog(wxWindow* parent)
{
	//(*Initialize(GenerateDialog)
	wxXmlResource::Get()->LoadObject(this,parent,_T("GenerateDialog"),_T("wxDialog"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	FieldSizeTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL1"));
	StaticText3 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT3"));
	SeedTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL4"));
	UseBlocksCheckBox = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	BlockSizeTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL2"));
	StaticText4 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT4"));
	ScaleTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL3"));
	GenerateDialogOKButton = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	GenerateDialogCancelButton = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));

	Connect(XRCID("ID_CHECKBOX1"),wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&GenerateDialog::OnUseBlocksCheckBoxClick);
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GenerateDialog::OnButton1Click);
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GenerateDialog::OnButton2Click);
	//*)

    //SeedTextCtrl->ChangeValue(wxString::Format(wxT("%li"), time(NULL)));
	clickedOK = false;
}

GenerateDialog::~GenerateDialog()
{
	//(*Destroy(GenerateDialog)
	//*)
}


void GenerateDialog::OnButton2Click(wxCommandEvent& event)
{
    clickedOK = false;
    Close();
}

void GenerateDialog::OnButton1Click(wxCommandEvent& event)
{
    clickedOK = true;
    Close();
}

bool GenerateDialog::isOk()
{
    return clickedOK;
}

void GenerateDialog::OnUseBlocksCheckBoxClick(wxCommandEvent& event)
{
    bool value = event.IsChecked();
    StaticText2->Enable(value);
    BlockSizeTextCtrl->Enable(value);
}
