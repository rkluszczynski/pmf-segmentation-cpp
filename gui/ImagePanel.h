#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

//(*Headers(ImagePanel)
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
//*)

#include "PMF.hpp"


class ImagePanel : public wxPanel
{
    DECLARE_CLASS(ImagePanel)

	public:

		ImagePanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ImagePanel();

		void LoadFile(wxString);
		bool SaveFile(wxString, int);

        void Reset();
        void Rescale(int, int);

        void ConvertToGreyscale(double lr = 0.299, double lg = 0.587, double lb = 0.114);
		void CalculateGradient();
		void PresentPMF(PMF<double> *);

		//(*Declarations(ImagePanel)
		wxScrolledWindow* scrolledImageWindow;
		wxStaticBitmap* staticBitmap;
		//*)

	protected:

		//(*Identifiers(ImagePanel)
		static const long ID_STATICBITMAP1;
		static const long ID_SCROLLEDWINDOW1;
		//*)

	private:

        void * mframe;
        wxImage image;
        wxBitmap * bmp;

		//(*Handlers(ImagePanel)
		void OnLeftUp(wxMouseEvent& event);
		//*)

	DECLARE_EVENT_TABLE()
};

#endif
