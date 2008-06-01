#include "PMFPanel.h"
#include "mainFrame.h"
#include "AddPointDialog.h"
#include "ModPointDialog.h"
#include <sys/timeb.h>
#include <wx/wx.h>
#include <wx/string.h>

//(*InternalHeaders(PMFPanel)
#include <wx/xrc/xmlres.h>
//*)

IMPLEMENT_CLASS(PMFPanel,wxPanel)

//(*IdInit(PMFPanel)
//*)

BEGIN_EVENT_TABLE(PMFPanel,wxPanel)
	//(*EventTable(PMFPanel)
	//*)
END_EVENT_TABLE()


PMFPanel::PMFPanel(wxWindow* parent)
{
	//(*Initialize(PMFPanel)
	wxXmlResource::Get()->LoadObject(this,parent,_T("PMFPanel"),_T("wxPanel"));
	staticBitmap = (wxStaticBitmap*)FindWindow(XRCID("ID_STATICBITMAP1"));
	scrolledWindow = (wxScrolledWindow*)FindWindow(XRCID("ID_SCROLLEDWINDOW1"));
	//*)
	staticBitmap->Connect(XRCID("ID_STATICBITMAP1"),wxEVT_RIGHT_UP,(wxObjectEventFunction)&PMFPanel::OnRightUp,0,this);
	staticBitmap->Connect(XRCID("ID_STATICBITMAP1"),wxEVT_LEFT_UP,(wxObjectEventFunction)&PMFPanel::OnLeftUp,0,this);
	staticBitmap->Connect(XRCID("ID_STATICBITMAP1"),wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&PMFPanel::OnLeftDClick,0,this);

	fieldSize = blockSize = 0.0;
	pmf = NULL;
	choosenPoint = NULL;
	setNewPointLocation = wxPoint(-1,-1);
	scale = 100;
	bmp = NULL;
	scrolledWindow->SetScrollRate(1, 1);

    mframe = (void *)GetParent()->GetParent();
    pmfPopupMenu = new PMFPopupMenu(mframe, this);
    doubleClicked = false;
}


PMFPanel::~PMFPanel()
{
	//(*Destroy(PMFPanel)
	//*)
	if (bmp) delete bmp;
}


void PMFPanel::IterateAndDrawConfiguration(wxMemoryDC& dc)
{
    Element<pmf_point<double> > * iter = pmf->getFirstElement();
    while (iter) {
        pmf_point<double> * pt = iter->data;
        int x0 = int(pt->x * double(scale));
        int y0 = int(pt->y * double(scale));
        if (pt->n1) {
            int x1 = int(pt->n1->x * double(scale));
            int y1 = int(pt->n1->y * double(scale));
            dc.DrawLine(x0, y0, x1, y1);
        }
        if (pt->n2) {
            int x2 = int(pt->n2->x * double(scale));
            int y2 = int(pt->n2->y * double(scale));
            dc.DrawLine(x0, y0, x2, y2);
        }
        iter = iter->next;
    }
}


