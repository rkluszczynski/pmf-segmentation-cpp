
#include "grayscaleimage.hpp"

inline
void
ApplyFixedEdgePointsToConfiguration ( PMF<double> * & pmf, EdgePoints<double> * ep)
{
    char plik[256];
    int fileCounter = 0;
#if pmf_LOG_ADD
    ofstream rkout("output/rk.txt");
#endif
    /*
    for (int i = 0; i < ep->getPointsNumber(); i++)
    {
        cerr << "[ EDGE ] : point " << i << "   ";
        cerr << "( " << ep->get(i)->x << " ; " << ep->get(i)->y << " )   ";
        cerr << ep->get(i)->angle << " ~ " << degree2radians(ep->get(i)->angle) << endl;

        bool isOk = true;
        do {
            PMF<double> * clone = pmf->Clone();
            //pmf->AddBirthSegment( ep->get(i)->x, ep->get(i)->y, degree2radians(ep->get(i)->angle), ep );
            pmf->AddBirthSegment( ep->get(i)->x, ep->get(i)->y, ep->get(i)->angle, ep );

            cerr << "[ EDGE ] : checking ... ";
            isOk = true;
            for (int k = 0; k < i; k++) {
                cerr << k;
                if (ep->get(k)->pt == NULL)  { cerr << "-ERR"; isOk = false; }
                cerr << " ... ";
            }
            cerr << " checked" << endl;

            if (! isOk) {
                cerr << " swaping" << endl;
                swap(pmf, clone);
                cerr << " swaped" << endl;
            }
            delete clone;
        }
        while (! isOk);

        cerr << " saving " << endl;
        sprintf(plik, "output/edge-iter-%d.txt", ++fileCounter);
        cerr << plik << endl;
        pmf->SaveConfiguration(plik);
    }
    return;
    //*/
    while (true)
    {
#if pmf_LOG_ADD
        if (fileCounter == 13) { break; }
        if (fileCounter == 12)
        {
            out.rdbuf(rkout.rdbuf());
        }
        out << "##############################################################" << endl;
        out << "################################## POINT " << fileCounter << endl;
#endif
        int i = -1;
        for (int k = 0; k < ep->getPointsNumber(); k++)
            if (ep->get(k)->pt == NULL)  { i = k; break; }
        if (i < 0) break;

        cerr << "[ EDGE ] : point " << i << "   ";
        cerr << "( " << ep->get(i)->x << " ; " << ep->get(i)->y << " )   ";
        cerr << ep->get(i)->angle << " ~ " << degree2radians(ep->get(i)->angle) << endl;

        pmf->AddBirthSegment( ep->get(i)->x, ep->get(i)->y, ep->get(i)->angle, ep );

        cerr << "[ EDGE ] : checking ... ";
        for (int k = 0; k < ep->getPointsNumber(); k++) {
            cerr << k;
            if (ep->get(k)->pt == NULL)  cerr << "-ERR";
            cerr << " ... ";
        }
        cerr << " done" << endl;

        sprintf(plik, "output/edge-iter-%d.txt", ++fileCounter);
        cerr << plik << endl;
        pmf->SaveConfiguration(plik);
    }
}


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
        double x = Uniform(0.0, pmf->GetPMFWidth());
        double y = Uniform(0.0, pmf->GetPMFHeight());
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
void
SimulateFixedEdgePointsSegmentation (
                            char * imageFileName,
                            PMF<double> * & pmf,
                            long iterations,
                            double PMFRate,
                            double bSize
                        )
{
    //* Initializing some variables. *
    GrayScaleImage gsimg(imageFileName);
    unsigned int pixCount = 10;

    cerr << "getting ep " << endl;
    EdgePoints<double> * ep = gsimg.GetRandomEdgePixels2(pixCount, 2, pmf->GetPMFWidth(), pmf->GetPMFHeight());

    ep->PrintData(cerr);


    ApplyFixedEdgePointsToConfiguration(pmf, ep);
    pmf->SaveConfiguration("output/edgepoints.cf");
    return;


    double    beta_1 = 20.0;
    double    beta_2 = 0.0;
    double areaOfPMF = M_PI * pmf->GetPMFHeight() * pmf->GetPMFWidth();

    double tmpArea, tmpELen, storedArea, storedELen;
    tmpArea = storedArea = gsimg.CalculateScanLinesEnergy(pmf);
    tmpELen = storedELen = 0.0; //estimateEdgesLen(Hist);

    double engH = beta_1 * storedArea + beta_2 * storedELen;
    fprintf(stderr, "[ ENERGY ] : %lf  (%.2lf)", engH, tmpArea);  fflush(stderr);

    //return;


    long loopIteration = 1;				// * Iteration counter *
    bool runSimulation = true;
    if(iterations == 0  &&  PMFRate == 0.0) runSimulation = false;
                                          // * ----------------- *
                                          // *  Simulation loop  *
                                          // * ----------------- *
    while (runSimulation)
    {
#if pmf_LOG_ADD
        out << std::endl << " ################################################################" << std::endl;
        out << std::endl << " ################################################################" << std::endl;
        out << " ############### SEGMENTATION STEP " << loopIteration << std::endl;
        out << std::endl << " ################################################################" << std::endl;
#endif
        fprintf(stderr, " {%5li }  ", loopIteration);  fflush(stderr);
        // * Setting inverse temperature. *
        beta_1 = 20.0 + 0.009 * loopIteration; //powf((float)loopIteartion, 1.0);
        beta_2 = 0.0;

        // * Recalculate energy for new values of inverse temperature. *
        engH = beta_1 * storedArea + beta_2 * storedELen;

        PMF<double> * clone = pmf->Clone();
        double angle = Uniform(0.0, 2.0*M_PI);

        // * Random choice of the operation for PMF modification. *
        bool restore = MODIFY_CONFIGURATION (pmf, areaOfPMF, angle, bSize);
        fprintf(stderr, "Przywroc:%s ", restore ? "TAK" : "NIE");  fflush(stderr);
#if pmf_LOG_ADD
        pmf->SaveConfiguration("output/pmf-modified.txt");
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

        // * Testing conditions for stoping simulation. *
        if (iterations > 0  &&  loopIteration >= iterations)  runSimulation = false;
        if (PMFRate > 0.0  &&  PMFRate > storedArea) runSimulation = false;

        fprintf(stderr, ".\n");  fflush(stderr);
        loopIteration++;
    }
    // * ------------------------- *
    // *  End of simulation loop.  *
    // * ------------------------- *
    return;
}
