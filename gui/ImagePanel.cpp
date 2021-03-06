//(*InternalHeaders(ImagePanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/msgdlg.h>

#include "macros.hpp"
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


bool ImagePanel::SaveFile(wxString path, int type = wxBITMAP_TYPE_PNG)
{
    wxImage img = bmp->ConvertToImage();
    return img.SaveFile(path, type);
}


void ImagePanel::Reset()
{
    if (bmp) delete bmp;
    bmp = new wxBitmap(image);
    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
}


void ImagePanel::Rescale(int w, int h)
{
    if (bmp->GetWidth() != w  ||  bmp->GetHeight() != h)
    {
        wxImage img = bmp->ConvertToImage();
        img.Rescale(w, h);

        if (bmp) delete bmp;
        bmp = new wxBitmap(img);
        staticBitmap->SetBitmap( *bmp );
        scrolledImageWindow->FitInside();
    }
}


void ImagePanel::SetVisibleImage()
{
    image = bmp->ConvertToImage();

    if (bmp) delete bmp;
    bmp = new wxBitmap(image);
    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
}


void ImagePanel::ConvertToGreyscale(double lr, double lg, double lb)
{
    wxImage img = image.ConvertToGreyscale(lr, lg, lb);

	if (bmp) delete bmp;
	bmp = new wxBitmap(img);
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
    //* Getting max value of gradient
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


void ImagePanel::GaussBlur()
{
    wxImage img = image.ConvertToGreyscale();

    for (int x = 1; x < img.GetWidth()-1; x++) {
        for (int y = 1; y < img.GetHeight()-1; y++) {
            int pix1 = img.GetGreen(x-1,y-1);
            int pix2 = img.GetGreen(x,y-1);
            int pix3 = img.GetGreen(x+1,y-1);
            int pix4 = img.GetGreen(x-1,y);
            int pix5 = img.GetGreen(x,y);
            int pix6 = img.GetGreen(x+1,y);
            int pix7 = img.GetGreen(x-1,y+1);
            int pix8 = img.GetGreen(x,y+1);
            int pix9 = img.GetGreen(x+1,y+1);

            int value = pix1 + 2 * pix2 + pix3
                        + 2 * pix4 + 4 * pix5 + 2 * pix6
                        + pix7 + 2 * pix8 + pix9;
            value /= 16;

            img.SetRGB(x, y, value, value, value);
        }
    }
	//*
	if (bmp) delete bmp;
	bmp = new wxBitmap(img);
    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
    //*/
}


void ImagePanel::Contrast()
{
    wxImage img = image.ConvertToGreyscale();
    double maxval = 256.;
    double inv256 = 1. / maxval;

    REP(x,img.GetWidth()) REP(y,img.GetHeight())
    {
        double normval = img.GetGreen(x,y) * inv256;
        normval = (normval < 0.5) ? normval*normval : 1.-(1.-normval)*(1.-normval);
        int pix = int(normval * maxval);
        img.SetRGB(x,y,pix,pix,pix);
    }
	//*
	if (bmp) delete bmp;
	bmp = new wxBitmap(img);
    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
    //*/
}


#include "grayscaleimage.hpp"

void ImagePanel::PresentPMF(PMF<double> * pmf)
{
    wxImage img = image.ConvertToGreyscale();
    GrayScaleImage gsimg(img.GetWidth(), img.GetHeight());

    int div = 2;
    int rest = 255 - 255 / div;
    for (int x = 0; x < img.GetWidth(); x++) {
        for (int y = 0; y < img.GetHeight(); y++) {
            int rr = img.GetRed(x,y);
            int gg = img.GetGreen(x,y);
            int bb = img.GetBlue(x,y);
            rr = rest + rr / div;
            gg = rest + gg / div;
            bb = rest + bb / div;
            img.SetRGB(x, y, rr, gg, bb);

            gsimg.PutPixelValue(x, y, img.GetGreen(x,y));
        }
    }

	if (bmp) delete bmp;
	bmp = new wxBitmap(img);

    if (pmf) {
        wxMemoryDC dc(*bmp);

        //wxColor cc(16,8,128);
        wxColor cc(34,177,76);
        //wxPen pen(cc, 1);
        wxPen pen(*wxBLUE, 1);
        dc.SetPen(pen);

        for(Element<pmf_point<double> > * iter = pmf->getFirstElement(); iter; iter = iter->next) {
            pmf_point<double> * pt = iter->data;

            double pmfw_1 = 1 / pmf->GetPMFWidth();
            double pmfh_1 = 1 / pmf->GetPMFHeight();

            int ix = int(pt->x * pmfw_1 * img.GetWidth());
            int iy = int(pt->y * pmfh_1 * img.GetHeight());
            if (pt->n1) {
                int ix1 = int(pt->n1->x * pmfw_1 * img.GetWidth());
                int iy1 = int(pt->n1->y * pmfh_1 * img.GetHeight());

                dc.DrawLine(ix, img.GetHeight()-1 - iy, ix1, img.GetHeight()-1 - iy1);
            }
            if (pt->n2) {
                int ix2 = int(pt->n2->x * pmfw_1 * img.GetWidth());
                int iy2 = int(pt->n2->y * pmfh_1 * img.GetHeight());

                dc.DrawLine(ix, img.GetHeight()-1 - iy, ix2, img.GetHeight()-1 - iy2);
            }
        }

        double eng = gsimg.CalculateScanLinesEnergy(pmf);
        wxString wstr = wxString::Format(wxT(" [ ENERGY ] : %.3lf"), eng);
        ((mainFrame *) mframe)->SetStatusText(wstr, 0);
    }
    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
}


double asd(double gx, double gy) {
    if (gx == 0.0  &&  gy == 0.0)  return 0.0;

    double aaa = atan2(gx, gy);
    if (aaa < 0.0) aaa += (2.0 * M_PI);
    if (aaa < M_PI_2)  aaa += M_PI;
    if (aaa >= 1.5 * M_PI)  aaa -= M_PI;
    return aaa;
}


void ImagePanel::OnLeftUp(wxMouseEvent& event)
{
    int x = event.GetX() + 1;
    int y = event.GetY() + 1;
    wxString wstr = wxString::Format(wxT("click=(%li,%li) [%i,%i,%i] :: "), x, y, image.GetRed(x,y), image.GetGreen(x,y), image.GetBlue(x,y));

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
        wstr += wxString::Format(wxT(" grad = %.2lf"), sqrt(G));
        #define radians2degree(X) (X * 180.0 / M_PI)
        /*
        if (G > 0.0) {
            double angle = atan(Gx / Gy);
            wstr += wxString::Format(wxT("  theta = %.2lf, %.0lf"), angle, radians2degree(angle));
        }
        //*/
        double aaa = asd(Gx, Gy);
        wstr += wxString::Format(wxT("  theta2 = %.2lf {%.0lf}"), aaa, radians2degree(aaa));
    }
    else {
        wstr = _(" border pixel ");
    }

    ((mainFrame *) mframe)->SetStatusText(wstr, 0);
    return;
}