bool PMFPanel::DrawGeneratedPMF(bool configurationLowerLayer = true)
{
    if (pmf == NULL  ||  bmp == NULL)  return false;

    wxMemoryDC dc(*bmp);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    //dc.SelectObject(wxNullBitmap);

    if (configurationLowerLayer) IterateAndDrawConfiguration(dc);

    if (setNewPointLocation.x != -1  &&  setNewPointLocation.y != -1)
    {
        wxColor cc(128,128,255);
        //wxPen pen(*wxRED, 3); // red pen of width 1
        wxPen pen(cc, 3);
        dc.SetPen(pen);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawCircle(setNewPointLocation.x, setNewPointLocation.y, 5);
        dc.DrawCircle(setNewPointLocation.x, setNewPointLocation.y, 1);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawPoint(setNewPointLocation);
        dc.SetPen(wxNullPen);
        dc.SetBrush(wxNullBrush);
    }
    else if (choosenPoint != NULL)
    {
        int x0 = int(choosenPoint->x * double(scale));
        int y0 = int(choosenPoint->y * double(scale));

        wxMemoryDC dc(*bmp);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        IterateAndDrawConfiguration(dc);

        wxColor cc(128,128,255);
        //wxPen pen(*wxRED, 3); // red pen of width 1
        wxPen pen(cc, 3);
        dc.SetPen(pen);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawCircle(x0, y0, 5);
        dc.DrawCircle(x0, y0, 1);

        wxColor cb(0,0,0);
        wxPen blackPen(cb, 2);
        dc.SetPen(blackPen);
        if (choosenPoint->n1) {
            int x1 = int(choosenPoint->n1->x * double(scale));
            int y1 = int(choosenPoint->n1->y * double(scale));
            dc.DrawLine(x0, y0, x1, y1);
        }
        if (choosenPoint->n2) {
            int x2 = int(choosenPoint->n2->x * double(scale));
            int y2 = int(choosenPoint->n2->y * double(scale));
            dc.DrawLine(x0, y0, x2, y2);
        }

        dc.DrawPoint(x0, y0);
        dc.SetPen(wxNullPen);
        dc.SetBrush(wxNullBrush);
    }

    if (! configurationLowerLayer) IterateAndDrawConfiguration(dc);

    staticBitmap->SetBitmap(*bmp);
    staticBitmap->Refresh();
    return true;
}


double PMFPanel::GeneratePMF(time_t sseed = 0)
{
    struct timeb tbeg, tend;

    if (pmf) delete pmf;
    pmf = new PMF<double>(fieldSize, fieldSize);
    pmf->SetSeed(sseed);

    ftime(&tbeg);
    pmf->Generate(blockSize);
    ftime(&tend);

    double genTime = tend.time - tbeg.time;
    genTime += ((tend.millitm - tbeg.millitm) * 0.001);
    return genTime;
}


void PMFPanel::SetParameters(double fSize, double bSize, long sscale)
{
    if (fSize >= 0.0)
    {
        fieldSize = fSize;
        blockSize = bSize;
        scale = sscale;

        int  width = int(double(sscale) * fieldSize);
        int height = int(double(sscale) * fieldSize);

        if (bmp) delete bmp;
        bmp = new wxBitmap(width, height);
        staticBitmap->SetBitmap(*bmp);
        scrolledWindow->FitInside();
    }
}


void PMFPanel::OnRightUp(wxMouseEvent& event)
{
    //((mainFrame *) mframe)->SetStatusText(wxString::Format(wxT(" Clicked at (%d,%d)"), event.GetX()+1, event.GetY()+1), 0);
    int offsetX, offsetY;
    scrolledWindow->GetViewStart(&offsetX, &offsetY);
    wxPoint popupPoint = event.GetPosition();
    popupPoint.x -= offsetX;
    popupPoint.y -= offsetY;

    pmfPopupMenu->Reinitialize();
    PopupMenu(pmfPopupMenu, popupPoint);
}


void PMFPanel::AddBirthPointToPMF(double xx, double yy)
{
    AddPointDialog gDialog(this);
    gDialog.SetPointCoordinates(xx, yy);
    gDialog.ShowModal();
    if ( gDialog.isOk() )
    {
        wxString strX = (gDialog.CoordinateXTextCtrl)->GetValue();
        wxString strY = (gDialog.CoordinateYTextCtrl)->GetValue();
        wxString strB = (gDialog.BlockSizeTextCtrl)->GetValue();
        wxString strA = (gDialog.RadianAngleTextCtrl)->GetValue();
        bool check = (gDialog.UseBlocksCheckBox)->GetValue();
        double xx, yy, bsize, angle;

        if (strX.ToDouble(&xx) && strY.ToDouble(&yy) && strB.ToDouble(&bsize) && strA.ToDouble(&angle))
            if (bsize >= 0.0)
            {
                if (!check) bsize = 0.0;
                // TODO :
                wxString ss = wxString::Format(wxT(" point ( %.3lf, %.3lf ), block = %.3lf"), xx, yy, bsize);
                ss += wxString::Format(wxT(",   angle = %.3lf,   sinL = %.3lf ,   cosL = %.3lf"), angle, sin(angle), cos(angle));

                //pmfPanel->AddBirthPointToPMF(xx, yy, angle);
                pmf->AddBirthPoint(xx, yy, angle);
                DrawGeneratedPMF(false);
                ClearConfigurationSelection();

                ((mainFrame *) mframe)->SetStatusText( ss, 0);
            }
            else {
                wxMessageBox(_("Block size should be positive!"), _("Wrong values!"));
            }
    }
}


