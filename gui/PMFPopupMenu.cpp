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


#define ITEM1_TEXT "Add point ..."
#define ITEM2_TEXT "Update point ..."
#define ITEM3_TEXT "Remove point ..."
#define ITEM4_TEXT "Clear"
#define ITEM5_TEXT "Add segment ..."


PMFPopupMenu::PMFPopupMenu(void *frame, const PMFPanel * pp)
{
    AppendSeparator();
    Append(wxID_ANY, wxT(ITEM1_TEXT));
    Append(wxID_ANY, wxT(ITEM2_TEXT));
    Append(wxID_ANY, wxT(ITEM3_TEXT));
    Append(wxID_ANY, wxT(ITEM5_TEXT));
    AppendSeparator();
    Append(wxID_ANY, wxT(ITEM4_TEXT));
    AppendSeparator();

    Connect(FindItem(wxT(ITEM1_TEXT)),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PMFPopupMenu::OnAddPointPopupMenuItemSelected);
    Connect(FindItem(wxT(ITEM2_TEXT)),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PMFPopupMenu::OnUpdatePointPopupMenuItemSelected);
    Connect(FindItem(wxT(ITEM3_TEXT)),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PMFPopupMenu::OnRemovePointPopupMenuItemSelected);
    Connect(FindItem(wxT(ITEM4_TEXT)),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PMFPopupMenu::OnClearPopupMenuItemSelected);
    Connect(FindItem(wxT(ITEM5_TEXT)),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PMFPopupMenu::OnAddSegmentPopupMenuItemSelected);

    mframe = frame;
    pmfPanel = (PMFPanel *)pp;

    Reinitialize();
}


PMFPopupMenu::~PMFPopupMenu()
{
	//(*Destroy(AddPointDialog)
	//*)
}


void PMFPopupMenu::Reinitialize()
{
    pmf_point<double> * pmfPoint = pmfPanel->GetSelectedPMFPoint();
    wxPoint clickedPoint = pmfPanel->GetNewPMFPointLocation();

    if (clickedPoint.x >= 0  &&  clickedPoint.y >= 0)
    {
        double xx = double(clickedPoint.x+1) / double(pmfPanel->GetScale());
        double yy = double(clickedPoint.y+1) / double(pmfPanel->GetScale());

        SetPoint(xx, yy);
        Enable(FindItem(wxT(ITEM1_TEXT)), true);
        Enable(FindItem(wxT(ITEM2_TEXT)), false);
        Enable(FindItem(wxT(ITEM3_TEXT)), false);
        Enable(FindItem(wxT(ITEM4_TEXT)), true);
        Enable(FindItem(wxT(ITEM5_TEXT)), true);
    }
    else if (pmfPoint != NULL)
    {
        Enable(FindItem(wxT(ITEM1_TEXT)), false);
        Enable(FindItem(wxT(ITEM2_TEXT)), true);
        Enable(FindItem(wxT(ITEM3_TEXT)), true);
        Enable(FindItem(wxT(ITEM4_TEXT)), true);
        Enable(FindItem(wxT(ITEM5_TEXT)), false);
    }
    else {
        bool value = false;
        Enable(FindItem(wxT(ITEM1_TEXT)), value);
        Enable(FindItem(wxT(ITEM2_TEXT)), value);
        Enable(FindItem(wxT(ITEM3_TEXT)), value);
        Enable(FindItem(wxT(ITEM4_TEXT)), false);
        Enable(FindItem(wxT(ITEM5_TEXT)), value);
    }
}


void PMFPopupMenu::OnAddPointPopupMenuItemSelected(wxCommandEvent& event)
{
    pmfPanel->AddBirthPointToPMF(x, y);
}


void PMFPopupMenu::OnUpdatePointPopupMenuItemSelected(wxCommandEvent& event)
{
    pmfPanel->UpdatePointInsidePMF();
}


void PMFPopupMenu::OnRemovePointPopupMenuItemSelected(wxCommandEvent& event)
{
    pmfPanel->RemovePointFromPMF();
}


void PMFPopupMenu::OnClearPopupMenuItemSelected(wxCommandEvent& event)
{
    pmfPanel->ClearConfigurationSelection();
    pmfPanel->DrawGeneratedPMF(true);
}


void PMFPopupMenu::OnAddSegmentPopupMenuItemSelected(wxCommandEvent& event)
{
    pmfPanel->AddBirthSegmentToPMF(x, y);
}


void PMFPopupMenu::SetPoint(double xx, double yy)
{
    x = xx;
    y = yy;
}

