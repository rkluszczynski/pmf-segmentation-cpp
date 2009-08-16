#include "../cmd/pmf.hpp"


int main(int argc, char **argv)
{
    int start = 0;
    int n = 10;
    if (argc > 1)
    {
        n = atoi(argv[1]);
    }
    if (argc > 2)  {  start = atoi(argv[2]);  }

    double size = 1.;
    FOR(seed, start, n)
    {
        using namespace pmf;

        DoublePMF * pmf = new DoublePMF (size, size);
        pmf->SetSeed (seed);
        pmf->GenerateField ();

        Statistics stats = pmf->GetStatistics ();
        REP(i, stats.GetNumberOfDeaths())
        {
            DoublePMF * clone = pmf->Clone();

            double angle = Probability::Uniform(0., 2.*M_PI);

            clone->RemoveBirthPoint(i, angle);

            delete clone;
        }
        delete pmf;
    }

    cout << "   PMF_EVENT_COUNTER  = " << pmf::pmf_event_counter << endl;
    cout << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
    cout << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;

    return 0;
}
