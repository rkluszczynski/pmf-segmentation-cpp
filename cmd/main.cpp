#include <iostream>

#include <wx/wx.h>
#include <wx/log.h>

using namespace std;

#include "../cmd/birthsites.hpp"
#include "../cmd/pmf.hpp"


int main(int argc, char **argv)
{
    //*
    using namespace pmf;
    cout.precision(12);
    //Configuration<double> cf(0.0,0.0);
    double size = 1;//21;
    PMF<double> * ppmf = new PMF<double>(size, size);
    //ppmf->SetSeed(1168); //przy size=2
    //ppmf->SetSeed(738703); //size=1
    //ppmf->SetSeed(atoi(argv[1]));
    ppmf->SetSeed(1);

    wxLog::SetVerbose(true);

    ppmf->GenerateField();
    cout << "   PMF_POINT_COUNTER  = " << pmf::pmf_point_counter << endl;
    assert(pmf::pmf_point_counter == ppmf->GetCount());

    ppmf->SavePMF("output/cmd-gen.txt");

    cout << "   PMF_POINT_COUNTER  = " << pmf::pmf_point_counter << endl;
    assert(pmf::pmf_point_counter == ppmf->GetCount());

    ofstream fout("output/qq.txt");
    out.rdbuf(fout.rdbuf());
    ppmf->AddBirthPoint(0.5, 0.5, M_PI);
    fout.close();
    ppmf->SavePMF("output/cmd-add.txt");

    cout << "   PMF_EVENT_COUNTER  = " << pmf::pmf_event_counter << endl;
    cout << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
    cout << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;

    delete ppmf;
    cout << "   PMF_POINT_COUNTER  = " << pmf::pmf_point_counter << endl;

    return 0;
    //*/
    /*
    ofstream fin("output/log.txt");

    wxLogStream* logger = new wxLogStream(&fin);
    //logger = new wxLogStream();
    wxLog::SetActiveTarget(logger);
    //*/
    PMFLog("check %i", 1);
    wxLog::Suspend();
    wxLogNull * q = new wxLogNull();
    PMFLog("check %i", 2);

    cout << "Test do couta " << endl;
    wxLog::Resume();
    delete q;
    PMFLog("check %i", 17);

    //fin.close();
    return 0;

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
