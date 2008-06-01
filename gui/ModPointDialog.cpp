#include "ModPointDialog.h"
#include <wx/wx.h>
#include <wx/dcbuffer.h>

//(*InternalHeaders(ModPointDialog)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ModPointDialog)
//*)

BEGIN_EVENT_TABLE(ModPointDialog,wxDialog)
	//(*EventTable(ModPointDialog)
	//*)
END_EVENT_TABLE()

ModPointDialog::ModPointDialog(wxWindow* parent)
{
	//(*Initialize(ModPointDialog)
	wxXmlResource::Get()->LoadObject(this,parent,_T("ModPointDialog"),_T("wxDialog"));
	StaticText5 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT5"));
	StaticText6 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT6"));
	pointIdTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_POINTID_TEXTCTRL"));
	pointInfoStaticText = (wxStaticText*)FindWindow(XRCID("ID_POINTINFO_STATICTEXT"));
	useBlocksCheckBox = (wxCheckBox*)FindWindow(XRCID("ID_USE_BLOCKS_CHECKBOX"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	blockSizeTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_BLOCKSIZE_TEXTCTRL"));
	StaticText3 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT3"));
	radianAngleTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_RADIAN_TEXTCTRL"));
	StaticText4 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT4"));
	degreeAngleTextCtrl = (wxTextCtrl*)FindWindow(XRCID("ID_DEGREE_TEXTCTRL"));
	leftPanel = (wxPanel*)FindWindow(XRCID("ID_LEFT_PANEL"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	angleCirclePanel = (wxPanel*)FindWindow(XRCID("ID_CIRCLE_PANEL"));
	sinStaticText = (wxStaticText*)FindWindow(XRCID("ID_SIN_STATICTEXT"));
	cosStaticText = (wxStaticText*)FindWindow(XRCID("ID_COS_STATICTEXT"));
	orientationPanel = (wxPanel*)FindWindow(XRCID("ID_ORIENTATION_PANEL"));
	topPanel = (wxPanel*)FindWindow(XRCID("ID_TOP_PANEL"));
	okButton = (wxButton*)FindWindow(XRCID("ID_OK_BUTTON"));
	cancelButton = (wxButton*)FindWindow(XRCID("ID_CANCEL_BUTTON"));
	bottomPanel = (wxPanel*)FindWindow(XRCID("ID_BOTTOM_PANEL"));

	Connect(XRCID("ID_USE_BLOCKS_CHECKBOX"),wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ModPointDialog::OnUseBlocksCheckBoxClick);
	Connect(XRCID("ID_RADIAN_TEXTCTRL"),wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ModPointDialog::OnRadianAngleTextCtrlText);
	Connect(XRCID("ID_DEGREE_TEXTCTRL"),wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ModPointDialog::OnDegreeAngleTextCtrlText);
	angleCirclePanel->Connect(XRCID("ID_CIRCLE_PANEL"),wxEVT_PAINT,(wxObjectEventFunction)&ModPointDialog::OnAngleCirclePanelPaint,0,this);
	angleCirclePanel->Connect(XRCID("ID_CIRCLE_PANEL"),wxEVT_LEFT_UP,(wxObjectEventFunction)&ModPointDialog::OnAngleCirclePanelLeftUp,0,this);
	Connect(XRCID("ID_OK_BUTTON"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ModPointDialog::OnOkButtonClick);
	Connect(XRCID("ID_CANCEL_BUTTON"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ModPointDialog::OnCancelButtonClick);
	//*)
	circleRadius = 55;

	clickedOK = false;
	x0 = y0 = 75;
	x = 75 + circleRadius;
	y = 75;
	angle = 0.0f;
}

ModPointDialog::~ModPointDialog()
{
	//(*Destroy(ModPointDialog)
	//*)
}


bool ModPointDialog::isOk()
{
    return clickedOK;
}


void ModPointDialog::OnAngleCirclePanelPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(angleCirclePanel);
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


void ModPointDialog::OnOkButtonClick(wxCommandEvent& event)
{
    clickedOK = true;
    Close();
}

void ModPointDialog::OnCancelButtonClick(wxCommandEvent& event)
{
    clickedOK = false;
    Close();
}


#define radians2degree(X) (X * 180.0 / M_PI)
#define degree2radians(X) (X * M_PI / 180.0)
void ModPointDialog::OnAngleCirclePanelLeftUp(wxMouseEvent& event)
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

        radianAngleTextCtrl->ChangeValue(wxString::Format(wxT("%lf"), angle));
        degreeAngleTextCtrl->ChangeValue(wxString::Format(wxT("%lf"), radians2degree(angle)));

        sinStaticText->SetLabel(wxString::Format(wxT("Sinus = %.3lf"), sinus ));
        cosStaticText->SetLabel(wxString::Format(wxT("Cosinus = %.3lf"), cosinus ));
    }
    angleCirclePanel->Refresh();
}


void ModPointDialog::OnDegreeAngleTextCtrlText(wxCommandEvent& event)
{
    wxString str = degreeAngleTextCtrl->GetValue();
    double deg;
    if (str.ToDouble(&deg)) {
        angle = degree2radians(deg);
        sinus = sin(angle);
        cosinus = cos(angle);
        x = x0 + int(cosinus * double(circleRadius));
        y = y0 - int(sinus * double(circleRadius));
        angleCirclePanel->Refresh();

        radianAngleTextCtrl->ChangeValue(wxString::Format(wxT("%lf"), angle));
        sinStaticText->SetLabel(wxString::Format(wxT("Sinus = %.3lf"), sinus ));
        cosStaticText->SetLabel(wxString::Format(wxT("Cosinus = %.3lf"), cosinus ));
    }
    else if (str.IsEmpty()) {
        degreeAngleTextCtrl->ChangeValue(wxT(""));
    }
    else {
        wxMessageBox(wxT("BLAD"), wxT("ERROR"));
        degreeAngleTextCtrl->Undo();
    }
}


void ModPointDialog::OnRadianAngleTextCtrlText(wxCommandEvent& event)
{
    wxString str = radianAngleTextCtrl->GetValue();
    double rad;
    if (str.ToDouble(&rad)) {
        angle = rad;
        sinus = sin(angle);
        cosinus = cos(angle);
        x = x0 + int(cosinus * double(circleRadius));
        y = y0 - int(sinus * double(circleRadius));
        angleCirclePanel->Refresh();

        degreeAngleTextCtrl->ChangeValue(wxString::Format(wxT("%lf"), radians2degree(angle)));
        sinStaticText->SetLabel(wxString::Format(wxT("Sinus = %.3lf"), sinus ));
        cosStaticText->SetLabel(wxString::Format(wxT("Cosinus = %.3lf"), cosinus ));
    }
    else if (str.IsEmpty()) {
        radianAngleTextCtrl->ChangeValue(wxT(""));
    }
    else {
        radianAngleTextCtrl->Undo();
        wxMessageBox(wxT("BLAD"), wxT("ERROR"));
    }
}
#undef radians2degree
#undef degree2radians


void ModPointDialog::SetPMFPointData(pmf_point<double> * pt)
{
    selectedPoint = pt;
    if (pt) {
        pointIdTextCtrl->ChangeValue(wxString::Format(wxT("%li"), pt->id));
        wxString coord(wxString::Format(wxT(" x = %.3lf  ;  y = %.3lf "), pt->x, pt->y));
        wxString info;
        switch (pt->type)
        {
            case  1 :  info = _("BRITH_NORMAL");  break;
            case  2 :  info = _(" BIRTH_LEFT ");  break;
            case  3 :  info = _("  BIRTH_UP  ");  break;
            case  4 :  info = _(" BIRTH_DOWN ");  break;
            case  5 :  info = _("   BORDER   ");  break;
            case  6 :  info = _("INTERSECTION");  break;
            case  7 :  info = _("   UPDATE   ");  break;
            default :  info = _("  UNKNOWN   ");
        }
        pointInfoStaticText->SetLabel(wxString::Format(wxT("%s\n%s"), coord.c_str(), info.c_str()));
    }
    else {
        pointIdTextCtrl->ChangeValue(wxT("(Enter ID)"));
        pointInfoStaticText->SetLabel(wxT("---\n-"));
    }
}


void ModPointDialog::OnUseBlocksCheckBoxClick(wxCommandEvent& event)
{
    bool value = event.IsChecked();
    StaticText2->Enable(value);
    blockSizeTextCtrl->Enable(value);
}

