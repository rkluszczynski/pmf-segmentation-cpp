
#include "grayscaleimage.hpp"


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

#include <cstdio>
void SimulateBinarySegmentation (
                            char * imageFileName,
                            PMF<double> * & pmf,
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
    fprintf(stderr, "[ ENERGY ] : %lf  (%.2lf)", engH, tmpArea);  fflush(stderr);

    //return;
    // * Main loop of algorithm *
    long loopIteration = 1;				// * Iteration counter *
    bool runSimulation = true;
    if(iterations == 0  &&  PMFRate == 0.0) runSimulation = false;
                                          // * ----------------- *
                                          // *  Simulation loop  *
                                          // * ----------------- *
#if pmf_LOG_ADD
    //ofstream fout1("output/abcdef.txt");
    ofstream fout1;
#endif
    while (runSimulation)
    {
        char plik[256];
        sprintf(plik, "output/abcdef.txt");
        //if (loopIteration >= 110000  &&  loopIteration % 1000 == 0)
        //if ((loopIteration >= 140964  &&  loopIteration <= 141000))
        //if (loopIteration == 586 || loopIteration == 587 || (loopIteration >= 586  &&  loopIteration <= 102))
        if (
            (loopIteration >= 140964)
            ||  (loopIteration % 10000 == 0)
        )
        {
            sprintf(plik, "output/pmf-iter-%li.txt", loopIteration);
            pmf->SaveConfiguration(plik);
            sprintf(plik, "output/debug-iter-%li.txt", loopIteration);
        }
        if (loopIteration == 120462) pmf->SaveConfiguration("output/pmf-iter-120462.txt");
        //if (loopIteration == 586) exit(1);
        if (loopIteration >= 140964) saveOp = true;
#if pmf_LOG_ADD
        if (saveOp)
        {
            fout1.flush();
            fout1.close();
            fout1.open(plik);
            out.rdbuf(fout1.rdbuf());
            if (loopIteration == 2) {
                //out.rdbuf(fout1.rdbuf());
                //pmf->GetPMFConfiguration()->save_configuration(out);
                ;
            }
            out << std::endl << " ################################################################" << std::endl;
            out << std::endl << " ################################################################" << std::endl;
            out << " ############### SEGMENTATION STEP " << loopIteration << std::endl;
            out << std::endl << " ################################################################" << std::endl;
        }
#endif
        fprintf(stderr, "\n { %5li }  \n", loopIteration);  fflush(stderr);
        // * Setting inverse temperature. *
        beta_1 = 20.0 + 0.009 * loopIteration; //powf((float)loopIteartion, 1.0);
        beta_2 = 0.0;

        // * Recalculate energy for new values of inverse temperature. *
        engH = beta_1 * storedArea + beta_2 * storedELen;

        PMF<double> * clone = pmf->Clone();
        double angle = Uniform(0.0, 2.0*M_PI);

        // * Random choice of the operation for PMF modification. *
        bool restore = MODIFY_CONFIGURATION (pmf, areaOfPMF, angle, bSize);
        fprintf(stderr, "|%li|Przywroc:%s ", loopIteration, restore ? "TAK" : "NIE");  fflush(stderr);
        //if (loopIteration >= 140964  &&  loopIteration <= 141000) scanf("%lf", &beta_2);
#if pmf_LOG_ADD
        //pmf->SaveConfiguration("output/pmf-modified.txt");
        ;
#endif
        if (restore == false)
        {
            // * Calculating energy of modified configuration. *
            tmpArea = gsimg.CalculateScanLinesEnergy(pmf);
            tmpELen = 0.0;
            fprintf(stderr, "[%.2lf]", tmpArea);  fflush(stderr);

            double newH = beta_1 * tmpArea + beta_2 * tmpELen;

            // * Mechanism of simulated annealing. *
            if (newH <= engH)
            {
                storedArea = tmpArea;
                storedELen = tmpELen;
                //fprintf(stderr, "(%.0f-%1.4f)", pmf->GetPMFPointsNumber(), storedArea);
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
            fprintf(stderr, "{Getting back pmf}");  fflush(stderr);
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
        fprintf(stderr, ".\n");  fflush(stderr);

        if (iterations > 0  &&  loopIteration >= iterations)  runSimulation = false;
        if (PMFRate > 0.0  &&  PMFRate > storedArea) runSimulation = false;
        ++loopIteration;

        pmf->TestConfigurationPoints();
    }
    // * ------------------------- *
    // *  End of simulation loop.  *
    // * ------------------------- *
    return;
}
