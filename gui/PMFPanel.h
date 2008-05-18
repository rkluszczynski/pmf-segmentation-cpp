#ifndef PMFPANEL_H
#define PMFPANEL_H

#define CHECK_ASSERTIONS 1

#include "PMF.hpp"
#include "PMFPopupMenu.h"
#include <wx/dcmemory.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>

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
		double GeneratePMF(time_t);
        bool DrawGeneratedPMF();
        void AddBirthPointToPMF(double, double, double);

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

        PMFPopupMenu * pmfPopupMenu;
        void * mframe;

		//(*Handlers(PMFPanel)
		void OnRightUp(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
		//*)

	DECLARE_EVENT_TABLE()
};

#endif