bool PMFPanel::SavePMF(wxString path, int index)
{
    bool result = false;

    wxCSConv wxConvISO8859_2(wxT("iso8859-2"));
    const wxCharBuffer wxbuf = wxConvISO8859_2.cWC2MB(path);
    const char * filepath = (const char *)wxbuf;

    switch (index)
    {
        case  1:  result = pmf->SaveConfigurationAsSVG(filepath, scale, 0.15);  break;
        default:  result = pmf->SaveConfiguration(filepath);
    }
    return result;
}


bool PMFPanel::LoadPMF(wxString path)
{
    wxCSConv wxConvISO8859_2(wxT("iso8859-2"));
    const wxCharBuffer wxbuf = wxConvISO8859_2.cWC2MB(path);
    const char * filepath = (const char *)wxbuf;

    pmf = new PMF<double>(0.0, 0.0);
    pmf->LoadConfiguration(filepath);

        //wxMessageBox(wxString::Format(_("PMF width = %lf"), pmf->GetPMFWidth()), _("debug"));
        int  width = int(double(scale) * pmf->GetPMFWidth());
        int height = int(double(scale) * pmf->GetPMFHeight());

        if (bmp) delete bmp;
        bmp = new wxBitmap(width, height);
        staticBitmap->SetBitmap(*bmp);
        scrolledWindow->FitInside();

    return true;
}


#define DIST(PT1, PT2) (sqrt(((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y)))
void PMFPanel::OnLeftUp(wxMouseEvent& event)
{
    if (doubleClicked) { doubleClicked = false;  return; }
    //wxString qqq = (doubleClicked) ? doubleClicked = false, wxT("DOUBLE") : wxT("SINGLE");
    //wxMessageBox(qqq, wxT("test"));
    //((mainFrame *) mframe)->SetStatusText(qqq, 0);
    //return;
    double xx = double(event.GetX()+1) / double(scale);
    double yy = double(event.GetY()+1) / double(scale);
    pmf_point<double> * pt = pmf->FindClosestTo(xx, yy);

    choosenPoint = pt;
    setNewPointLocation = wxPoint(-1,-1);

    wxString wstr;
    if (pt)
    {
        DrawGeneratedPMF();

        wstr = wxString::Format(wxT(" Id = %li, ( %.3lf, %.3lf ), "), pt->id, pt->x, pt->y);
        switch (pt->type) {
            case   PT_BIRTH_DOWN: wstr += wxT("BRITH_DOWN"); break;
            case   PT_BIRTH_LEFT: wstr += wxT("BIRTH_LEFT"); break;
            case PT_BIRTH_NORMAL: wstr += wxT("BIRTH_NORM"); break;
            case     PT_BIRTH_UP: wstr += wxT("BIRTH_UP"); break;
            case       PT_BORDER: wstr += wxT("BORDER"); break;
            case PT_INTERSECTION: wstr += wxT("INTERSECT"); break;
            case       PT_UPDATE: wstr += wxT("UPDATE"); break;
            default:  wstr += wxT("UNKNOWN");
        }
        wstr += wxString::Format(wxT(" [%li] [%li]"), (pt->n1 ? pt->n1->id : 0), (pt->n2 ? pt->n2->id : 0));
        wstr += wxString::Format(wxT(", { %.3lf, %.3lf }"), pt->l1, pt->l2);
        wstr += wxString::Format(wxT(", oldId=%li"), pt->oid);
        wstr += wxString::Format(wxT("  // %.3lf : %.3lf"), (pt->n1) ? DIST(pt, pt->n1) : -1.0, (pt->n2) ? DIST(pt, pt->n2) : -1.0);
    }
    else {
        wstr = wxString::Format(wxT(" Left clicked at (%d,%d)"), event.GetX(), event.GetY());
    }

    ((mainFrame *) mframe)->SetStatusText(wstr, 0);
}
#undef DIST


