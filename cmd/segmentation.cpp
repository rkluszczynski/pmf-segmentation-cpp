#include "segmentation.h"
#include "probability.hpp"
#include "pmf.hpp"

namespace pmf
{

    BinarySegmentation::BinarySegmentation(double wsize, double hsize, const char * initialFile, const char * outputFile, time_t seed, const char * pictureFile, long iter, double pmr)
    : loopIteration(0), iterations(iter), rate(pmr), outputfile(outputFile)
    {
        img = new GrayscaleImage(pictureFile);

        pmf = new DoublePMF (wsize, hsize);
        pmf->SetSeed (seed);
		if (initialFile)
            pmf->LoadPMF (initialFile);
		else
            pmf->GenerateField ();
    }


    BinarySegmentation::~BinarySegmentation()
    {
        delete img;
    }


    bool
    BinarySegmentation::CheckRunningCondition()
    {
        if (iterations > 0  &&  loopIteration >= iterations) return false;
        if (rate > 0.0  &&  rate > storedArea) return false;
        return true;
    }

    bool
    BinarySegmentation::CheckApplyCondition()
    {
        //return SimulatedAnnealingSimulation<double>::CheckApplyCondition();
        return apply;
    }

    double
    BinarySegmentation::CalculateHamiltonian()
    {
        double beta_1 = 20. + 0.009 * loopIteration;
        double beta_2 = 0.0;
        double result = 0.0;

        double tmpArea, tmpElen;
        tmpArea = storedArea = pmf->CalculateEnergy(img);
        tmpElen = storedElen = 0.0;

        result = beta_1 * storedArea + beta_2 * storedElen;
        fprintf(stderr, "[ ENERGY ] : %lf  (%.2lf)\n", result, tmpArea);

        return result;
    }

    inline
    void
    BinarySegmentation::MakeModification()
    {
        using Probability::Uniform;

        double angle = Uniform<double>(0.0, 2. * M_PI);
        double sinL = sin(angle);
        double cosL = cos(angle);
        //pmf->RotatePointTypes(sinL, cosL);

        pmf::Statistics stats = pmf->GetStatistics();

        // * Determinig limits for random move. *
        double   noOfBirths = stats.GetNumberOfBirths();
        double    noOfTurns = stats.GetNumberOfUpdates();
        double denominatorZ = 1.0 / (areaOfPMF + noOfBirths + noOfTurns);

        double limit1 = areaOfPMF * denominatorZ;
        double limit2 = (areaOfPMF + noOfBirths) * denominatorZ;


        // * Applying random operation. *
        double chance = Uniform(0.0, 1.0);
        if (chance < limit1)
        {
            double x = Uniform(0.0, pmf->GetWidth());
            double y = Uniform(0.0, pmf->GetHeight());
            pmf->AddBirthPoint (x, y, angle);
        }
        else if(chance < limit2)
        {
            int number = rand() % int(noOfBirths);
            pmf->RemoveBirthPoint (number, angle);
        }
        else
        {
            int number = rand() % int(noOfTurns);
            pmf->UpdatePointVelocity (number, angle);
        }
    }

    void
    BinarySegmentation::ApplyModification()
    {
    }

    void
    BinarySegmentation::CancelModification()
    {
        swap(pmf, clone);
    }

    void
    BinarySegmentation::PreIteration()
    {
        //clone = pmf->Clone();
        apply = true;
    }

    void
    BinarySegmentation::PostIteration()
    {
        //delete clone;
        ++loopIteration;
    }


    void
    BinarySegmentation::Prepare()
    {
        areaOfPMF = M_PI * pmf->GetHeight() * pmf->GetWidth();
    }

    void
    BinarySegmentation::Finish()
    {
        if (outputfile) pmf->SavePMF (outputfile);
        delete pmf;
    }
}
