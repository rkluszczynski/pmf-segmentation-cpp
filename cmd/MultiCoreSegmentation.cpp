#include "MultiCoreSegmentation.h"

#include <omp.h>
#include <sstream>

#include "SegmentationParameters.h"


MultiCoreSegmentation::MultiCoreSegmentation (int num) : numberOfThreads(num), strategy(IndependentStrategy)
{
    numberOfThreads = 1;
    numberOfStepsToSync = 500;
    //ctor
    if (numberOfThreads > 0) omp_set_num_threads(numberOfThreads);
    else numberOfThreads = omp_get_num_threads();

    fprintf(stderr, "Number of threads : %i\n", numberOfThreads);
    scanf("%*c");

    simulations = new pmf::BinarySegmentation * [numberOfThreads];

    SegmentationParameters sparam;
    sparam.SetFieldHeight (3.0);
    sparam.SetFieldWidth (3.0);
    sparam.SetInitialFile (NULL);
    sparam.SetSeed (17);

    //sparam.SetInitialFile ("output/_shaked-pmf.txt");
    sparam.SetInitialFile ("output/_shaked-pmf.txt");

    sparam.SetPictureFile ("input/tmp/szara-wisienka-do-segm.png");
    //sparam.SetPictureFile ("output/grzybek2.png");
    //sparam.SetPictureFile ("input/moj-grzybek-to-simulate.png");
    //sparam.SetPictureFile ("output/segm_kruki.png");
    //sparam.SetPictureFile ("input/bush-gauss-histogramcurvation.png");
    //char * pictureFile = "input/ring-spread.png";

    sparam.SetOutputDirectory ("output/");
    sparam.SetOutputFile ("output-test-file.txt");

    sparam.SetIterationsNumber (0L);
    sparam.SetPMRRate (.17);

    FILE * stream = stderr;
    pmf::BinarySegmentation * * sims = simulations;
#pragma omp parallel default(none) \
                firstprivate(sparam) \
                shared(sims,stream)
    {
        int id = omp_get_thread_num();
        std::stringstream ssout;
        ssout << "th" << id << "_";
        sparam.SetOutputPrefix (ssout.str().c_str());
        sparam.SetSeed (sparam.GetSeed() + id);

        sims[id] = new pmf::BinarySegmentation( sparam );
        sparam.PrintParameters(stream);
    }
}


MultiCoreSegmentation::~MultiCoreSegmentation ()
{
    //dtor
    REP(i, numberOfThreads)  delete simulations[i];
}


void
MultiCoreSegmentation::SimulateOnMultiCore ()
{
    fprintf(stderr, "simulations::begin()");

    pmf::BinarySegmentation * * sims = simulations;
    int syncSteps = numberOfStepsToSync;

    bool sync = false;
    bool done = false;
#pragma omp parallel default(none) shared(sims, sync, done) firstprivate(syncSteps)
    {
        int id = omp_get_thread_num();
        bool nextStep = true;

        sims[id]->PrepareSimulation();
        while (nextStep)
        {
            sims[id]->RunNextStep();
            //printf(" [[{%i}[%.7lf]]] ", id, sims[id]->CalculateImageEnergy());
            if (! sims[id]->CheckRunningCondition()) done = true;

            if (sync  or  syncSteps == 0  or  done)
            {
                #pragma omp critical
                    sync = true;
                #pragma omp barrier
                #pragma omp master
                    {
                        printf("sync::begin() =>  %i:%i\n", id, syncSteps);

                        if (done) printf(" IT IS DONE \n");
                        printf("[ energy during sync ] :");
                        for (int i = 0; i < omp_get_num_threads(); ++i)
                            printf("  [%i]=%.5lf(%.3lf)", i, sims[i]->CalculateImageEnergy(), sims[i]->GetStoredImageEnergy());
                        printf("\n");

                        switch (strategy)
                        {
                            case IndependentStrategy :
                                                        UseIndependentStrategy();
                                                        break;;
                            case MinimalRateStrategy :
                                                        UseMinimalRateStrategy();
                                                        break;;
                            case GibbsRandomizationStrategy :
                                                        UseGibbsRandomizationStrategy();
                                                        break;;
                            default :
                                        assert("NOT KNOWN STRATEGY" and false);
                                        break;
                        }
                        //scanf("%*c");

                        sync = false;
                        printf("sync::end()\n");
                    }
                #pragma omp barrier
                    if (done) nextStep = false;
                    syncSteps = numberOfStepsToSync;
            }
            --syncSteps;
        }
        sims[id]->FinishSimulation();
    }

#pragma omp barrier
    fprintf(stderr, "simulations::end()");
}


void
MultiCoreSegmentation::UseIndependentStrategy ()
{

}

void
MultiCoreSegmentation::UseMinimalRateStrategy ()
{
    vector<double> pmrs(numberOfThreads);
    for (int i = 0; i < numberOfThreads; ++i) pmrs[i] = simulations[i]->GetStoredImageEnergy();
    vector<double>::iterator minit = min_element(pmrs.begin(), pmrs.end());
    int minpos = minit - pmrs.begin();

    printf("[ sync ] : minimum at %i with value %.5lf\n", minpos, *minit);
    for (int i = 0; i < omp_get_num_threads(); ++i)
    {
        if (i == minpos) continue;
        simulations[i]->ReplacePMF( simulations[minpos]->GetPMF() );
    }
}

void
MultiCoreSegmentation::UseGibbsRandomizationStrategy ()
{

}

