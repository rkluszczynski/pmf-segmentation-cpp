#include "mainFrame.h"
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/progdlg.h>
#include "GenerateDialog.h"
#include "ImagePanel.h"
#include "PMFPanel.h"
#include "AddPointDialog.h"

//(*InternalHeaders(mainFrame)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(mainFrame)
//*)

BEGIN_EVENT_TABLE(mainFrame,wxFrame)
	//(*EventTable(mainFrame)
	//*)
END_EVENT_TABLE()


mainFrame::mainFrame(wxWindow* parent)
{
	//(*Initialize(mainFrame)
	wxXmlResource::Get()->LoadObject(this,parent,_T("mainFrame"),_T("wxFrame"));
	StaticBitmap1 = (wxStaticBitmap*)FindWindow(XRCID("ID_STATICBITMAP1"));
	myScrolledWindow = (wxScrolledWindow*)FindWindow(XRCID("ID_SCROLLEDWINDOW1"));
	myHtmlWindow = (wxHtmlWindow*)FindWindow(XRCID("ID_HTMLWINDOW1"));
	mySplitterWindow = (wxSplitterWindow*)FindWindow(XRCID("ID_SPLITTERWINDOW1"));
	myNotebook = (wxNotebook*)FindWindow(XRCID("ID_NOTEBOOK1"));
	saveImageMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_SAVEIMAGE_MENUITEM")) : NULL;
	savePMFMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_SAVEPMF_MENUITEM")) : NULL;
	resetMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_RESET_MENUITEM")) : NULL;
	saveItMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_SAVE_IT_MENUITEM")) : NULL;
	grayscaleMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_GRAYSCALE_MENUITEM")) : NULL;
	gaussBlurMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_GAUSS_BLUR_MENUITEM")) : NULL;
	rescaleMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_RESCALE_MENUITEM")) : NULL;
	progressMenuItem = GetMenuBar() ? (wxMenuItem*)GetMenuBar()->FindItem(XRCID("ID_MENUITEM1")) : NULL;
	StatusBar1 = (wxStatusBar*)FindWindow(XRCID("ID_STATUSBAR1"));

	Connect(XRCID("ID_SPLITTERWINDOW1"),wxEVT_COMMAND_SPLITTER_DOUBLECLICKED,(wxObjectEventFunction)&mainFrame::OnMySplitterWindowDClick);
	Connect(XRCID("ID_NOTEBOOK1"),wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&mainFrame::OnMyNotebookPageChanged);
	Connect(XRCID("ID_LOADIMAGE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnLoadImageMenuItemSelected);
	Connect(XRCID("ID_LOADPMF_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnLoadPMFMenuItemSelected);
	Connect(XRCID("ID_SAVEIMAGE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnSaveImageMenuItemSelected);
	Connect(XRCID("ID_SAVEPMF_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnSavePMFMenuItemSelected);
	Connect(XRCID("ID_CLOSE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnCloseImageMenuItemSelected);
	Connect(XRCID("ID_QUIT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnQuit);
	Connect(XRCID("ID_GENERATE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnGenerateMenuItemSelected);
	Connect(XRCID("ID_REGENERATE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnRegenerateMenuItemSelected);
	Connect(XRCID("ID_ADDPOINT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnAddPointMenuItemSelected);
	Connect(XRCID("ID_UPDPOINT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnUpdatePointMenuItemSelected);
	Connect(XRCID("ID_REMPOINT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnRemovePointMenuItemSelected);
	Connect(XRCID("ID_ADDSEGMENT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnAddSegmentMenuItemSelected);
	Connect(XRCID("ID_RESET_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnResetMenuItemSelected);
	Connect(XRCID("ID_SAVE_IT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnSaveItMenuItemSelected);
	Connect(XRCID("ID_GRAYSCALE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnGrayscaleMenuItemSelected);
	Connect(XRCID("ID_GRADIENT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnGradientMenuItemSelected);
	Connect(XRCID("ID_GAUSS_BLUR_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnGaussBlurMenuItemSelected);
	Connect(XRCID("ID_CONTRAST_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnContrastMenuItemSelected);
	Connect(XRCID("ID_RESCALE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnRescaleMenuItemSelected);
	Connect(XRCID("ID_PRESENT_PMF_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnPresentPMFMenuItemSelected);
	Connect(XRCID("ID_LOG_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnViewInfosMenuItemSelected);
	Connect(XRCID("ID_MENUITEM1"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnShowProgressMenuItemSelected);
	Connect(XRCID("ID_MENUITEM3"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnAbout);
	//*)
	//Connect(wxID_ANY,wxEVT_SIZE,(wxObjectEventFunction)&mainFrame::OnMyScrolledWindowSize);

    htmlWindowHeight = 100;

    mySplitterWindow->SetSashGravity(1.0);
    mySplitterWindow->SetSashPosition(GetSize().GetHeight() - 2*htmlWindowHeight);
    mySplitterWindow->Refresh();
	SetStatusText(_T(" (c) 2008"), 2);

    GetMenuBar()->EnableTop(GetMenuBar()->FindMenu(_("Image")), false);
}


mainFrame::~mainFrame()
{
	//(*Destroy(mainFrame)
	//*)
}


void mainFrame::OnQuit(wxCommandEvent& event)
{
    wxMessageDialog dlg(this, _("Are you sure to quit?"), _("Quitting ..."), wxOK | wxCANCEL | wxICON_WARNING);
    if ( dlg.ShowModal() == wxID_OK )
    {
        Destroy();
    }
}


void mainFrame::OnAbout(wxCommandEvent& event)
{
    wxString wxbuild(wxVERSION_STRING);
#if defined(__WXMSW__)
    wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
    wxbuild << _T("-Mac");
#elif defined(__UNIX__)
    wxbuild << _T("-Linux");
#endif
#if wxUSE_UNICODE
    wxbuild << _T("-Unicode build");
#else
    wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    wxbuild << _T("\n\nBuilt on: ");
    wxbuild << _T(__DATE__);
    wxbuild << _T(" (") << _T(__TIME__) << _T(")");
    wxMessageBox(wxbuild, _("Welcome to..."));
}


/*
I have a ScrolledWindow. But I programmatically add some controls
to it. But it doesn't show a scrollbar until I resize the frame. I
couldn't find a way to show scrollbars. Maybe it can be done by firing
size event. But do you know any easy way of doing this ? (on windows
2.8.0.1)
>
If you're not using a sizer then you need to SetVirtualSize and
SetScrollRate. If you are using a sizer then you need to trigger a
layout. If calling Layout doesn't do it then try SendSizeEvent.
>
It's in a sizer but Layout didn't do anything. But SendSizeEvent made
it work. Thanks to the masters, one more windows issue is solved :-)
*/
void mainFrame::OnMenuItem1Selected(wxCommandEvent& event)
{
    wxSize sz = myScrolledWindow->GetClientSize();
    myScrolledWindow->SetVirtualSize(sz.GetWidth()+100, sz.GetHeight()+10);
    myScrolledWindow->SetScrollRate(10, 10);
}



void mainFrame::GeneratingPMFAction(wxCommandEvent& event, PMFPanel * pmf = NULL)
{
    GenerateDialog gDialog(this);
    gDialog.ShowModal();
    if ( gDialog.isOk() ) {
        wxString str1 = (gDialog.FieldSizeTextCtrl)->GetValue();
        wxString str2 = (gDialog.BlockSizeTextCtrl)->GetValue();
        wxString str3 = (gDialog.ScaleTextCtrl)->GetValue();
        wxString str4 = (gDialog.SeedTextCtrl)->GetValue();
        bool check = (gDialog.UseBlocksCheckBox)->GetValue();
        double fieldSize, blockSize;
        long scale, seed;
        if (str1.ToDouble(&fieldSize) && str2.ToDouble(&blockSize) && str3.ToLong(&scale) && str4.ToLong(&seed))
        {
            if (fieldSize > 0.0  &&  blockSize > 0.0  &&  scale > 0)
            {
                bool newPanel = (pmf == NULL) ? true : false;
                if (newPanel) {
                    pmf = new PMFPanel(myNotebook);
                    myNotebook->AddPage(pmf, wxString::Format(wxT("[ PMF ] : Unnamed%d"), myNotebook->GetPageCount()), false, -1);
                    myNotebook->Refresh();
                }

                pmf->SetParameters(fieldSize, (check) ? blockSize : 0.0f, scale);
                double genTime = pmf->GeneratePMF(seed);
                if (! pmf->DrawGeneratedPMF(true)) {
                    ;
                }
                SetStatusText( wxString::Format(wxT("Generation time : %.3lf sec."), genTime), 0);
            }
            else {
                wxMessageBox(_("Field and block sizes should be positive!"), _("Wrong values!"));
            }
        }

    }
}


void mainFrame::OnGenerateMenuItemSelected(wxCommandEvent& event)
{
    GeneratingPMFAction(event);
    myNotebook->SetSelection(myNotebook->GetPageCount() - 1);
}


/// Paint the background
void mainFrame::PaintScrolledWindowBackground(wxDC& dc)
{
    wxColour backgroundColour = myScrolledWindow->GetBackgroundColour();
    if (!backgroundColour.Ok())
        backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
    dc.SetBrush(wxBrush(backgroundColour));
    //dc.SetPen(wxPen(backgroundColour, 1));
    //dc.SetPen(*wxBLACK_DASHED_PEN);
    wxRect windowRect(wxPoint(0, 0), myScrolledWindow->GetVirtualSize());//GetClientSize());
    myScrolledWindow->CalcUnscrolledPosition(windowRect.x, windowRect.y, & windowRect.x, & windowRect.y);
    dc.DrawRectangle(windowRect);
}


void mainFrame::OnMyScrolledWindowPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(myScrolledWindow);
    PrepareDC(dc);
    PaintScrolledWindowBackground(dc);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxRED_BRUSH);
    wxSize sz = myScrolledWindow->GetVirtualSize(); //GetClientSize();
    wxCoord w = 100, h = 50;
    int x = wxMax(0, (sz.GetX() - w)/2);
    int y = wxMax(0, (sz.GetY() - h)/2);
    wxRect rectToDraw(x, y, w, h);
    if (myScrolledWindow->IsExposed(rectToDraw)) dc.DrawRectangle(rectToDraw);
}


void mainFrame::OnLoadImageMenuItemSelected(wxCommandEvent& event)
{
    wxString caption = wxT("Choose a file");
    wxString wildcard = wxT("PNG files (*.png)|*.png");
    wildcard += wxT("|BMP files (*.bmp)|*.bmp");
    wildcard += wxT("|GIF files (*.gif)|*.gif");
    wxString defaultDir = wxT("../input");
    wxString defaultFilename = wxEmptyString;

    wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    dialog.SetFilterIndex(1);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();
        //int fiterIndex = dialog.GetFilterIndex();

        ImagePanel * ip = new ImagePanel(myNotebook, wxID_ANY);
        ip->LoadFile(path);
        myNotebook->AddPage(ip, wxT("[ IMAGE ] : ") + dialog.GetFilename(), false, -1);
        myNotebook->SetSelection(myNotebook->GetPageCount() - 1);
    }
}


void mainFrame::OnMyNotebookPageChanged(wxNotebookEvent& event)
{
    wxWindow * w = myNotebook->GetCurrentPage();
    bool isClassImagePanel = w->IsKindOf(CLASSINFO(ImagePanel));
    bool isClassPMFPanel = w->IsKindOf(CLASSINFO(PMFPanel));

    // PMFPanel and ImagePanel options
    GetMenuBar()->Enable(XRCID("ID_CLOSE_MENUITEM"), isClassImagePanel || isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_SAVEIMAGE_MENUITEM"), (isClassPMFPanel || isClassImagePanel));

    // Only PMFPanel options
    GetMenuBar()->Enable(XRCID("ID_SAVEPMF_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_GENERATE_MENUITEM"), ! isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_REGENERATE_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_ADDPOINT_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_UPDPOINT_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_REMPOINT_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_ADDSEGMENT_MENUITEM"), isClassPMFPanel);

    // Only ImagePanel options
    GetMenuBar()->EnableTop(GetMenuBar()->FindMenu(_("Image")), isClassImagePanel);

    // START panel options
    GetMenuBar()->Enable(XRCID("ID_LOG_MENUITEM"), !(isClassPMFPanel || isClassImagePanel));
}


void mainFrame::OnCloseImageMenuItemSelected(wxCommandEvent& event)
{
    int pageid = myNotebook->GetSelection();
    myNotebook->DeletePage(pageid);
}


void mainFrame::OnAddPointMenuItemSelected(wxCommandEvent& event)
{
    PMFPanel * pp = (PMFPanel *) myNotebook->GetCurrentPage();
    double dscale = double(pp->GetScale());
    wxPoint spt = pp->GetNewPMFPointLocation();
    std::pair<double, double> coord = pp->GetPMFSize();

    double xx, yy;
    if (spt.x == -1 && spt.y == -1)
    {
        long widthRange = long(coord.first * dscale);
        long heightRange = long(coord.second * dscale);

        xx = double(rand() % widthRange) / dscale;
        yy = double(rand() % heightRange) / dscale;
    }
    else {
        xx = double(spt.x+1) / dscale;
        yy = double(spt.y+1) / dscale;
    }
    pp->AddBirthPointToPMF(xx, yy);
}


void mainFrame::OnAddSegmentMenuItemSelected(wxCommandEvent& event)
{
    PMFPanel * pp = (PMFPanel *) myNotebook->GetCurrentPage();
    double dscale = double(pp->GetScale());
    wxPoint spt = pp->GetNewPMFPointLocation();
    std::pair<double, double> coord = pp->GetPMFSize();

    double xx, yy;
    if (spt.x == -1 && spt.y == -1)
    {
        long widthRange = long(coord.first * dscale);
        long heightRange = long(coord.second * dscale);

        xx = double(rand() % widthRange) / dscale;
        yy = double(rand() % heightRange) / dscale;
    }
    else {
        xx = double(spt.x+1) / dscale;
        yy = double(spt.y+1) / dscale;
    }
    pp->AddBirthSegmentToPMF(xx, yy);
}


void mainFrame::OnRegenerateMenuItemSelected(wxCommandEvent& event)
{
    PMFPanel * pmf = (PMFPanel *) myNotebook->GetCurrentPage();
    GeneratingPMFAction(event, pmf);
    pmf->Refresh();
}


void mainFrame::OnSavePMFMenuItemSelected(wxCommandEvent& event)
{
    wxString caption = wxT("Saving PMF Configuration ...");
    wxString wildcard = wxT("Configuration file (*.cf)|*.cf");
    wildcard += wxT("|SVG graphics file (*.svg)|*.svg");
    wildcard += wxT("|Text file (*.txt)|*.txt");
    wxString defaultDir = wxT("../output");
    wxString defaultFilename = wxEmptyString;

    wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();
        wxString file = dialog.GetFilename();
        int filterIndex = dialog.GetFilterIndex();

        wxString msg = _("Path = ") + path;
        msg += (_("\nFile = ") + file);
        msg += (_("\nFilterIndex = ") + wxString::Format(wxT("%d\n"), filterIndex));
        //wxMessageBox(msg, _("INFO"));

        PMFPanel * pp = (PMFPanel *) myNotebook->GetCurrentPage();
        if (! pp->SavePMF(path, filterIndex))  { wxMessageBox(_("Error during save"), _("Error")); }
        else  { myNotebook->SetPageText(myNotebook->GetSelection(), wxT("[ PMF ] : ") + file); }
    }
}

void mainFrame::OnLoadPMFMenuItemSelected(wxCommandEvent& event)
{
    wxString caption = wxT("Choose a configuration file");
    wxString wildcard = wxT("Configuration files (*.cf)|*.cf;*.txt");
    wxString defaultDir = wxT("../output");
    wxString defaultFilename = wxEmptyString;

    wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();
        //int fiterIndex = dialog.GetFilterIndex();

        PMFPanel * pp = new PMFPanel(myNotebook);
        myNotebook->AddPage(pp, wxT("[ PMF ] : ") + dialog.GetFilename(), false, -1);
        myNotebook->Refresh();

        pp->SetParameters(0.0, 0.0, 200);
        pp->LoadPMF(path);
        if (! pp->DrawGeneratedPMF(true)) {
            ;
        }

        myNotebook->SetSelection(myNotebook->GetPageCount() - 1);
        wxString msg = wxT("Configuration file '") + dialog.GetFilename() + wxT("' loaded");
        SetStatusText( msg , 0);
    }
}


void mainFrame::OnViewInfosMenuItemSelected(wxCommandEvent& event)
{
    bool value = myHtmlWindow->IsShown();

    myHtmlWindow->Show((value) ? false : true);
    //wxMessageBox(((value) ? _("TRUE") : _("FALSE")), _("INFO"));
    if (mySplitterWindow->IsSplit()) mySplitterWindow->Unsplit();
    else mySplitterWindow->SplitHorizontally(myScrolledWindow, myHtmlWindow);
    mySplitterWindow->SetSashPosition(mySplitterWindow->GetSize().GetHeight() - htmlWindowHeight, true);
    mySplitterWindow->Refresh();
}


void mainFrame::OnMySplitterWindowDClick(wxSplitterEvent& event)
{
    if (mySplitterWindow->IsSplit())
    {
        mySplitterWindow->SetSashPosition(mySplitterWindow->GetSize().GetHeight() - htmlWindowHeight, true);
        mySplitterWindow->Refresh();
    }
}

/*
void mainFrame::OnMyScrolledWindowSize(wxSizeEvent& event)
{
    wxTopLevelWindow::OnSize(event);
    int width, height;

    width = myScrolledWindow->GetSize().GetWidth() - 10;
    height = myScrolledWindow->GetSize().GetHeight() - 10;

    wxBitmap bbmp(StaticBitmap1->GetBitmap().ConvertToImage().Scale(width, height));
    StaticBitmap1->SetBitmap(bbmp);
    myScrolledWindow->Refresh();
}
*/


void mainFrame::OnUpdatePointMenuItemSelected(wxCommandEvent& event)
{
    PMFPanel * pp = (PMFPanel *) myNotebook->GetCurrentPage();
    pmf_point<double> * pt = pp->GetSelectedPMFPoint();

    if (pt == NULL)
    {
        long csize = pp->GetConfigurationSize();
        long rnum = rand() % csize + 1;
        pp->SetSelectedPMFPoint(rnum);
    }
    pp->UpdatePointInsidePMF();
    pp->ClearConfigurationSelection();
}



void mainFrame::OnShowProgressMenuItemSelected(wxCommandEvent& event)
{
    static const int max = 10;

    wxProgressDialog dialog(wxT("Progress dialog example"),
                            wxT("An informative message"),
                            max, // range
                            this, // parent
                            wxPD_CAN_ABORT |
                            wxPD_APP_MODAL |
                            wxPD_ELAPSED_TIME |
                            wxPD_ESTIMATED_TIME |
                            wxPD_REMAINING_TIME);
    bool cont = true;
    for ( int i = 0; i <= max; i++ )
    {
        wxSleep(1);
        if ( i == max )
            //cont = dialog.Update(i, wxT("To wszystko!"));
            cont = dialog.Pulse(wxT("That's all folks!"));
        else if ( i == max / 2 )
            //cont = dialog.Update(i, wxT("Polowa za nami!"));
            cont = dialog.Pulse(wxT("Half the way!"));
        else
            //cont = dialog.Update(i);
            cont = dialog.Pulse();
        if ( !cont )
        {
            if ( wxMessageBox(wxT("Do you really want to cancel?"),
                              wxT("Progress dialog question"),
                              wxYES_NO | wxICON_QUESTION) == wxYES )
                break;
            dialog.Resume();
        }
    }

    if ( !cont )
        wxLogStatus(wxT("Progress dialog aborted!"));
    else
        wxLogStatus(wxT("Countdown from %d finished"), max);
}


void mainFrame::OnRemovePointMenuItemSelected(wxCommandEvent& event)
{
    PMFPanel * pp = (PMFPanel *) myNotebook->GetCurrentPage();
    pmf_point<double> * pt = pp->GetSelectedPMFPoint();

    if (pt == NULL)
    {
        long csize = pp->GetConfigurationSize();
        long rnum = rand() % csize + 1;
        pp->SetSelectedPMFPoint(rnum);
    }
    pp->RemovePointFromPMF();
    pp->ClearConfigurationSelection();
}


void mainFrame::OnGradientMenuItemSelected(wxCommandEvent& event)
{
    ImagePanel * ip = (ImagePanel *) myNotebook->GetCurrentPage();
    ip->CalculateGradient();
    /*
        ImagePanel * ip = new ImagePanel(myNotebook, wxID_ANY);
        ip->LoadFile(path);
        myNotebook->AddPage(ip, wxT("[ IMAGE ] : ") + dialog.GetFilename(), false, -1);
    //*/
}

void mainFrame::OnSaveImageMenuItemSelected(wxCommandEvent& event)
{
    wxString caption = wxT("Saving Image ...");
    wxString wildcard = wxT("PNG images (*.png)|*.png");
    //wildcard += wxT("|SVG graphics file (*.svg)|*.svg");
    //wildcard += wxT("|Text file (*.txt)|*.txt");
    wxString defaultDir = wxT("../output");
    wxString defaultFilename = wxEmptyString;

    wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();
        wxString file = dialog.GetFilename();
        int filterIndex = dialog.GetFilterIndex();

        wxString msg = _("Path = ") + path;
        msg += (_("\nFile = ") + file);
        msg += (_("\nFilterIndex = ") + wxString::Format(wxT("%d\n"), filterIndex));
        //wxMessageBox(msg, _("INFO"));

        wxWindow * w = myNotebook->GetCurrentPage();
        if (w->IsKindOf(CLASSINFO(ImagePanel))) {
            ImagePanel * ip = (ImagePanel *) myNotebook->GetCurrentPage();
            bool res = ip->SaveFile(path, wxBITMAP_TYPE_PNG);

            if (! res) { wxMessageBox(_("Error during save"), _("Error")); }
            else  { myNotebook->SetPageText(myNotebook->GetSelection(), wxT("[ IMG ] : ") + file); }
        }
        else if (w->IsKindOf(CLASSINFO(PMFPanel))) {
            PMFPanel * pp = (PMFPanel *) myNotebook->GetCurrentPage();

            bool res = pp->SaveFile(path, wxBITMAP_TYPE_PNG);

            if (! res) { wxMessageBox(_("Error during save"), _("Error")); }
            else  { myNotebook->SetPageText(myNotebook->GetSelection(), wxT("[ PMF ] : ") + file); }
        }
    }
}


void mainFrame::OnGrayscaleMenuItemSelected(wxCommandEvent& event)
{
    wxWindow * w = myNotebook->GetCurrentPage();
    if (w->IsKindOf(CLASSINFO(ImagePanel))) {
        ImagePanel * ip = (ImagePanel *) myNotebook->GetCurrentPage();
        ip->ConvertToGreyscale();
    }
}

void mainFrame::OnPresentPMFMenuItemSelected(wxCommandEvent& event)
{
    wxWindow * w = myNotebook->GetCurrentPage();
    if (w->IsKindOf(CLASSINFO(ImagePanel))) {
        ImagePanel * ip = (ImagePanel *) myNotebook->GetCurrentPage();
        PMF<double> * pmf = NULL;

        wxString caption = wxT("Choose a configuration file");
        wxString wildcard = wxT("Configuration files (*.cf)|*.cf;*.txt");
        wxString defaultDir = wxT("../output");
        wxString defaultFilename = wxEmptyString;

        wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (dialog.ShowModal() == wxID_OK)
        {
            wxString path = dialog.GetPath();
            //int fiterIndex = dialog.GetFilterIndex();

            wxCSConv wxConvISO8859_2(wxT("iso8859-2"));
            const wxCharBuffer wxbuf = wxConvISO8859_2.cWC2MB(path);
            const char * filepath = (const char *)wxbuf;

            pmf = new PMF<double>(0.0, 0.0);
            pmf->LoadConfiguration(filepath);
        }
        ip->PresentPMF(pmf);
    }
}


void mainFrame::OnRescaleMenuItemSelected(wxCommandEvent& event)
{
    wxWindow * w = myNotebook->GetCurrentPage();
    if (w->IsKindOf(CLASSINFO(ImagePanel))) {
        ImagePanel * ip = (ImagePanel *) myNotebook->GetCurrentPage();
        ip->Rescale(1000, 1000);
    }
}


void mainFrame::OnResetMenuItemSelected(wxCommandEvent& event)
{
    if (myNotebook->GetCurrentPage()->IsKindOf(CLASSINFO(ImagePanel)))
        ((ImagePanel *) myNotebook->GetCurrentPage())->Reset();
}

void mainFrame::OnGaussBlurMenuItemSelected(wxCommandEvent& event)
{
    if (myNotebook->GetCurrentPage()->IsKindOf(CLASSINFO(ImagePanel)))
        ((ImagePanel *) myNotebook->GetCurrentPage())->GaussBlur();
}

void mainFrame::OnSaveItMenuItemSelected(wxCommandEvent& event)
{
    if (myNotebook->GetCurrentPage()->IsKindOf(CLASSINFO(ImagePanel)))
        ((ImagePanel *) myNotebook->GetCurrentPage())->SetVisibleImage();
}

void mainFrame::OnContrastMenuItemSelected(wxCommandEvent& event)
{
    if (myNotebook->GetCurrentPage()->IsKindOf(CLASSINFO(ImagePanel)))
        ((ImagePanel *) myNotebook->GetCurrentPage())->Contrast();
}
