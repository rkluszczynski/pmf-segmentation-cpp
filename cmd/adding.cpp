#include "../cmd/pmf.hpp"


namespace pmf
{

    void GetTheFieldAndDoTheModification (
                                            double fieldWidth,
                                            double fieldHeight,
                                            time_t seed,
                                            const char * initialFile,
                                            const char * outputFile,
                                            double x,
                                            double y,
                                            double angle,
                                            struct timeb * tbeg = NULL,
                                            struct timeb * tend = NULL
                                        )
    {
		/* Getting Polygonal Markov Field realization. */
        using pmf::DoublePMF;

        DoublePMF * pmf = new DoublePMF (fieldWidth, fieldHeight);
        pmf->SetSeed (seed);
		if (initialFile)
            pmf->LoadPMF (initialFile);
		else
            pmf->GenerateField ();

        /* Adding a birth site to PMF. */
        if (tbeg) ftime(tbeg);

        pmf->RotatePoints2 (angle);
#if defined(__PMF_ADD_SEGMENT)
        pmf->AddBirthSegment (x, y, angle);
#elif defined(__PMF_ADD_POINT)
        pmf->AddBirthPoint (x, y, angle);
#else
    #error "Define preprocesor directive you want (__PMF_ADD_POINT or __PMF_ADD_SEGMENT)."
#endif

	    if (tend) ftime(tend);

        if (outputFile) pmf->SavePMF (outputFile);
        delete pmf;
    }

}
