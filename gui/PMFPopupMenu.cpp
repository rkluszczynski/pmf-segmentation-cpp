#include "PMFPopupMenu.h"
#include "mainFrame.h"
#include <cmath>
#include <wx/wx.h>
#include <wx/menu.h>

//(*InternalHeaders(AddPointDialog)
//*)

//(*IdInit(AddPointDialog)
//*)

BEGIN_EVENT_TABLE(PMFPopupMenu,wxMenu)
	//(*EventTable(AddPointDialog)
	//*)
END_EVENT_TABLE()


PMFPopupMenu::PMFPopupMenu(void *frame)
{
    AppendSeparator();
    Append(wxID_ANY, wxT("Add point ..."));
    Append(wxID_ANY, wxT("Update point ..."));
    Append(wxID_ANY, wxT("Delete point ..."));
    AppendSeparator();

    Connect(FindItem(wxT("Add point ...")),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PMFPopupMenu::OnAddPointPopupMenuItemSelected);
    Enable(FindItem(wxT("Update point ...")), false);
    Enable(FindItem(wxT("Delete point ...")), false);

    mframe = frame;
}


PMFPopupMenu::~PMFPopupMenu()
{
	//(*Destroy(AddPointDialog)
	//*)
}


void PMFPopupMenu::OnAddPointPopupMenuItemSelected(wxCommandEvent& event)
{
    //wxMessageBox(wxString::Format(wxT("%lf --- %lf"), x, y), wxT("vfegre"));
    ((mainFrame *) mframe)->AddPointAction(event, x, y);
}


void PMFPopupMenu::SetPoint(double xx, double yy)
{
    x = xx;
    y = yy;
}

