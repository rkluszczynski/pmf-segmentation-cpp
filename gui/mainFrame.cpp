#include "mainFrame.h"
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include "GenerateDialog.h"

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
	myScrolledWindow = (wxScrolledWindow*)FindWindow(XRCID("ID_SCROLLEDWINDOW1"));
	myHtmlWindow = (wxHtmlWindow*)FindWindow(XRCID("ID_HTMLWINDOW1"));
	mySplitterWindow = (wxSplitterWindow*)FindWindow(XRCID("ID_SPLITTERWINDOW1"));
	myNotebook = (wxNotebook*)FindWindow(XRCID("ID_NOTEBOOK1"));
	MenuItem1 = (wxMenuItem*)FindWindow(XRCID("ID_MENUITEM4"));
	quitMenuItem = (wxMenuItem*)FindWindow(XRCID("ID_MENUITEM1"));
	generateMenuItem = (wxMenuItem*)FindWindow(XRCID("ID_MENUITEM2"));
	aboutMenuItem = (wxMenuItem*)FindWindow(XRCID("ID_MENUITEM3"));
	StatusBar1 = (wxStatusBar*)FindWindow(XRCID("ID_STATUSBAR1"));

	Connect(XRCID("ID_MENUITEM4"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnMenuItem1Selected);
	Connect(XRCID("ID_MENUITEM1"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnQuit);
	Connect(XRCID("ID_MENUITEM2"),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&mainFrame::OnGenerateMenuItemSelected);
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
    Destroy();
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
    myScrolledWindow->SetVirtualSize(500, 500);
    myScrolledWindow->SetScrollRate(10, 10);
}

void mainFrame::OnGenerateMenuItemSelected(wxCommandEvent& event)
{
    GenerateDialog gDialog(this);
    gDialog.ShowModal();
    if ( gDialog.isOk() ) {
        wxString str = (gDialog.TextCtrl1)->GetValue();
        wxMessageBox(str, _("Welcome to..."));
    }
}
