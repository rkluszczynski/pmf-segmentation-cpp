#ifndef MULTICORESEGMENTATION_H
#define MULTICORESEGMENTATION_H

#include "segmentation.h"


class MultiCoreSegmentation
{
    public:
        MultiCoreSegmentation(int = 0);
        virtual ~MultiCoreSegmentation();

        void SimulateOnMultiCore();


    protected:
        void UseIndependentStrategy();
        void UseMinimalRateStrategy();
        void UseGibbsRandomizationStrategy();


    private:
        enum StrategyType
        {
            IndependentStrategy,
            MinimalRateStrategy,
            GibbsRandomizationStrategy
        };

        inline StrategyType GetStrategyType() { return strategy; }


        int numberOfThreads, numberOfStepsToSync;
        StrategyType strategy;
        pmf::BinarySegmentation ** simulations;
};

#endif // MULTICORESEGMENTATION_H
