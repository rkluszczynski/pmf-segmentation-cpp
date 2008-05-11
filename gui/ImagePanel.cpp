//(*InternalHeaders(ImagePanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/bitmap.h>
#include <wx/image.h>
#include "ImagePanel.h"


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
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("id"));
	SetBackgroundColour(wxColour(240,240,255));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	scrolledImageWindow = new wxScrolledWindow(this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL, _T("ID_SCROLLEDWINDOW1"));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	staticBitmap = new wxStaticBitmap(scrolledImageWindow, ID_STATICBITMAP1, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP1"));
	BoxSizer2->Add(staticBitmap, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	scrolledImageWindow->SetSizer(BoxSizer2);
	BoxSizer2->Fit(scrolledImageWindow);
	BoxSizer2->SetSizeHints(scrolledImageWindow);
	BoxSizer1->Add(scrolledImageWindow, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
	bmp = NULL;
    scrolledImageWindow->SetScrollRate(1, 1);
}

ImagePanel::~ImagePanel()
{
	//(*Destroy(ImagePanel)
	//*)
}


void ImagePanel::LoadFile(wxString path)
{
	image.LoadFile(path);
	if (bmp) delete bmp;
	bmp = new wxBitmap(image);

    staticBitmap->SetBitmap( *bmp );
    scrolledImageWindow->FitInside();
}

