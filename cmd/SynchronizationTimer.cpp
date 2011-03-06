#include "SynchronizationTimer.h"
#include "MultiCoreTypes.h"

//#include "probability.hpp"
#include "DoublePRNG.h"

#include <cassert>

SynchronizationTimer::SynchronizationTimer(int sType) : strategyType(sType)
{
    syncSteps = 1;
    syncProb = 0.;
}

SynchronizationTimer::~SynchronizationTimer()
{
    //dtor
}

bool
SynchronizationTimer::CheckSynchronizationTime()
{
    bool timeForSynchronization = false;
    switch (strategyType)
    {
        case IndependentStrategy :
        case MinimalRateStrategy :
        case GibbsRandomizationStrategy :
                                    if (syncSteps > 0) --syncSteps;
                                    timeForSynchronization = (syncSteps == 0);
                                    break;;
        case ParallelTemperingStrategy :
                                    {
                                        ///double fate = pmf::Probability::Uniform(0., 1.);
                                        double fate = pmf::PRNG->GetUniform(0., 1.);
                                        timeForSynchronization = (fate < syncProb);
                                        break;;
                                    }
        default :
                    assert("wrong strategy type" and false);
    }
    return timeForSynchronization;
}

void
SynchronizationTimer::SetStepCount(int steps)
{
    syncSteps = steps;
}

void
SynchronizationTimer::SetSyncProbability(double prob)
{
    syncProb = prob;
}
