#include "PMFPanel.h"
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

	fieldSize = blockSize = 0.0;
	pmf = NULL;
	scale = 100;
	bmp = NULL;
	scrolledWindow->SetScrollRate(1, 1);
}


PMFPanel::~PMFPanel()
{
	//(*Destroy(PMFPanel)
	//*)
	if (bmp) delete bmp;
}


bool PMFPanel::DrawGeneratedPMF(wxMemoryDC & dc)
{
    if (pmf == NULL  ||  bmp == NULL)  return false;
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
    return true;
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

        wxMemoryDC dc(*bmp);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        //dc.SelectObject(wxNullBitmap);

        pmf = new PMF<double>(fieldSize, fieldSize);
        pmf->Generate();
        if (! DrawGeneratedPMF(dc))
        {
            wxMessageBox(_("ERROR"), _("ERROR"));
        }
        staticBitmap->SetBitmap(*bmp);
        scrolledWindow->FitInside();
    }
}


void PMFPanel::OnRightUp(wxMouseEvent& event)
{
    wxMenu m_menu;
    m_menu.Append(wxID_EXIT, wxString::Format(wxT("x = %d"), event.GetX()));
    m_menu.AppendSeparator();
    m_menu.Append(wxID_EXIT, wxString::Format(wxT("y = %d"), event.GetY()));

    PopupMenu(&m_menu, event.GetPosition());
}
