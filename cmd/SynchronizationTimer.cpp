#include "SynchronizationTimer.h"
#include "MultiCoreTypes.h"
#include <cassert>

SynchronizationTimer::SynchronizationTimer(int sType) : strategyType(sType)
{
    syncSteps = 1;
}

SynchronizationTimer::~SynchronizationTimer()
{
    //dtor
}

bool
SynchronizationTimer::CheckSynchronizationTime()
{
    switch (strategyType)
    {
        case IndependentStrategy :
        case MinimalRateStrategy :
        case GibbsRandomizationStrategy :
                                    if (syncSteps > 0) --syncSteps;
                                    break;;
        default :
                    assert("wrong strategy type" and false);
    }
    return syncSteps == 0;
}

void
SynchronizationTimer::SetStepCount(int steps)
{
    syncSteps = steps;
}
