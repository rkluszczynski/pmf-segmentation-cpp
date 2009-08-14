#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "simulation.hpp"
#include "grayimage.h"
#include "../include/statistics.h"


namespace pmf
{
    template <class REAL> class PMF;

    class BinarySegmentation : public SimulatedAnnealingSimulation<double>
    {
        public:
            BinarySegmentation(double, double, const char *, const char *, time_t, const char *, long, double);
            virtual ~BinarySegmentation();

            virtual void  Prepare ();
            virtual void  Finish ();

        protected:
            virtual void           PreIteration();
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
            double rate;
            PMF<double> * pmf;
            PMF<double> * clone;
            GrayscaleImage * img;
            const char * outputfile;

            double storedArea, storedElen;
            double areaOfPMF, angle;
            bool apply;
    };

}

#endif // SEGMENTATION_H
