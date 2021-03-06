#ifndef PMFPOPUPMENU_H_INCLUDED
#define PMFPOPUPMENU_H_INCLUDED

#include <wx/menu.h>

class PMFPanel;

class PMFPopupMenu : public wxMenu
{
	public:

		PMFPopupMenu(void *, const PMFPanel * pp);
		virtual ~PMFPopupMenu();

		void SetPoint(double, double);
		void Reinitialize();


	private:

        PMFPanel * pmfPanel;
        void * mframe;
        double x;
        double y;

        void OnAddPointPopupMenuItemSelected(wxCommandEvent& event);
        void OnUpdatePointPopupMenuItemSelected(wxCommandEvent& event);
        void OnRemovePointPopupMenuItemSelected(wxCommandEvent& event);
        void OnClearPopupMenuItemSelected(wxCommandEvent& event);
        void OnAddSegmentPopupMenuItemSelected(wxCommandEvent& event);


    DECLARE_EVENT_TABLE()
};



#endif // PMFPOPUPMENU_H_INCLUDED
