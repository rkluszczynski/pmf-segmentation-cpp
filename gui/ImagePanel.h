#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

//(*Headers(ImagePanel)
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
//*)

class ImagePanel: public wxPanel
{
	public:

		ImagePanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ImagePanel();

		void LoadFile(wxString);

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
        wxImage image;
        wxBitmap * bmp;

		//(*Handlers(ImagePanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
