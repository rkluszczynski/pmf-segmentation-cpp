#include "AddPointDialog.h"
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

	clickedOK = false;
	x0 = y0 = 75;
	x = y = 0;
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
    x = event.GetX();
    y = event.GetY();
    Panel4->Refresh();
}


/// Paint the background
void AddPointDialog::PaintScrolledWindowBackground(wxDC& dc)
{
    wxColour backgroundColour = Panel4->GetBackgroundColour();
    if (!backgroundColour.Ok())
        backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK);
    dc.SetBrush(wxBrush(backgroundColour));
    wxRect windowRect(wxPoint(0, 0), Panel4->GetVirtualSize());//GetClientSize());
    //Panel4->CalcUnscrolledPosition(windowRect.x, windowRect.y, & windowRect.x, & windowRect.y);
    dc.DrawRectangle(windowRect);
}

void AddPointDialog::OnPanel4Paint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(Panel4);
    PrepareDC(dc);

    dc.SetBackground(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK)));
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawCircle(x0, y0, 55);
    dc.SetPen(wxPen(*wxBLACK, 2, wxSOLID));
    dc.DrawLine(x0, y0, x, y);
    //dc.SetUserScale(0.5,0.5);
}
