#ifndef DOUBLEPRNG_H
#define DOUBLEPRNG_H

#include <cstdlib>
#include <cmath>
#include <utility>

#include "dSFMT/dSFMT.h"

namespace pmf
{

    class DoublePRNG
    {
        public:
            DoublePRNG();
            DoublePRNG(time_t seed);
            virtual ~DoublePRNG();
            DoublePRNG(const DoublePRNG& other);
            DoublePRNG& operator=(const DoublePRNG& other);

            void DetermineBirthAngles(double &, double &);
            double DetermineUpdateAngle();
            double GetExp(double);
            double GetUniform(double, double);
            double GetUniform();

            unsigned int GetInt();

        protected:
            void OnInit();

        private:
            time_t  _seed;
            double  _norm;
            dsfmt_t dsfmt;
    };


    //extern DoublePRNG * PRNG;
}

#endif // DOUBLEPRNG_H
