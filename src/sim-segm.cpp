
#include "grayscaleimage.hpp"


inline void CALCULATE_LIMITS (PMF<double> * pmf, double areaOfPMF, double angle, double *limit1, double *limit2)
{
    double sinL = sin(angle);
    double cosL = cos(angle);
    int pmfStatistic[PT_MAX_NUMBER];

    pmf->RotatePointTypes(sinL, cosL);
    pmf->GetPMFConfiguration()->calculate_statistics(pmfStatistic);

    double   noOfBirths = double(pmfStatistic[PT_BIRTH_DOWN] + pmfStatistic[PT_BIRTH_LEFT]
                         + pmfStatistic[PT_BIRTH_NORMAL] + pmfStatistic[PT_BIRTH_UP]);
    double    noOfTurns = 0.5 * double(pmfStatistic[PT_UPDATE]);
    double denominatorZ = 1.0 / (areaOfPMF + noOfBirths + noOfTurns);

    *limit1 = areaOfPMF * denominatorZ;
    *limit2 = (areaOfPMF + noOfBirths) * denominatorZ;

    return;
}


inline bool MODIFY_CONFIGURATION (PMF<double> * pmf, double angle, double chance, double limit1, double limit2, double bSize)
{
    int ID = 0;

    if (chance < limit1) {  //ADD
        double x = Uniform(0.0, pmf->GetPMFWidth());
        double y = Uniform(0.0, pmf->GetPMFHeight());
        pmf->AddBirthPoint(x, y, angle, bSize);
    }
    else if(chance < limit2) { //REM
        pmf->RemoveBirthPoint(ID, angle, bSize);
    }
    else { //UPD
        pmf->UpdatePointVelocity(ID, angle, bSize);
    }
    return false;
}


void SimulateBinarySegmentation (
                            char * imageFileName,
                            PMF<double> * pmf,
                            long iterations,
                            double PMFRate,
                            double bSize
                        )
{
    //* Initializing some variables. *
    GrayScaleImage gsimg(imageFileName);


    double    beta_1 = 20.0;
    double    beta_2 = 0.0;
    double areaOfPMF = M_PI * pmf->GetPMFHeight() * pmf->GetPMFWidth();

    double tmpArea, tmpELen, storedArea, storedELen;
    tmpArea = storedArea = gsimg.CalculateScanLinesEnergy(pmf);
    tmpELen = storedELen = 0.0; //estimateEdgesLen(Hist);

    double engH = beta_1 * storedArea + beta_2 * storedELen;


    // * Main loop of brilliant, image segmentation algorithm:-) *
    long loopIteration = 1;				// * Iteration counter *
    bool runSimulation = false;
    if(iterations == 0  &&  PMFRate == 0.0)  runSimulation = true;
                                          // * ----------------- *
                                          // *  Simulation loop  *
                                          // * ----------------- *
    while (runSimulation)
    {
        // * Setting inverse temperature. *
        beta_1 = 20.0 + 0.009 * loopIteration; //powf((float)loopIteartion, 1.0);
        beta_2 = 0.0;

        // * Recalculate energy for new values of inverse temperature. *
        engH = beta_1 * storedArea + beta_2 * storedELen;

        PMF<double> * clone = pmf->Clone();
        double angle = Uniform(0.0, 2.0*M_PI);

        // * Random choice of the operation for PMF modification. *
        double limit1, limit2;
        CALCULATE_LIMITS (pmf, areaOfPMF, angle, &limit1, &limit2);

        bool restore = MODIFY_CONFIGURATION (pmf, angle, Uniform(0.0, 1.0), limit1, limit2, bSize);
        if (restore == false)
        {
            // * Calculating energy of modified configuration. *
            tmpArea = gsimg.CalculateScanLinesEnergy(pmf);
            tmpELen = 0.0;
            //fprintf(stderr, "[%.2f].", tmpArea);  fflush(stderr);

            double newH = beta_1 * tmpArea + beta_2 * tmpELen;

            // * Mechanism of simulated annealing. *
            if (newH <= engH)
            {
                storedArea = tmpArea;
                storedELen = tmpELen;
                //fprintf(stderr, "(%.0f-%1.4f)", size, storedArea);
            }
            else {
                double  limit = exp(- (newH - engH));
                double chance = Uniform(0.0, 1.0);
                if (chance < limit)
                {
                    storedArea = tmpArea;
                    storedELen = tmpELen;
                }
                else { restore = true; }
            }
        }

        if (restore == true) {
            swap(pmf, clone);
            delete clone;
        }

        /* Saving value of PMF rate. *
        if(graphStep != 0)
        {
            if(qq % graphStep == 0)  fprintf(PMRfd, "%f\n", storedArea);
            fflush(PMRfd);
        }
        //*/
        // * Testing conditions for stoping simulation. *
        fprintf(stderr, ".");  fflush(stderr);
        loopIteration++;

        if (iterations > 0  &&  loopIteration >= iterations)  runSimulation = false;
        if (PMFRate > 0.0  &&  PMFRate > storedArea) runSimulation = false;
    }
    //* ------------------------- *
    //*  End of simulation loop.  *
    //* ------------------------- *

    return;
}
