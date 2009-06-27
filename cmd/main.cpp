#include <iostream>

#include <wx/wx.h>
#include <wx/log.h>

using namespace std;

#include "../cmd/point.hpp"
#include "../cmd/birthsites.hpp"
#include "../cmd/geometry.hpp"
#include "../cmd/probability.hpp"
#include "../cmd/segmentset.hpp"
#include "../cmd/events.hpp"
#include "../cmd/schedule.hpp"
#include "../cmd/pmf.hpp"

int main()
{
    using namespace pmf;
    //Configuration<double> cf(0.0,0.0);
    PMF<double> ppmf(3.,3.);
    MutableSegmentSet mss;

    ppmf.GenerateField();

    BirthSites<double> bs;
    Point<double> * pt1 = new Point<double> (0.0, 0.0, 0.0, 0.0, 1);
    Point<double> * pt2 = new Point<double> (1.0, 1.0, 0.0, 0.0, 2);
    Point<double> * pt3 = new Point<double> (2.0, 2.0, 0.0, 0.0, 3);

    bs.Insert(pt1);
    bs.Insert(pt2);
    bs.Insert(pt3);

    delete bs.Erase(2);

    bs.PrintSites(cout);
    cout << pmf_point_counter << endl;

    delete bs.Erase(bs.GetFirst());
    bs.PrintSites(cout);
    cout << pmf_point_counter << endl;

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
