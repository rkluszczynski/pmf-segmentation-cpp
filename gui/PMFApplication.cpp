/***************************************************************
 * Name:      PMF_GUIApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Rafal ()
 * Created:   2008-01-28
 * Copyright: Rafal ()
 * License:
 **************************************************************/

#include "PMFApplication.h"

//(*AppHeaders
#include "mainFrame.h"
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
//*)

IMPLEMENT_APP(PMFApplication);

bool PMFApplication::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    wxXmlResource::Get()->InitAllHandlers();
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("mainFrame.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("GenerateDialog.xrc"));
    if ( wxsOK )
    {
    	mainFrame* Frame = new mainFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;
}
