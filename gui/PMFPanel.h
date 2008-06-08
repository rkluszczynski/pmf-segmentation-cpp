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
        bool DrawGeneratedPMF(bool configurationLowerLayer);
        void AddBirthPointToPMF(double, double);
        void UpdatePointInsidePMF();
        void RemovePointFromPMF();
        void AddBirthSegmentToPMF(double, double);

        bool SavePMF(wxString, int);
        bool LoadPMF(wxString);
        //void SetScale(long);

		void ClearConfigurationSelection();

        pmf_point<double> * GetSelectedPMFPoint();
        wxPoint & GetNewPMFPointLocation();
        long GetScale();
        std::pair<double, double> GetPMFSize();
        long GetConfigurationSize();

        void SetSelectedPMFPoint(long);

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
        pmf_point<double> * choosenPoint;
        wxPoint setNewPointLocation;

        wxBitmap * bmp;
        long scale;

        PMFPopupMenu * pmfPopupMenu;
        void * mframe;
        bool doubleClicked;

		//(*Handlers(PMFPanel)
		void OnRightUp(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
		void OnLeftDClick(wxMouseEvent& event);
		//*)
		void IterateAndDrawConfiguration(wxMemoryDC& dc);
		void ExecutePointModificationDialog(long, wxString &);
		void ExecutePointBirthsDialog(double, double, long, wxString &);

	DECLARE_EVENT_TABLE()
};

#endif
