#include <iostream>

#include <wx/wx.h>
#include <wx/log.h>

using namespace std;

#include "../cmd/point.hpp"
#include "../cmd/configuration.hpp"

int main()
{
    wxLogVerbose(_("tego nie widac"));

    wxLog::SetVerbose(true);

    /*
    wxLog *logger = new wxLogStream(&cout);
    wxLog::SetActiveTarget(logger);
    //*/
    cout << "Hello, PMF!" << endl;

    wxLogMessage(_("DONE"));
    wxLogVerbose(_("with exit code = 0"));

    return 0;
}
