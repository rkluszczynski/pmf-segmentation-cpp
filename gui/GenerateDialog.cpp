#include "GenerateDialog.h"

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
	TextCtrl1 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	TextCtrl2 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL2"));
	Button1 = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	Button2 = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GenerateDialog::OnButton1Click);
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GenerateDialog::OnButton2Click);
	//*)

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
