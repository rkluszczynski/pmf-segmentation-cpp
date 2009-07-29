#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "simulation.hpp"

namespace pmf
{

    class BinarySegmentation : public SimulatedAnnealingSimulation<double>
    {
        public:
            BinarySegmentation();
            virtual ~BinarySegmentation();

        protected:
            virtual bool  CheckRunningCondition();
            virtual bool    CheckApplyCondition();
            virtual double CalculateHamiltonian();
            virtual void       MakeModification();
            virtual void      ApplyModification();
            virtual void     CancelModification();

        private:
    };

}

#endif // SEGMENTATION_H
