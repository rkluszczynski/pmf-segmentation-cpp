#include "MultiCoreSegmentation.h"

#include <cstdio>
#include <cassert>
#include <sstream>

#include "SegmentationParameters.h"
#include "SynchronizationTimer.h"
//#include "../cmd/DoubleProbability.h"
#include "DoublePRNG.h"

#include <omp.h>

//#pragma omp threadprivate(__PRETTY_FUNCTION__)

extern int _tmp_seed;

MultiCoreSegmentation::MultiCoreSegmentation (SegmentationParameters & segparams, unsigned noOfThreads) : numberOfThreads(noOfThreads),
    strategy(MinimalRateStrategy)
    //strategy(GibbsRandomizationStrategy)
    //strategy(ParallelTemperingStrategy)
{
    numberOfThreads = 2;
    numberOfStepsToSync = 750;

    //ctor
    if (numberOfThreads > 0u) omp_set_num_threads(numberOfThreads);
    else numberOfThreads = omp_get_num_threads();

    SegmentationParameters sparam = segparams;

    fprintf(stderr, "Number of threads : %i\n", numberOfThreads);
    scanf("%*c");

    simulations = new pmf::BinarySegmentation * [numberOfThreads];


    FILE * stream = stderr;
    pmf::BinarySegmentation * * sims = simulations;
#pragma omp parallel default(none) \
                firstprivate(sparam) \
                shared(sims,stream)
    {
        int id = omp_get_thread_num();
        printf("[%i] ctor::parallel region::begin()\n", id);

        std::stringstream ssout;
        if (numberOfThreads == 1u)
            ssout << "singiel_";
        else
            ssout << "th" << id << "of" << numberOfThreads << "_";
        sparam.SetOutputPrefix (ssout.str().c_str());
        sparam.SetSeed (sparam.GetSeed() + id);

        sims[id] = new pmf::BinarySegmentation( sparam );
        sparam.PrintParameters(stream);

        printf("[%i] ctor::parallel region::end()\n", id);
    }
}


MultiCoreSegmentation::~MultiCoreSegmentation ()
{
    //dtor
    REP(i, numberOfThreads)  delete simulations[i];
    delete[] simulations;
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
#pragma omp parallel default(none) shared(sims, sync, done, erno)
    {
        int id = omp_get_thread_num();
        bool nextStep = true;
        SynchronizationTimer timer(this->GetStrategyType(), sims[id]->GetPRNG());
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
                    {
                        assert("wrong strategy type for timer" and false);
                        //fprintf(stderr, "wrong strategy type for timer");
                        //exit(-1);
                    }
        }

        sims[id]->PrepareSimulation();
        while (nextStep)
        {
            //sims[id]->SetPRNG(&(dps[id]));
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
                                                            UseIndependentStrategy(id);
                                                            break;;
                                case MinimalRateStrategy :
                                                            numberOfStepsToSync += 250;
                                                            UseMinimalRateStrategy(id);
                                                            break;;
                                case GibbsRandomizationStrategy :
                                                            numberOfStepsToSync += 250;
                                                            UseGibbsRandomizationStrategy(id);
                                                            break;;
                                case ParallelTemperingStrategy :
                                                            UseParallelTemperingStrategy(id);
                                                            break;;
                                default :
                                            erno = 100;
                                            done = true;
                                            break;
                            }
                        }
                        scanf("%*c");

                        sync = false;
                        printf("sync::end()\n");
                    }
                #pragma omp barrier
                    if (done) { nextStep = false; }
                    else
                    {
                        //syncSteps = numberOfStepsToSync;
                        timer.SetStepCount(numberOfStepsToSync);
                        //srand( rand() + id * 100 );
                    }
            }
        } // end while
        sims[id]->FinishSimulation();
    } // end parallel region
