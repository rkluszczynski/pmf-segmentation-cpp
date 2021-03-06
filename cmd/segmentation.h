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
            BinarySegmentation();
            BinarySegmentation(SegmentationParameters, unsigned = 0u);
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

            void testImagePrefixSums();


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
            void OnInit();

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

            double beta_LJn;
            double beta_LJg;

            unsigned threadId;
    };

}

#endif // SEGMENTATION_H
