#ifndef PMFPANEL_H
#define PMFPANEL_H

#include "PMF.cpp"
#include <wx/dcmemory.h>

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

        bool DrawGeneratedPMF(wxMemoryDC &);

		//(*Handlers(PMFPanel)
		void OnRightUp(wxMouseEvent& event);
		//*)

	DECLARE_EVENT_TABLE()
};

#endif
