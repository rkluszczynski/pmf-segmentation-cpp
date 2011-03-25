#include "PrallelDoublePRNG.h"

PrallelDoublePRNG::PrallelDoublePRNG(unsigned _threadsCount) : threadsCount(_threadsCount)
{
    //ctor
    states = new dsfmt_t[threadsCount];
    dnums = new double *[threadsCount];
    inums = new int *[threadsCount];
    for (unsigned i = 0; i < threadsCount; ++i)
    {
        dnums[i] = new double[BUFFOR];
        inums[i] = new int[BUFFOR];
    }
}

PrallelDoublePRNG::~PrallelDoublePRNG()
{
    //dtor
    for (unsigned i = 0; i < threadsCount; ++i)
    {
        delete[] dnums[i];
        delete[] inums[i];
    }
    delete[] inums;
    delete[] dnums;
    delete[] states;
}
