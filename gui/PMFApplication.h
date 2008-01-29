/***************************************************************
 * Name:      PMF_GUIApp.h
 * Purpose:   Defines Application Class
 * Author:    Rafal ()
 * Created:   2008-01-28
 * Copyright: Rafal ()
 * License:
 **************************************************************/

#ifndef PMF_GUIAPP_H
#define PMF_GUIAPP_H

#include <wx/app.h>

class PMFApplication : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // PMF_GUIAPP_H
