#include "PMFPanel.h"
#include "mainFrame.h"
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

	fieldSize = blockSize = 0.0;
	pmf = NULL;
	scale = 100;
	bmp = NULL;
	scrolledWindow->SetScrollRate(1, 1);

    mframe = (void *)GetParent()->GetParent();
    pmfPopupMenu = new PMFPopupMenu(mframe);
}


PMFPanel::~PMFPanel()
{
	//(*Destroy(PMFPanel)
	//*)
	if (bmp) delete bmp;
}


bool PMFPanel::DrawGeneratedPMF()
{
    if (pmf == NULL  ||  bmp == NULL)  return false;

    wxMemoryDC dc(*bmp);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    //dc.SelectObject(wxNullBitmap);

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

    staticBitmap->SetBitmap(*bmp);
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
    if (fSize > 0.0)
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
    double xx = double(event.GetX()+1) / double(scale);
    double yy = double(event.GetY()+1) / double(scale);
    pmfPopupMenu->SetPoint(xx, yy);
    ((mainFrame *) mframe)->SetStatusText(wxString::Format(wxT(" Clicked at (%d,%d)"), event.GetX()+1, event.GetY()+1), 0);
    //PopupMenu(pmfPopupMenu, event.GetPosition());
    int offsetX, offsetY;
    scrolledWindow->GetViewStart(&offsetX, &offsetY);
    wxPoint popupPoint = event.GetPosition();
    popupPoint.x -= offsetX;
    popupPoint.y -= offsetY;
    //offsetX.x += staticBitmap->GetWindowBorderSize().GetWidth();
    PopupMenu(pmfPopupMenu, popupPoint);
}


void PMFPanel::AddBirthPointToPMF(double xx, double yy, double alpha = 0.0)
{
    pmf->AddBirthPoint(xx, yy, alpha);
    DrawGeneratedPMF();
    Refresh();
}


bool PMFPanel::SavePMF(wxString path, int index)
{
    bool result = false;
    std::string sstr = path.c_str();
    const char * filepath = sstr.c_str();
    wxMessageBox(_("TEST"), _("INFO"));
    switch (index)
    {
        case  1:  result = pmf->SaveConfigurationAsSVG(filepath, 0.15);  break;
        default:  result = pmf->SaveConfiguration(filepath);
    }
    return result;
}


void PMFPanel::OnLeftUp(wxMouseEvent& event)
{
    double xx = double(event.GetX()+1) / double(scale);
    double yy = double(event.GetY()+1) / double(scale);
    pmf_point<double> * pt = pmf->FindClosestTo(xx, yy);
    wxString wstr;
    if (pt) {
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
        wstr += wxString::Format(wxT(", [%li, %li]"), (pt->n1 ? pt->n1->id : 0), (pt->n2 ? pt->n2->id : 0));
        wstr += wxString::Format(wxT(", { %.3lf, %.3lf }"), pt->l1, pt->l2);
        wstr += wxString::Format(wxT(", oldId=%li"), pt->oid);
    }
    else {
        wstr = wxString::Format(wxT(" Left clicked at (%d,%d)"), event.GetX(), event.GetY());
    }
    ((mainFrame *) mframe)->SetStatusText(wstr, 0);
}
