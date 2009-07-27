#include <iostream>

#include <wx/wx.h>
#include <wx/log.h>

using namespace std;

#include "../cmd/birthsites.hpp"
#include "../cmd/pmf.hpp"
//#include "../cmd/simulation.hpp"


int main(int argc, char **argv)
{
    char dir[128] = "output/";
    int n = 1, start = 0;
    bool useFor = false;
    if (argc > 1)
    {
        n = atoi(argv[1]);
        strcpy(dir, "../output/");
    }
    if (argc > 2)  {  start = atoi(argv[2]);  }
    int len = strlen(dir);
    //*
    using namespace pmf;
    cout.precision(12);
    //Configuration<double> cf(0.0,0.0);
    double size = 1;//21;
    time_t seed = 2858699;
    //seed = 2858779;
    //seed = 2858855;
    //seed = 114835;
    seed = 37047;
    if (useFor)
    {
        start = 34648;
        n = 34650;
    }
    for (int i = start; i < n; ++i)
    {
        if (argc > 1 || useFor) seed = i;
        PMF<double> * ppmf = new PMF<double>(size, size);
        //ppmf->SetSeed(1168); //przy size=2
        //ppmf->SetSeed(738703); //size=1
        ppmf->SetSeed(seed);
        //ppmf->SetSeed(2010);

        wxLog::SetVerbose(true);

        strcpy(dir + len, "q0.txt");
        ofstream fout1(dir);
        out.rdbuf(fout1.rdbuf());
        ppmf->GenerateField();
        fout1.close();
        cout << "   PMF_POINT_COUNTER  = " << pmf::pmf_point_counter << endl;
        assert(pmf::pmf_point_counter == ppmf->GetCount());

        strcpy(dir + len, "cmd-gen.txt");
        ppmf->SavePMF(dir);
        strcpy(dir + len, "geo-gen.txt");
        ppmf->SavePMF(dir, GeoGebraCmds);
        strcpy(dir + len, "geo-gen.zip");
        ppmf->SavePMF(dir, GeoGebraFile);

        cout << "   PMF_POINT_COUNTER  = " << pmf::pmf_point_counter << endl;
        assert(pmf::pmf_point_counter == ppmf->GetCount());

        out.rdbuf(std::cout.rdbuf());
        strcpy(dir + len, "qq.txt");
        ofstream fout(dir);
        fout << "[ SEED ] : " << seed << endl;
        out.rdbuf(fout.rdbuf());
        double angle = pmf::Probability::Uniform(0.0, 2. * M_PI);
        //angle = M_PI;
        angle = M_PI_2;
        ppmf->AddBirthPoint(0.5, 0.5, angle);
        fout.close();
        strcpy(dir + len, "cmd-add.txt");
        ppmf->SavePMF(dir);
        strcpy(dir + len, "geo-add.zip");
        ppmf->SavePMF(dir, GeoGebraFile);

        cout << "   PMF_EVENT_COUNTER  = " << pmf::pmf_event_counter << endl;
        cout << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
        cout << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;

        delete ppmf;
        cout << "   PMF_POINT_COUNTER  = " << pmf::pmf_point_counter << endl;
    }
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
