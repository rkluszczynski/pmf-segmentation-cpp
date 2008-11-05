//(*InternalHeaders(ImagePanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/msgdlg.h>

#include <cmath>
#include "ImagePanel.h"
#include "mainFrame.h"


IMPLEMENT_CLASS(ImagePanel,wxPanel)

//(*IdInit(ImagePanel)
const long ImagePanel::ID_STATICBITMAP1 = wxNewId();
const long ImagePanel::ID_SCROLLEDWINDOW1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ImagePanel,wxPanel)
	//(*EventTable(ImagePanel)
	//*)
END_EVENT_TABLE()


ImagePanel::ImagePanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ImagePanel)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("id"));
	SetBackgroundColour(wxColour(240,240,255));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	scrolledImageWindow = new wxScrolledWindow(this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL, _T("ID_SCROLLEDWINDOW1"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableCol(2);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	staticBitmap = new wxStaticBitmap(scrolledImageWindow, ID_STATICBITMAP1, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T("ID_STATICBITMAP1"));
	staticBitmap->SetBackgroundColour(wxColour(240,240,240));
	FlexGridSizer1->Add(staticBitmap, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	scrolledImageWindow->SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(scrolledImageWindow);
	FlexGridSizer1->SetSizeHints(scrolledImageWindow);
	BoxSizer1->Add(scrolledImageWindow, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
    staticBitmap->Connect(ID_STATICBITMAP1,wxEVT_LEFT_UP,(wxObjectEventFunction)&ImagePanel::OnLeftUp,0,this);

	bmp = NULL;
    scrolledImageWindow->SetScrollRate(1, 1);

    mframe = (void *)GetParent()->GetParent();
}


ImagePanel::~ImagePanel()
{
	//(*Destroy(ImagePanel)
	//*)
	if (bmp) delete bmp;
}


void ImagePanel::LoadFile(wxString path)
{
	image.LoadFile(path);
	if (bmp) delete bmp;
	bmp = new wxBitmap(image);

    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
}


void ImagePanel::CalculateGradient()
{
    wxMessageDialog dlg1(this, _("equal"), _("Info ..."), wxOK);
    wxMessageDialog dlg2(this, _("not equal"), _("Info ..."), wxOK);

    wxImage img = image.ConvertToGreyscale();
    img.SetRGB(4,4,128,128,128);
    /*
    if (img.GetGreen(2,2) == img.GetRed(2,2))
        dlg1.ShowModal();
    else
        dlg2.ShowModal();
    */
    double * grads = new double[img.GetWidth()*img.GetHeight()];
    if (! grads) dlg1.ShowModal();
    #define INDEX(X,Y) ((X)*img.GetHeight()+(Y))
    //*
    for (int x = 0; x < img.GetWidth(); x++) {
        for (int y = 0; y < img.GetHeight(); y++) {
            if (x == 0 || y == 0 || x == img.GetWidth()-1 || y == img.GetHeight()-1) {
                grads[INDEX(x,y)] = 0.0;
            }
            else {
                double Gx = img.GetGreen(x+1,y-1) + 2.0 * img.GetGreen(x+1,y) + img.GetGreen(x+1,y+1)
                    - img.GetGreen(x-1,y-1) - 2.0 * img.GetGreen(x-1,y) - img.GetGreen(x-1,y+1);
                double Gy = img.GetGreen(x-1,y-1) + 2.0 * img.GetGreen(x,y-1) + img.GetGreen(x+1,y-1)
                    - img.GetGreen(x-1,y+1) - 2.0 * img.GetGreen(x,y+1) - img.GetGreen(x+1,y+1);
                double G = sqrt(Gx * Gx + Gy * Gy);
                grads[INDEX(x,y)] = G;
            }
        }
    }
    //* Getting max
    double maks = 0.0;
    for (int i = 0; i < img.GetWidth()*img.GetHeight(); i++)
        if (grads[i] > maks)  maks = grads[i];
    // Rescaling
    for (int i = 0; i < img.GetWidth()*img.GetHeight(); i++)
        grads[i] = 255.0 * grads[i] / maks;
    for (int x = 0; x < img.GetWidth(); x++)
        for (int y = 0; y < img.GetHeight(); y++) {
            int v = int(grads[INDEX(x,y)]);
            img.SetRGB(x,y,v,v,v);
        }
    //*/

	//*
	if (bmp) delete bmp;
	bmp = new wxBitmap(img);
    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
    //*/
}

void ImagePanel::OnLeftUp(wxMouseEvent& event)
{
    int x = event.GetX() + 1;
    int y = event.GetY() + 1;
    wxString wstr = wxString::Format(wxT("click=(%li,%li)  :: "), x, y);

    if (! (x == 0 || y == 0 || x == image.GetWidth()-1 || y == image.GetHeight()-1))
    {
        //double Gx = image.GetGreen(x+1,y-1) + 2.0 * image.GetGreen(x+1,y) + image.GetGreen(x+1,y+1)
        //    - image.GetGreen(x-1,y-1) - 2.0 * image.GetGreen(x-1,y) - image.GetGreen(x-1,y+1);
        //double Gy = image.GetGreen(x-1,y-1) + 2.0 * image.GetGreen(x,y-1) + image.GetGreen(x+1,y-1)
        //    - image.GetGreen(x-1,y+1) - 2.0 * image.GetGreen(x,y+1) - image.GetGreen(x+1,y+1);

        double Gx = image.GetGreen(x+1,y+1) + 2.0 * image.GetGreen(x+1,y) + image.GetGreen(x+1,y-1)
            - image.GetGreen(x-1,y+1) - 2.0 * image.GetGreen(x-1,y) - image.GetGreen(x-1,y-1);
        double Gy = image.GetGreen(x-1,y+1) + 2.0 * image.GetGreen(x,y+1) + image.GetGreen(x+1,y+1)
            - image.GetGreen(x-1,y-1) - 2.0 * image.GetGreen(x,y-1) - image.GetGreen(x+1,y-1);

        double G = Gy * Gy + Gx * Gx;
        wstr += wxString::Format(wxT(" grad = %.3lf"), sqrt(G));
        if (G > 0.0) {
            double angle = atan(Gy / Gx);
            #define radians2degree(X) (X * 180.0 / M_PI)
            wstr += wxString::Format(wxT("  theta = %.3lf, %.3lf"), angle, radians2degree(angle));
        }
    }
    else {
        wstr = _(" border pixel ");
    }

    ((mainFrame *) mframe)->SetStatusText(wstr, 0);
    return;
    /*

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
    //*/
}

