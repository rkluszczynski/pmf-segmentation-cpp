#ifndef MULTICORESEGMENTATION_H
#define MULTICORESEGMENTATION_H

#include "segmentation.h"
#include "MultiCoreTypes.h"
#include "DoublePRNG.h"

class MultiCoreSegmentation
{
    public:
        MultiCoreSegmentation(int = 0);
        virtual ~MultiCoreSegmentation();

        void SimulateOnMultiCore();


    protected:
        void UseIndependentStrategy(int);
        void UseMinimalRateStrategy(int);
        void UseGibbsRandomizationStrategy(int);
        void UseParallelTemperingStrategy(int);


    private:
        inline
        StrategyType GetStrategyType() { return strategy; }


        int numberOfThreads, numberOfStepsToSync;
        StrategyType strategy;
        pmf::BinarySegmentation ** simulations;
};

#endif // MULTICORESEGMENTATION_H
