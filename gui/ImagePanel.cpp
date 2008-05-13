//(*InternalHeaders(ImagePanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/bitmap.h>
#include <wx/image.h>
#include "ImagePanel.h"

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
	bmp = NULL;
    scrolledImageWindow->SetScrollRate(1, 1);
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

