#ifndef PMFPANEL_H
#define PMFPANEL_H

//(*Headers(PMFPanel)
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
//*)

class PMFPanel: public wxPanel
{
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
        long scale;
        wxBitmap * bmp;

		//(*Handlers(PMFPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
