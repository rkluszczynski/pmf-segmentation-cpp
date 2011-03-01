#ifndef PROBABILITY_H
#define PROBABILITY_H

#include <cstdlib>
#include <cmath>
#include <utility>

#include "dSFMT.h"

class DoubleProbability
{
    public:
        DoubleProbability();
        DoubleProbability(time_t seed);
        virtual ~DoubleProbability();

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
        double GetMTUniform()  { return dsfmt_genrand_close_open(&dsfmt); }
        inline
        double GetStdUniform()  { return double(rand()) * _norm; }

    private:
        time_t  _seed;
        double  _norm;
        dsfmt_t dsfmt;
};


#endif // PROBABILITY_H
