#ifndef SYNCHRONIZATIONTIMER_H
#define SYNCHRONIZATIONTIMER_H

#include "DoublePRNG.h"

class SynchronizationTimer
{
    public:
        SynchronizationTimer(int, pmf::DoublePRNG *);
        virtual ~SynchronizationTimer();

        bool CheckSynchronizationTime();

        void SetStepCount(int);
        void SetSyncProbability(double);

        inline
        int GetStepCount() { return syncSteps; }

    protected:
    private:
        int strategyType;
        int syncSteps;
        double syncProb;
        pmf::DoublePRNG * PRNG;
};

#endif // SYNCHRONIZATIONTIMER_H