void PMFPanel::OnLeftDClick(wxMouseEvent& event)
{
    doubleClicked = true;
    //wxMessageBox(_("DClick"), _("info"));
    //return;
    choosenPoint = NULL;
    setNewPointLocation = event.GetPosition();

    double xx = double(setNewPointLocation.x+1) / double(scale);
    double yy = double(setNewPointLocation.y+1) / double(scale);
    pmfPopupMenu->SetPoint(xx, yy);

    DrawGeneratedPMF();
}


pmf_point<double> * PMFPanel::GetSelectedPMFPoint() { return choosenPoint; }

wxPoint & PMFPanel::GetNewPMFPointLocation() { return setNewPointLocation; }

long PMFPanel::GetScale() { return scale; }

void PMFPanel::ClearConfigurationSelection()
{
    choosenPoint = NULL;
    setNewPointLocation = wxPoint(-1,-1);
}


#define __OP_UPDATE 0
#define __OP_REMOVE 1
void PMFPanel::ExecutePointModificationDialog(long op, wxString & title)
{
    ModPointDialog gDialog(this);
    gDialog.SetTitle(title);
    gDialog.SetPMFPointData(choosenPoint);
    gDialog.ShowModal();
    if ( gDialog.isOk() )
    {
        wxString strID = (gDialog.pointIdTextCtrl)->GetValue();
        wxString strBS = (gDialog.blockSizeTextCtrl)->GetValue();
        wxString strA = (gDialog.radianAngleTextCtrl)->GetValue();
        bool check = (gDialog.useBlocksCheckBox)->GetValue();
        long pointID;
        double bsize, angle;

        if (strID.ToLong(&pointID) && strBS.ToDouble(&bsize) && strA.ToDouble(&angle))
            if (bsize >= 0.0)
            {
                if (!check) bsize = 0.0;
                // TODO :
                wxString ss = wxString::Format(wxT(" point ( %.3lf, %.3lf ), block = %.3lf"), choosenPoint->x, choosenPoint->y, bsize);
                ss += wxString::Format(wxT(",   angle = %.3lf,   sinL = %.3lf ,   cosL = %.3lf"), angle, sin(angle), cos(angle));

                switch (op)
                {
                    case __OP_UPDATE :
                                        pmf->UpdatePointVelocity(pointID, angle);
                                        break;
                    case __OP_REMOVE :
                                        pmf->RemoveBirthPoint(pointID, angle);
                                        break;
                }

                ClearConfigurationSelection();
                DrawGeneratedPMF(false);
                ClearConfigurationSelection();

                ((mainFrame *) mframe)->SetStatusText( ss, 0);
            }
            else {
                wxMessageBox(_("Block size should be positive!"), _("Wrong values!"));
            }
    }
}


void PMFPanel::UpdatePointInsidePMF()
{
    wxString title = wxT("Update point velocity ...");
    ExecutePointModificationDialog(__OP_UPDATE, title);
}


void PMFPanel::RemovePointFromPMF()
{
    wxString title = wxT("Remove birth point ...");
    ExecutePointModificationDialog(__OP_REMOVE, title);
}
#undef __OP_UPDATE
#undef __OP_REMOVE
