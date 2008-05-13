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
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	TextCtrl2 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL2"));
	StaticText3 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT3"));
	TextCtrl3 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL3"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	TextCtrl1 = (wxTextCtrl*)FindWindow(XRCID("ID_TEXTCTRL1"));
	Panel3 = (wxPanel*)FindWindow(XRCID("ID_PANEL3"));
	Panel4 = (wxPanel*)FindWindow(XRCID("ID_PANEL4"));
	Panel1 = (wxPanel*)FindWindow(XRCID("ID_PANEL1"));
	Button1 = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	Button2 = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));
	Panel2 = (wxPanel*)FindWindow(XRCID("ID_PANEL2"));

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

        x = x0 + int(cosinus * double(circleRadius));
        y = y0 - int(sinus * double(circleRadius));
        //y = GetSize().GetHeight() - y;

        TextCtrl1->SetValue(wxString::Format(wxT("%lf"), angle));
    }
    Panel4->Refresh();
}


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
