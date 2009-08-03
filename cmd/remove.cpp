#include "../cmd/pmf.hpp"


namespace pmf
{

    void GetTheFieldAndDoTheModification (
                                            double fieldWidth,
                                            double fieldHeight,
                                            time_t seed,
                                            const char * initialFile,
                                            const char * outputFile,
                                            long pointId,
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

        if (tbeg) ftime(tbeg);

        pmf->RotatePoints2 (angle);
#if defined(__PMF_UPDATE_POINT)
        /* Updating an update site in PMF. */
        pmf->UpdatePointVelocity (pointId, angle);
#elif defined(__PMF_REMOVE_POINT)
        /* Removing a birth site from PMF. */
        pmf->RemoveBirthPoint (pointId, angle);
#else
    #error "Define preprocesor directive you want (__PMF_UPDATE_POINT or __PMF_REMOVE_POINT)."
#endif

	    if (tend) ftime(tend);

        if (outputFile) pmf->SavePMF (outputFile);
        delete pmf;
    }

}
