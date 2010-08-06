#include "MultiCoreSegmentation.h"

#include <cstdio>
#include <cassert>
#include <sstream>

#include "SegmentationParameters.h"
#include "SynchronizationTimer.h"

#include <omp.h>

//#pragma omp threadprivate(__PRETTY_FUNCTION__)

MultiCoreSegmentation::MultiCoreSegmentation (int num) : numberOfThreads(num),
    strategy(MinimalRateStrategy)
    //strategy(GibbsRandomizationStrategy)
    //strategy(ParallelTemperingStrategy)
{
    numberOfThreads = 1;
    numberOfStepsToSync = 750;
    //ctor
    if (numberOfThreads > 0) omp_set_num_threads(numberOfThreads);
    else numberOfThreads = omp_get_num_threads();

    fprintf(stderr, "Number of threads : %i\n", numberOfThreads);
    scanf("%*c");

    simulations = new pmf::BinarySegmentation * [numberOfThreads];

    SegmentationParameters sparam;
    sparam.SetFieldHeight (3.0);
    sparam.SetFieldWidth (3.0);
    sparam.SetSeed (7217);

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
    sparam.SetPMRRate (.018);

    FILE * stream = stderr;
    pmf::BinarySegmentation * * sims = simulations;
#pragma omp parallel default(none) \
                firstprivate(sparam) \
                shared(sims,stream)
    {
        int id = omp_get_thread_num();
        std::stringstream ssout;
        if (numberOfThreads == 1)
            ssout << "singiel_";
        else
            ssout << "th" << id << "of" << numberOfThreads << "_";
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
    int erno = 0;

    pmf::BinarySegmentation * * sims = simulations;
    //int syncSteps = 1;//numberOfStepsToSync;
    //assert(syncSteps > 0);

    bool sync = false;
    bool done = false;
#pragma omp parallel default(none) shared(sims, sync, done, erno) //firstprivate(syncSteps)
    {
        int id = omp_get_thread_num();
        bool nextStep = true;
        SynchronizationTimer timer(this->GetStrategyType());
        switch (this->GetStrategyType())
        {
            case IndependentStrategy :
            case MinimalRateStrategy :
            case GibbsRandomizationStrategy :
                                        timer.SetStepCount(numberOfStepsToSync);
                                        break;;
            case ParallelTemperingStrategy :
                                        timer.SetSyncProbability(0.001);
                                        break;;
            default :
                        assert("wrong strategy type for timer" and false);
        }

        sims[id]->PrepareSimulation();
        while (nextStep)
        {
            sims[id]->RunNextStep();
            //printf(" [[{%i}[%.7lf]]] ", id, sims[id]->CalculateImageEnergy());
            if (not sims[id]->CheckRunningCondition()) done = true;

            //--syncSteps;
            bool timeToSync = timer.CheckSynchronizationTime();
            //if (sync  or  syncSteps == 0  or  done)
            if (timeToSync or sync or done)
            {
                #pragma omp critical
                    sync = true;
                #pragma omp barrier
                #pragma omp single
                    {
                        //printf("sync::begin() =>  %i:%i\n", id, syncSteps);
                        printf("sync::begin() =>  %i:%i\n", id, timer.GetStepCount());

                        if (done) printf(" IT IS DONE \n");
                        printf("[ pmr during sync ] :");
                        for (int i = 0; i < omp_get_num_threads(); ++i)
                            printf("  [%i]=%.5lf(%.3lf)", i, sims[i]->CalculateImageEnergy(), sims[i]->GetStoredImagePMR());
                        printf("\n");

                        if (not done)
                        {
                            switch (this->GetStrategyType())
                            {
                                case IndependentStrategy :
                                                            UseIndependentStrategy();
                                                            break;;
                                case MinimalRateStrategy :
                                                            numberOfStepsToSync += 250;
                                                            UseMinimalRateStrategy();
                                                            break;;
                                case GibbsRandomizationStrategy :
                                                            numberOfStepsToSync += 250;
                                                            UseGibbsRandomizationStrategy();
                                                            break;;
                                case ParallelTemperingStrategy :
                                                            UseParallelTemperingStrategy();
                                                            break;;
                                default :
                                            erno = 100;
                                            done = true;
                                            break;
                            }
                        }
                        //scanf("%*c");

                        sync = false;
                        printf("sync::end()\n");
                    }
                #pragma omp barrier
                    if (done) { nextStep = false; }
                    else
                    {
                        //syncSteps = numberOfStepsToSync;
                        timer.SetStepCount(numberOfStepsToSync);
                        srand( rand() + id * 100 );
                    }
            }
        } // end while
        sims[id]->FinishSimulation();
    } // end parallel region
#pragma omp barrier
    if (!erno)
    {
        double energies[numberOfThreads];
        for (int i = 0; i < numberOfThreads; ++i)  energies[i] = simulations[i]->GetStoredImagePMR();
        double * mineng = min_element(energies, energies + numberOfThreads);
        int thid = mineng - energies;
        printf("NUMBER __%i__ ended!\n", thid);
    }
    else
    {
        switch (erno)
        {
            case 100 : assert("NOT KNOWN STRATEGY" and false);
        }
    }
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
    for (int i = 0; i < numberOfThreads; ++i) pmrs[i] = simulations[i]->GetStoredImagePMR();
    for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", pmrs[i]);  printf("\n");

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
    //double pmrs[numberOfThreads];
    double energies[numberOfThreads];
    //for (int i = 0; i < numberOfThreads; ++i) pmrs[i] = simulations[i]->GetStoredImagePMR();
    for (int i = 0; i < numberOfThreads; ++i) energies[i] = simulations[i]->GetStoredSegmentationEnergy();
    for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", energies[i]);  printf("\n");

    double * minptr = min_element(energies, energies + numberOfThreads);
    int minpos = minptr - energies;
    printf("[ sync ] : minimum at %i with value %.5lf\n", minpos, *minptr);

    double minval = *minptr;
    for (int i = 0; i < numberOfThreads; ++i)  energies[i] -= minval;
    for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", energies[i]);  printf("\n");

    double weights[numberOfThreads];
    for (int i = 0; i < numberOfThreads; ++i) weights[i] = exp(-energies[i]);
    //for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", weights[i]);  printf("\n");

    double probs_prefsum[numberOfThreads];
    probs_prefsum[0] = weights[0];
    for (int i = 1; i < numberOfThreads; ++i) probs_prefsum[i] = (probs_prefsum[i-1] + weights[i]);
    //for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", probs_prefsum[i]);  printf("\n");

    double rand = pmf::Probability::Uniform<double>(0., probs_prefsum[numberOfThreads-1]);
    //printf("[ rand ] : %.11lf\n", rand);
    double * randptr = lower_bound(probs_prefsum, probs_prefsum + numberOfThreads, rand);
    int randpos = randptr - probs_prefsum;
    printf("[ sync ] : gibbs random choice at %i with probability %.5lf\n", randpos, weights[randpos] / probs_prefsum[numberOfThreads-1]);

    //*
    for (int i = 0; i < omp_get_num_threads(); ++i)
    {
        if (i == randpos) continue;
        simulations[i]->ReplacePMF( simulations[randpos]->GetPMF() );
    }
    // */
}

void
MultiCoreSegmentation::UseParallelTemperingStrategy ()
{
    printf("[ sync ] :: begin() \n");
    printf("[ sync ] : parallel tempering strategy \n");

    double energies[numberOfThreads];
    for (int i = 0; i < numberOfThreads; ++i) energies[i] = simulations[i]->GetStoredSegmentationEnergy();
    for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", energies[i]);  printf("\n");

    for (int i = 0; i < numberOfThreads; ++i)
    {
        int swapper = rand() % (numberOfThreads - 1);
        if (swapper >= i)  ++swapper;

        printf("[ swap ] : trying %d <-> %d \n", i, swapper);
        assert(i != swapper);

        bool doTheSwap = true;
        double delta = energies[swapper] - energies[i];
        if (delta > 0.)
        {
            double limit = exp(-delta);
            double chance = pmf::Probability::Uniform(0., 1.);
            if (chance > limit)
                doTheSwap = false;
        }

        printf("[ swap ] : %s\n", doTheSwap ? "done" : "NOT DONE");
        if (doTheSwap) std::swap(simulations[i], simulations[swapper]);
    }
    printf("[ sync ] :: end() \n");
    for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", simulations[i]->GetStoredSegmentationEnergy());  printf("\n");
}
