#ifndef PMFPANEL_H
#define PMFPANEL_H

#include "PMF.cpp"
#include <wx/dcmemory.h>
#include <wx/menu.h>

//(*Headers(PMFPanel)
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
//*)

class PMFPanel: public wxPanel
{
    DECLARE_CLASS(PMFPanel)

	public:

		PMFPanel(wxWindow* parent);
		virtual ~PMFPanel();

		void SetParameters(double, double, long);
		double GeneratePMF();
        bool DrawGeneratedPMF();

		//(*Declarations(PMFPanel)
		wxScrolledWindow* scrolledWindow;
		wxStaticBitmap* staticBitmap;
		//*)

	protected:

		//(*Identifiers(PMFPanel)
		//*)

	private:

        double fieldSize, blockSize;
        PMF<double> * pmf;

        wxBitmap * bmp;
        long scale;
        wxMenu popupMenu;

		//(*Handlers(PMFPanel)
		void OnRightUp(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
		//*)
		void OnAddPointPopupMenuItemSelected(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif
