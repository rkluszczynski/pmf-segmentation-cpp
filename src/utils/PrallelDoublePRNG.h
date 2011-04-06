#ifndef PRALLELDOUBLEPRNG_H
#define PRALLELDOUBLEPRNG_H

#include "dSFMT/dSFMT.h"
#include <omp.h>

#define BUFFOR 10000000

class PrallelDoublePRNG
{
    public:
        PrallelDoublePRNG(unsigned);
        virtual ~PrallelDoublePRNG();

            void DetermineBirthAngles(unsigned, double &, double &);
            double DetermineUpdateAngle(unsigned);
            double GetExp(unsigned, double);
            double GetUniform(unsigned, double, double);
            double GetUniform(unsigned);

            unsigned int GetInt(unsigned);


    protected:
        void GenerateRadoms();
        void GenerateRandomNumbers(unsigned, dsfmt_t *);

    private:
        dsfmt_t * states;
        double ** dnums;
        int ** inums;

        int * dpos[2];
        int * ipos[2];

        unsigned threadsCount;
        omp_lock_t lock_;
};

#endif // PRALLELDOUBLEPRNG_H
