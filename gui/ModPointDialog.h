#ifndef MODPOINTDIALOG_H
#define MODPOINTDIALOG_H

#include "point.hpp"
//(*Headers(ModPointDialog)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class ModPointDialog: public wxDialog
{
	public:

		ModPointDialog(wxWindow* parent);
		virtual ~ModPointDialog();

        bool isOk();
        void SetPMFPointData(pmf_point<double> *);

		//(*Declarations(ModPointDialog)
		wxTextCtrl* degreeAngleTextCtrl;
		wxPanel* leftPanel;
		wxStaticText* StaticText2;
		wxTextCtrl* blockSizeTextCtrl;
		wxStaticText* cosStaticText;
		wxStaticText* StaticText6;
		wxPanel* orientationPanel;
		wxTextCtrl* radianAngleTextCtrl;
		wxPanel* topPanel;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxStaticText* pointInfoStaticText;
		wxPanel* angleCirclePanel;
		wxStaticText* StaticText5;
		wxTextCtrl* pointIdTextCtrl;
		wxStaticText* sinStaticText;
		wxButton* okButton;
		wxCheckBox* useBlocksCheckBox;
		wxStaticText* StaticText4;
		wxPanel* bottomPanel;
		wxButton* cancelButton;
		//*)

	protected:

		//(*Identifiers(ModPointDialog)
		//*)

	private:

        pmf_point<double> * selectedPoint;

        bool clickedOK;
        int x0, y0, x, y;
        double angle, sinus, cosinus;
        int circleRadius;

		//(*Handlers(ModPointDialog)
		void OnAngleCirclePanelPaint(wxPaintEvent& event);
		void OnOkButtonClick(wxCommandEvent& event);
		void OnCancelButtonClick(wxCommandEvent& event);
		void OnAngleCirclePanelLeftUp(wxMouseEvent& event);
		void OnDegreeAngleTextCtrlText(wxCommandEvent& event);
		void OnRadianAngleTextCtrlText(wxCommandEvent& event);
		void OnUseBlocksCheckBoxClick(wxCommandEvent& event);
		//*)

    DECLARE_EVENT_TABLE()
};

#endif
