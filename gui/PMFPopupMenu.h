#ifndef PMFPOPUPMENU_H_INCLUDED
#define PMFPOPUPMENU_H_INCLUDED

#include <wx/menu.h>

class PMFPopupMenu : public wxMenu
{
	public:

		PMFPopupMenu(void *);
		virtual ~PMFPopupMenu();

		void SetPoint(double, double);


	private:

        void * mframe;
        double x;
        double y;

        void OnAddPointPopupMenuItemSelected(wxCommandEvent& event);



    DECLARE_EVENT_TABLE()
};



#endif // PMFPOPUPMENU_H_INCLUDED
