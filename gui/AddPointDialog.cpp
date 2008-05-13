#include "AddPointDialog.h"
#include <cmath>
#include <wx/wx.h>
#include <wx/dcbuffer.h>

//(*InternalHeaders(AddPointDialog)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(AddPointDialog)
//*)

BEGIN_EVENT_TABLE(AddPointDialog,wxDialog)
	//(*EventTable(AddPointDialog)
	//*)
END_EVENT_TABLE()


AddPointDialog::AddPointDialog(wxWindow* parent)
{
	//(*Initialize(AddPointDialog)
	wxXmlResource::Get()->LoadObject(this,parent,_T("AddPointDialog"),_T("wxDialog"));
	StaticText5 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT5"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	TextCtrl2 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL2"));
	StaticText3 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT3"));
	TextCtrl3 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL3"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	TextCtrl1 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL1"));
	StaticText4 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT4"));
	TextCtrl4 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL4"));
	Panel3 = (wxPanel*)FindWindow(XRCID("ID_PANEL3"));
	StaticText7 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT7"));
	Panel4 = (wxPanel*)FindWindow(XRCID("ID_PANEL4"));
	StaticText8 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT8"));
	StaticText9 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT9"));
	Panel5 = (wxPanel*)FindWindow(XRCID("ID_PANEL5"));
	Panel1 = (wxPanel*)FindWindow(XRCID("ID_PANEL1"));
	Button1 = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	Button2 = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));
	Panel2 = (wxPanel*)FindWindow(XRCID("ID_PANEL2"));

	Connect(XRCID("ID_TEXTCTRL1"),wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&AddPointDialog::OnTextCtrl1Text);
	Connect(XRCID("ID_TEXTCTRL4"),wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&AddPointDialog::OnTextCtrl4Text);
	Panel4->Connect(XRCID("ID_PANEL4"),wxEVT_PAINT,(wxObjectEventFunction)&AddPointDialog::OnPanel4Paint,0,this);
	Panel4->Connect(XRCID("ID_PANEL4"),wxEVT_LEFT_UP,(wxObjectEventFunction)&AddPointDialog::OnPanel4LeftUp,0,this);
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AddPointDialog::OnButton1Click);
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AddPointDialog::OnButton2Click);
	//*)
	circleRadius = 55;

	clickedOK = false;
	x0 = y0 = 75;
	x = 75 + circleRadius;
	y = 75;
	angle = 0.0f;
}


AddPointDialog::~AddPointDialog()
{
	//(*Destroy(AddPointDialog)
	//*)
}

bool AddPointDialog::isOk()
{
    return clickedOK;
}

void AddPointDialog::OnButton1Click(wxCommandEvent& event)
{
    clickedOK = true;
    Close();
}

void AddPointDialog::OnButton2Click(wxCommandEvent& event)
{
    clickedOK = false;
    Close();
}


#define radians2degree(X) (X * 180.0 / M_PI)
#define degree2radians(X) (X * M_PI / 180.0)
void AddPointDialog::OnPanel4LeftUp(wxMouseEvent& event)
{
    if (event.GetX() != x0  ||  event.GetY() != y0)
    {
        double dx = double(event.GetX() - x0);
        double dy = double(y0 - event.GetY());
        double length = sqrt(dx*dx + dy*dy);

        sinus = dy / length;
        cosinus = dx / length;

        angle = asin(sinus);
        if (cosinus < 0) { angle = M_PI - angle; }
        else if (sinus < 0) { angle = 2 * M_PI + angle; }

        x = x0 + int(cosinus * double(circleRadius));
        y = y0 - int(sinus * double(circleRadius));
        //y = GetSize().GetHeight() - y;

        TextCtrl1->ChangeValue(wxString::Format(wxT("%lf"), angle));
        TextCtrl4->ChangeValue(wxString::Format(wxT("%lf"), radians2degree(angle)));

        StaticText8->SetLabel(wxString::Format(wxT("Sinus = %.3lf"), sinus ));
        StaticText9->SetLabel(wxString::Format(wxT("Cosinus = %.3lf"), cosinus ));
    }
    Panel4->Refresh();
}


void AddPointDialog::OnTextCtrl4Text(wxCommandEvent& event)
{
    wxString str = TextCtrl4->GetValue();
    double deg;
    if (str.ToDouble(&deg)) {
        angle = degree2radians(deg);
        sinus = sin(angle);
        cosinus = cos(angle);
        x = x0 + int(cosinus * double(circleRadius));
        y = y0 - int(sinus * double(circleRadius));
        Panel4->Refresh();

        TextCtrl1->ChangeValue(wxString::Format(wxT("%lf"), angle));
        StaticText8->SetLabel(wxString::Format(wxT("Sinus = %.3lf"), sinus ));
        StaticText9->SetLabel(wxString::Format(wxT("Cosinus = %.3lf"), cosinus ));
    }
    else if (str.IsEmpty()) {
        TextCtrl1->ChangeValue(wxT(""));
    }
    else {
        wxMessageBox(wxT("BLAD"), wxT("ERROR"));
        TextCtrl4->Undo();
    }
}


void AddPointDialog::OnTextCtrl1Text(wxCommandEvent& event)
{
    wxString str = TextCtrl1->GetValue();
    double rad;
    if (str.ToDouble(&rad)) {
        angle = rad;
        sinus = sin(angle);
        cosinus = cos(angle);
        x = x0 + int(cosinus * double(circleRadius));
        y = y0 - int(sinus * double(circleRadius));
        Panel4->Refresh();

        TextCtrl4->ChangeValue(wxString::Format(wxT("%lf"), radians2degree(angle)));
        StaticText8->SetLabel(wxString::Format(wxT("Sinus = %.3lf"), sinus ));
        StaticText9->SetLabel(wxString::Format(wxT("Cosinus = %.3lf"), cosinus ));
    }
    else if (str.IsEmpty()) {
        TextCtrl1->ChangeValue(wxT(""));
    }
    else {
        wxMessageBox(wxT("BLAD"), wxT("ERROR"));
        TextCtrl1->Undo();
    }
}
#undef radians2degree
#undef degree2radians


void AddPointDialog::OnPanel4Paint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(Panel4);
    PrepareDC(dc);

    dc.SetBackground(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK)));
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawCircle(x0, y0, circleRadius);
    dc.DrawLine(x0+circleRadius+2, y0, x0+circleRadius+10, y0);
    //dc.DrawText(wxT("0"), x0+circleRadius+5, y0+1);
    if (x != x0  ||  y != y0)
    {
        dc.SetPen(wxPen(*wxBLACK, 2, wxSOLID));
        dc.DrawLine(x0, y0, x, y);
    }
    //dc.SetUserScale(0.5,0.5);
}

