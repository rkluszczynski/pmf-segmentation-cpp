#ifndef PRALLELDOUBLEPRNG_H
#define PRALLELDOUBLEPRNG_H

#include "dSFMT/dSFMT.h"

#define BUFFOR 1000000

class PrallelDoublePRNG
{
    public:
        PrallelDoublePRNG(unsigned);
        virtual ~PrallelDoublePRNG();

    protected:

    private:
        dsfmt_t * states;
        double ** dnums;
        int ** inums;

        unsigned threadsCount;
};

#endif // PRALLELDOUBLEPRNG_H
