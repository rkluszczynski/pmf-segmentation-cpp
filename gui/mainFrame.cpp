#include "mainFrame.h"
#include <wx/wx.h>
#include <wx/dcbuffer.h>
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
	myScrolledWindow = (wxScrolledWindow*)FindWindow(XRCID("ID_mySCROLLEDWINDOW"));
	myHtmlWindow = (wxHtmlWindow*)FindWindow(XRCID("ID_HTMLWINDOW1"));
	mySplitterWindow = (wxSplitterWindow*)FindWindow(XRCID("ID_SPLITTERWINDOW1"));
	myNotebook = (wxNotebook*)FindWindow(XRCID("ID_NOTEBOOK1"));
	savePMFMenuItem = (wxMenuItem*)FindWindow(XRCID("ID_SAVEPMF_MENUITEM"));
	StatusBar1 = (wxStatusBar*)FindWindow(XRCID("ID_STATUSBAR1"));

	myScrolledWindow->Connect(XRCID("ID_mySCROLLEDWINDOW"),wxEVT_PAINT,(wxObjectEventFunction)&mainFrame::OnMyScrolledWindowPaint,0,this);
	Connect(XRCID("ID_NOTEBOOK1"),wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&mainFrame::OnMyNotebookPageChanged);
	Connect(XRCID("ID_MENUITEM4"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnMenuItem1Selected);
	Connect(XRCID("ID_MENUITEM6"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnLoadImageMenuItemSelected);
	Connect(XRCID("ID_SAVEPMF_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnSavePMFMenuItemSelected);
	Connect(XRCID("ID_MENUITEM7"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnCloseImageMenuItemSelected);
	Connect(XRCID("ID_MENUITEM1"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnQuit);
	Connect(XRCID("ID_GENERATE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnGenerateMenuItemSelected);
	Connect(XRCID("ID_REGENERATE_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnRegenerateMenuItemSelected);
	Connect(XRCID("ID_ADDPOINT_MENUITEM"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnAddPointMenuItemSelected);
	Connect(XRCID("ID_MENUITEM5"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnViewInfosMenuItemSelected);
	Connect(XRCID("ID_MENUITEM3"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnAbout);
	//*)

	SetStatusText(_T(" (c) 2008"), 2);
}


mainFrame::~mainFrame()
{
	//(*Destroy(mainFrame)
	//*)
}


void mainFrame::OnQuit(wxCommandEvent& event)
{
    wxMessageDialog dlg(this, _("Are you sure to quit?"), _("Quitting ..."), wxOK | wxCANCEL | wxICON_QUESTION);
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
            if (fieldSize > 0.0  &&  blockSize > 0.0  &&  scale > 0)
            {
                bool newPanel = (pmf == NULL) ? true : false;
                if (newPanel) {
                    pmf = new PMFPanel(myNotebook);
                    myNotebook->AddPage(pmf, wxString::Format(wxT("[ PMF ] : Unnamed%d"), myNotebook->GetPageCount()), false, 2);
                    myNotebook->Refresh();
                }

                pmf->SetParameters(fieldSize, (check) ? blockSize : 0.0f, scale);
                double genTime = pmf->GeneratePMF(seed);
                if (! pmf->DrawGeneratedPMF()) {
                    ;
                }
                SetStatusText( wxString::Format(wxT("Generation time : %.3lf sec."), genTime), 0);
            }
            else {
                wxMessageBox(_("Field and block sizes should be positive!"), _("Wrong values!"));
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


void mainFrame::OnViewInfosMenuItemSelected(wxCommandEvent& event)
{
    bool value = myHtmlWindow->IsShown();
    myHtmlWindow->Show((value) ? false : true);
    wxMessageBox(((value) ? _("TRUE") : _("FALSE")), _("INFO"));
    myHtmlWindow->Refresh();
}


void mainFrame::OnLoadImageMenuItemSelected(wxCommandEvent& event)
{
    wxString caption = wxT("Choose a file");
    wxString wildcard = wxT("PNG files (*.png)|*.png");
    wildcard += wxT("|BMP files (*.bmp)|*.bmp");
    wildcard += wxT("|GIF files (*.gif)|*.gif");
    wxString defaultDir = wxT("C:\\Uzytki\\ImageJ\\images");
    wxString defaultFilename = wxEmptyString;

    wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();
        //int fiterIndex = dialog.GetFilterIndex();

        ImagePanel * ip = new ImagePanel(myNotebook, wxID_ANY);
        ip->LoadFile(path);
        myNotebook->AddPage(ip, wxT("[ IMAGE ] : ") + dialog.GetFilename(), false, 2);
    }
}


void mainFrame::OnMyNotebookPageChanged(wxNotebookEvent& event)
{
    wxWindow * w = myNotebook->GetCurrentPage();
    bool isClassImagePanel = w->IsKindOf(CLASSINFO(ImagePanel));
    bool isClassPMFPanel = w->IsKindOf(CLASSINFO(PMFPanel));

    if (isClassPMFPanel)
    {
        printf("Changed to ImagePanel ...\n");
    }
    GetMenuBar()->Enable(XRCID("ID_MENUITEM7"), isClassImagePanel || isClassPMFPanel);
    // PMFPanel options
    GetMenuBar()->Enable(XRCID("ID_GENERATE_MENUITEM"), ! isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_REGENERATE_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_ADDPOINT_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_SAVEPMF_MENUITEM"), isClassPMFPanel);
    isClassPMFPanel = false;
    GetMenuBar()->Enable(XRCID("ID_UPDPOINT_MENUITEM"), isClassPMFPanel);
    GetMenuBar()->Enable(XRCID("ID_DELPOINT_MENUITEM"), isClassPMFPanel);
}


void mainFrame::OnCloseImageMenuItemSelected(wxCommandEvent& event)
{
    int pageid = myNotebook->GetSelection();
    myNotebook->DeletePage(pageid);
}


void mainFrame::OnAddPointMenuItemSelected(wxCommandEvent& event)
{
    AddPointAction(event, 0.2, 2.0);
}


void mainFrame::OnRegenerateMenuItemSelected(wxCommandEvent& event)
{
    PMFPanel * pmf = (PMFPanel *) myNotebook->GetCurrentPage();
    GeneratingPMFAction(event, pmf);
    pmf->Refresh();
}


void mainFrame::AddPointAction(wxCommandEvent& event, double xx, double yy)
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

                PMFPanel * pp = (PMFPanel *) myNotebook->GetCurrentPage();
                pp->AddBirthPointToPMF(xx, yy, angle);

                SetStatusText( ss, 0);
            }
            else {
                wxMessageBox(_("Block size should be positive!"), _("Wrong values!"));
            }
    }
}

void mainFrame::OnSavePMFMenuItemSelected(wxCommandEvent& event)
{
    wxString caption = wxT("Saving PMF Configuration ...");
    wxString wildcard = wxT("Configuration files (*.cf)|*.cf");
    wildcard += wxT("|SVG graphics files (*.svg)|*.svg");
    wildcard += wxT("|Text files (*.txt)|*.txt");
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
        if (! pp->SavePMF(path, filterIndex))  wxMessageBox(_("Error during save"), _("Error"));
    }
}
