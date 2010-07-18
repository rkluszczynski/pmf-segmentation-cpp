#ifndef SYNCHRONIZATIONTIMER_H
#define SYNCHRONIZATIONTIMER_H


class SynchronizationTimer
{
    public:
        SynchronizationTimer(int);
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
};

#endif // SYNCHRONIZATIONTIMER_H
