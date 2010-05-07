#include "MultiCoreSegmentation.h"

#include <omp.h>
#include <sstream>

#include "SegmentationParameters.h"


MultiCoreSegmentation::MultiCoreSegmentation (int num) : numberOfThreads(num), strategy(GibbsRandomizationStrategy)
{
    numberOfThreads = 2;
    numberOfStepsToSync = 1500;
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
    sparam.SetPMRRate (.15);

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
            if (not sims[id]->CheckRunningCondition()) done = true;

            if (sync  or  syncSteps == 0  or  done)
            {
                #pragma omp critical
                    sync = true;
                #pragma omp barrier
                #pragma omp single
                    {
                        printf("sync::begin() =>  %i:%i\n", id, syncSteps);

                        if (done) printf(" IT IS DONE \n");
                        printf("[ energy during sync ] :");
                        for (int i = 0; i < omp_get_num_threads(); ++i)
                            printf("  [%i]=%.5lf(%.3lf)", i, sims[i]->CalculateImageEnergy(), sims[i]->GetStoredImageEnergy());
                        printf("\n");

                        if (not done)
                        {
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
                        }
                        scanf("%*c");

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

    double energies[numberOfThreads];
    for (int i = 0; i < numberOfThreads; ++i)  energies[i] = simulations[i]->GetStoredImageEnergy();
    double * mineng = min_element(energies, energies + numberOfThreads);
    int thid = mineng - energies;
    printf("NUMBER __%i__ ended!\n", thid);

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
    double pmrs[numberOfThreads];
    for (int i = 0; i < numberOfThreads; ++i) pmrs[i] = simulations[i]->GetStoredImageEnergy();
    for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", pmrs[i]);  printf("\n");

    double * minptr = min_element(pmrs, pmrs + numberOfThreads);
    int minpos = minptr - pmrs;
    printf("[ sync ] : minimum at %i with value %.5lf\n", minpos, *minptr);

    double minval = *minptr;
    for (int i = 0; i < numberOfThreads; ++i)  pmrs[i] -= minval;
    //for (int i = 0; i < numberOfThreads; ++i) printf(" %.7lf", pmrs[i]);  printf("\n");

    double weights[numberOfThreads];
    for (int i = 0; i < numberOfThreads; ++i) weights[i] = exp(-pmrs[i]);
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

