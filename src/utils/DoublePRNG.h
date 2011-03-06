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
            inline
            //double GetUniform()  { return GetStdUniform(); }
            double GetUniform()  { return GetMTUniform(); }

        protected:
            void OnInit();

            inline
            double GetMTUniform()  { return dsfmt_genrand_open_open(&dsfmt); }
            inline
            double GetStdUniform()  { return double(rand()) * _norm; }

        private:
            time_t  _seed;
            double  _norm;
            dsfmt_t dsfmt;
    };

}

#endif // DOUBLEPRNG_H
