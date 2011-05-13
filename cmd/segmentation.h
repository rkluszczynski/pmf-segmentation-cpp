#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "simulation.hpp"
#include "grayimage.h"
#include "../include/statistics.h"
#include "SegmentationParameters.h"


namespace pmf
{
    template <class REAL> class PMF;

    class BinarySegmentation : public SimulatedAnnealingSimulation<double>
    {
        public:
            BinarySegmentation(SegmentationParameters);
            BinarySegmentation(double, double, const char *, const char *, time_t, const char *, long, double);
            virtual ~BinarySegmentation();

            PMF<double> * GetPMF() const { return pmf; }
            void ReplacePMF(PMF<double> *);

            virtual void  Prepare ();
            virtual void  Finish ();

            virtual bool        CheckRunningCondition();
            inline double           GetStoredImagePMR() { return storedArea; }
            inline double GetStoredSegmentationEnergy() { return storedEnergy; }
                   double        CalculateImageEnergy();

            inline
            pmf::DoublePRNG * GetPRNG() { return prng; }


        protected:
            virtual void           PreIteration();
            virtual void          PostIteration();

            virtual bool    CheckApplyCondition();
            virtual double CalculateHamiltonian();
            virtual void       MakeModification();
            virtual void      ApplyModification();
            virtual void     CancelModification();

            virtual double         GetUniform01();

        private:
            SegmentationParameters parameters;

            long loopIteration;
            long iterations;
            double rate;
            PMF<double> * pmf;
            PMF<double> * clone;
            GrayscaleImage * img;
            const char * outputfile;

            pmf::DoublePRNG * prng;
            pmf::NumericalParameters * numerics;

            double storedArea, storedEnergy, storedElen;
            double tmpArea, tmpEnergy, tmpElen;
            double tmpLJn, tmpLJg, _sigma6, _sigma12, _rcutoff;
            double areaOfPMF, angle;
            bool apply;
    };

}

#endif // SEGMENTATION_H
