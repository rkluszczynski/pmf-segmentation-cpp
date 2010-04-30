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

    private:
        int numberOfThreads;
        pmf::BinarySegmentation ** simulations;
};

#endif // MULTICORESEGMENTATION_H
