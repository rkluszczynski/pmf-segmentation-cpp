#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "simulation.hpp"

namespace pmf
{
    template <class REAL> class PMF;

    class BinarySegmentation : public SimulatedAnnealingSimulation<double>
    {
        public:
            BinarySegmentation(const char *, long, double);
            virtual ~BinarySegmentation();

        protected:
            virtual void          PostIteration();

            virtual bool  CheckRunningCondition();
            virtual bool    CheckApplyCondition();
            virtual double CalculateHamiltonian();
            virtual void       MakeModification();
            virtual void      ApplyModification();
            virtual void     CancelModification();

        private:
            long loopIteration;
            long iterations;
            PMF<double> * clone;
            double rate;
    };

}

#endif // SEGMENTATION_H
