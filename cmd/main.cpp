#include <iostream>

#include <wx/wx.h>
#include <wx/log.h>

using namespace std;

#include "../cmd/point.hpp"
#include "../cmd/configuration.hpp"

int main()
{
    pmf::Configuration<double> cf(0.0,0.0);

    wxLogVerbose(_("tego nie widac"));

    wxLog::SetVerbose(true);
    wxLogDebug(_("debug mode 1"));

#define __WXDEBUG__
    //wxLogTrace(_("debug mode 2"));
    wxLogDebug(_("debug mode 1"));

    /*
    wxLog *logger = new wxLogStream(&cout);
    wxLog::SetActiveTarget(logger);
    //*/
    cout << "Hello, PMF!" << endl;

    wxLogMessage(_("DONE"));

    wxLog::SetTimestamp(_("[%d/%b/%y %H:%M:%S] "));

    FILE * m_pLogFile = NULL;
    if (m_pLogFile == NULL)
    {
        m_pLogFile = fopen( "output/logfile.txt", "w+" );
        delete wxLog::SetActiveTarget(new wxLogStderr(m_pLogFile));
    }
    wxLogMessage(_("Test"));
    delete wxLog::SetActiveTarget(NULL);
    if (m_pLogFile != NULL)
    {
        fclose(m_pLogFile);
    }

    wxLogVerbose(_("with exit code = 0"));

    return 0;
}
