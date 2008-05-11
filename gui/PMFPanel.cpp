#include "PMFPanel.h"
#include <wx/dcmemory.h>

//(*InternalHeaders(PMFPanel)
#include <wx/xrc/xmlres.h>
//*)

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
	fieldSize = blockSize = 0.0;
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

        /// TODO: generate and draw a configuration

        staticBitmap->SetBitmap(*bmp);
        scrolledWindow->FitInside();
    }
}

