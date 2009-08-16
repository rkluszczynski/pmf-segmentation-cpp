#include "../cmd/pmf.hpp"


int main(int argc, char **argv)
{
    int start = 0;
    int n = 10;
    string file, path = "output/";

    if (argc > 1)
    {
        n = atoi(argv[1]);
        path = "../output/";
    }
    if (argc > 2)  {  start = atoi(argv[2]);  }

    double size = 1.;
    FOR(seed, start, n)
    {
        fprintf(stderr, "\n----------------------------\n[ SEED ] = %i\n", seed);  fflush(stderr);
        using namespace pmf;

        ofstream fout0(string(path + "test-log-gen.txt").c_str());
        out.rdbuf(fout0.rdbuf());

        DoublePMF * pmf = new DoublePMF (size, size);
        pmf->SetSeed (seed);
        pmf->GenerateField ();

        out.rdbuf(std::cout.rdbuf());
        fout0.close();

        pmf->SavePMF (string(path + "test-cf-gen.txt").c_str());
        pmf->SavePMF (string(path + "test-cf-gen.zip").c_str(), GeoGebraFile);

        double angle = Probability::Uniform(0., 2.*M_PI);

        ofstream tmp0(string(path + "tmp.txt").c_str());
        out.rdbuf(tmp0.rdbuf());

        DoublePMF * clone = pmf->Clone();
        clone->RotatePoints2 (angle);
        Statistics stats = clone->GetStatistics ();
        delete clone;
        fprintf(stderr, "[ STAT ] : { births = %u }\n\n", stats.GetNumberOfBirths());  fflush(stderr);

        REP(i, int(stats.GetNumberOfBirths()))
        {
            DoublePMF * clone = pmf->Clone();
            fprintf(stderr, "[  TRY ] : removing birth %i\n", i);  fflush(stderr);

            ofstream fout1(string(path + "test-log-rem.txt").c_str());
            out.rdbuf(fout1.rdbuf());

            clone->RemoveBirthPoint(i, angle);
            clone->SavePMF (string(path + "test-cf-rem.txt").c_str());
            clone->SavePMF (string(path + "test-cf-rem.zip").c_str(), GeoGebraFile);

            out.rdbuf(std::cout.rdbuf());
            fout1.close();

            delete clone;
        }
        delete pmf;
    }

    cout << "   PMF_EVENT_COUNTER  = " << pmf::pmf_event_counter << endl;
    cout << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
    cout << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;

    return 0;
}