#pragma omp barrier
    if (!erno)
    {
        double energies[numberOfThreads];
        for (unsigned i = 0; i < numberOfThreads; ++i)  energies[i] = simulations[i]->GetStoredImagePMR();
        double * mineng = min_element(energies, energies + numberOfThreads);
        unsigned thid = unsigned(mineng - energies);
        printf("NUMBER __%u__ ended!\n", thid);
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
MultiCoreSegmentation::UseIndependentStrategy (int id)
{
}

void
MultiCoreSegmentation::UseMinimalRateStrategy (int id)
{
    vector<double> pmrs(numberOfThreads);
    for (unsigned i = 0; i < numberOfThreads; ++i) pmrs[i] = simulations[i]->GetStoredImagePMR();
    for (unsigned i = 0; i < numberOfThreads; ++i) printf(" %.7lf", pmrs[i]);  printf("\n");

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
MultiCoreSegmentation::UseGibbsRandomizationStrategy (int id)
{
    //double pmrs[numberOfThreads];
    double energies[numberOfThreads];
    //for (int i = 0; i < numberOfThreads; ++i) pmrs[i] = simulations[i]->GetStoredImagePMR();
    for (unsigned i = 0; i < numberOfThreads; ++i) energies[i] = simulations[i]->GetStoredSegmentationEnergy();
    for (unsigned i = 0; i < numberOfThreads; ++i) printf(" %.7lf", energies[i]);  printf("\n");

    double * minptr = min_element(energies, energies + numberOfThreads);
    int minpos = minptr - energies;
    printf("[ sync ] : minimum at %i with value %.5lf\n", minpos, *minptr);

    double minval = *minptr;
    for (unsigned i = 0; i < numberOfThreads; ++i)  energies[i] -= minval;
    for (unsigned i = 0; i < numberOfThreads; ++i) printf(" %.7lf", energies[i]);  printf("\n");

    double weights[numberOfThreads];
    for (unsigned i = 0; i < numberOfThreads; ++i) weights[i] = exp(-energies[i]);
    //for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", weights[i]);  printf("\n");

    double probs_prefsum[numberOfThreads];
    probs_prefsum[0] = weights[0];
    for (unsigned i = 1; i < numberOfThreads; ++i) probs_prefsum[i] = (probs_prefsum[i-1] + weights[i]);
    //for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", probs_prefsum[i]);  printf("\n");

    ///double rand = pmf::Probability::Uniform<double>(0., probs_prefsum[numberOfThreads-1]);
    double rand = simulations[id]->GetPRNG()->GetUniform(0., probs_prefsum[numberOfThreads-1]);
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
MultiCoreSegmentation::UseParallelTemperingStrategy (int id)
{
    printf("[ sync ] :: begin() \n");
    printf("[ sync ] : parallel tempering strategy \n");

    double energies[numberOfThreads];
    for (unsigned i = 0; i < numberOfThreads; ++i) energies[i] = simulations[i]->GetStoredSegmentationEnergy();
    for (unsigned i = 0; i < numberOfThreads; ++i) printf(" %.7lf", energies[i]);  printf("\n");

    for (unsigned i = 0; i < numberOfThreads; ++i)
    {
        unsigned swapper = unsigned(rand() % (numberOfThreads - 1u));
        if (swapper >= i)  ++swapper;

        printf("[ swap ] : trying %d <-> %d \n", i, swapper);
        assert(i != swapper);

        bool doTheSwap = true;
        double delta = energies[swapper] - energies[i];
        if (delta > 0.)
        {
            double limit = exp(-delta);
            ///double chance = pmf::Probability::Uniform(0., 1.);
            double chance = simulations[id]->GetPRNG()->GetUniform(0., 1.);
            if (chance > limit)
                doTheSwap = false;
        }

        printf("[ swap ] : %s\n", doTheSwap ? "done" : "NOT DONE");
        if (doTheSwap) std::swap(simulations[i], simulations[swapper]);
    }
    printf("[ sync ] :: end() \n");
    for (unsigned i = 0; i < numberOfThreads; ++i) printf(" %.7lf", simulations[i]->GetStoredSegmentationEnergy());  printf("\n");
}
