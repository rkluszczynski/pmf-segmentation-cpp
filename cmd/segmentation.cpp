#include "segmentation.h"
#include "pmf.hpp"

namespace pmf
{

    BinarySegmentation::BinarySegmentation(double wsize, double hsize, const char * initialFile, const char * outputFile, time_t seed, const char * pictureFile, long iter, double pmr)
    : loopIteration(1), iterations(iter), rate(pmr), outputfile(outputFile)
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
        /// TODO (klusi#1#): rate breaking simulation
        if (rate > 0.0 ) return false;
        return true;
    }

    bool
    BinarySegmentation::CheckApplyCondition()
    {
        return SimulatedAnnealingSimulation<double>::CheckApplyCondition();
    }

    double
    BinarySegmentation::CalculateHamiltonian()
    {
        double beta_1 = 20. + 0.009 * loopIteration;
        double beta_2 = 0.0;

        double area = pmf->CalculateEnergy(img);

        return 0.0;
    }

    inline
    void
    BinarySegmentation::MakeModification()
    {
/*
inline
bool
MODIFY_CONFIGURATION (PMF<double> * pmf, double areaOfPMF, double angle, double bSize)
{
    double sinL = sin(angle);
    double cosL = cos(angle);
    int pmfStatistic[PT_MAX_NUMBER];

    // * Determinig limits for random move. *
    pmf->RotatePointTypes(sinL, cosL);
    pmf->GetPMFConfiguration()->calculate_statistics(pmfStatistic);

    double   noOfBirths = double(pmfStatistic[PT_BIRTH_DOWN] + pmfStatistic[PT_BIRTH_LEFT]
                         + pmfStatistic[PT_BIRTH_NORMAL] + pmfStatistic[PT_BIRTH_UP]);
    double    noOfTurns = 0.5 * double(pmfStatistic[PT_UPDATE]);
    double denominatorZ = 1.0 / (areaOfPMF + noOfBirths + noOfTurns);

    double limit1 = areaOfPMF * denominatorZ;
    double limit2 = (areaOfPMF + noOfBirths) * denominatorZ;

    // * Applying random operation. *
    double chance = Uniform(0.0, 1.0);
    if (chance < limit1)  //ADD
    {
        double x, y;
        do
        {
            x = Uniform(0.0, pmf->GetPMFWidth());
            y = Uniform(0.0, pmf->GetPMFHeight());
        }
        while(! (0.0 < x  &&  x < pmf->GetPMFWidth()  &&  0.0 < y  &&  y < pmf->GetPMFHeight()));
        pmf->AddBirthPoint(x, y, angle, bSize);
    }
    else if(chance < limit2) //REM
    {
        int number = rand() % int(noOfBirths);
        int ID = 0;

        Element<pmf_point<double> > * iter = pmf->GetPMFConfiguration()->getHead();
        long typePointCounter = 0;
        while (iter)
        {
            pmf_point<double> * pt = iter->data;
            if (pt->type == PT_BIRTH_NORMAL  ||  pt->type == PT_BIRTH_LEFT
                ||  pt->type == PT_BIRTH_UP  ||  pt->type == PT_BIRTH_DOWN)
            {
                if (typePointCounter == number) {
                    ID = pt->id;
                    break;
                }
                ++typePointCounter;
            }
            iter = iter->next;
        }
        pmf->RemoveBirthPoint(ID, angle, bSize);
    }
    else //UPD
    {
        int number = rand() % int(noOfTurns);
        int ID = 0;

        Element<pmf_point<double> > * iter = pmf->GetPMFConfiguration()->getHead();
        long typePointCounter = 0;
        while (iter)
        {
            pmf_point<double> * pt = iter->data;
            if (pt->type == PT_UPDATE)
            {
                if (typePointCounter == number) {
                    ID = pt->id;
                    break;
                }
                ++typePointCounter;
            }
            iter = iter->next;
        }
        pmf->UpdatePointVelocity(ID, angle, bSize);
    }
    //return false;
    return (bSize > 0.0) ? pmf->IsThereIntersection() : false;
}
//*/
    }

    void
    BinarySegmentation::ApplyModification()
    {
    }

    void
    BinarySegmentation::CancelModification()
    {
    }

    void
    BinarySegmentation::PostIteration()
    {
        //delete clone;
        ++loopIteration;
    }


    void
    BinarySegmentation::Finish()
    {
        if (outputfile) pmf->SavePMF (outputfile);
        delete pmf;
    }
}
