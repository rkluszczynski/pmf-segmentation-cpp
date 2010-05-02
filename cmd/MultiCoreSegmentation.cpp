#include "MultiCoreSegmentation.h"

#include <omp.h>
#include <sstream>

#include "SegmentationParameters.h"


MultiCoreSegmentation::MultiCoreSegmentation (int num) : numberOfThreads(num)
{
    numberOfThreads = 2;
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
    sparam.SetSeed (10);

    sparam.SetInitialFile ("output/_shaked-pmf.txt");
    sparam.SetPictureFile ("input/bush-gauss-histogramcurvation.png");
    //char * pictureFile = "input/ring-spread.png";

    sparam.SetOutputDirectory ("output/");
    sparam.SetOutputFile ("output-test-file.txt");

    sparam.SetIterationsNumber (0L);
    sparam.SetPMRRate (.07);

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
    int syncSteps = 0;

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
            if (! sims[id]->CheckRunningCondition()) done = true;

            if (sync  or  syncSteps == 200  or  done)
            {
                #pragma omp critical
                    sync = true;
                #pragma omp barrier
                #pragma omp master
                    {
                        printf("sync::begin()\n");

                        scanf("%*c");

                        syncSteps = 0;
                        sync = false;
                        printf("sync::end()\n");
                    }
                #pragma omp barrier
                    if (done) nextStep = false;
            }
            ++syncSteps;
        }
        sims[id]->FinishSimulation();
    }

#pragma omp barrier
    fprintf(stderr, "simulations::end()");
}

