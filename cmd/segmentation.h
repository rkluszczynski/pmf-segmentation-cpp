#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "simulation.hpp"

namespace pmf
{

    class BinarySegmentation : public SimulatedAnnealingSimulation
    {
        public:
            BinarySegmentation();
            virtual ~BinarySegmentation();
        protected:
        private:
    };

}

#endif // SEGMENTATION_H
