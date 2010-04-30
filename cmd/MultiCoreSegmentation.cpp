#include "MultiCoreSegmentation.h"

#include <omp.h>
#include <sstream>


MultiCoreSegmentation::MultiCoreSegmentation (int num) : numberOfThreads(num)
{
    //ctor
    if (numberOfThreads > 0) omp_set_num_threads(num);
    else numberOfThreads = omp_get_num_threads();

    fprintf(stderr, "Number of threads : %i\n", numberOfThreads);
    scanf("%*c");

    simulations = new pmf::BinarySegmentation * [numberOfThreads];

    double sizeArak = 3.0;
    char * initialFile = NULL;
    time_t seed = 0;
    char * pictureFile = "input/ring-spread.png";

    long iterations = 0L;
    double pmrStop = .07;

    pmf::BinarySegmentation * * sims = simulations;
#pragma omp parallel default(none) \
                firstprivate(sizeArak, initialFile, seed, pictureFile, iterations, pmrStop) \
                shared(sims)
    {
        int id = omp_get_thread_num();
        std::stringstream ssout;
        ssout << "output-test" << "_thread-" << id << ".txt";
        sims[id] =
            new pmf::BinarySegmentation( sizeArak, sizeArak, initialFile, ssout.str().c_str(), seed + id, pictureFile, iterations, pmrStop );
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

            if (sync  or  syncSteps == 100  or  done)
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


    fprintf(stderr, "simulations::end()");
}

