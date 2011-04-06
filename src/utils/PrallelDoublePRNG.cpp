#include "PrallelDoublePRNG.h"

#include <cmath>
#include <utility>

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
    dpos[0] = new int[threadsCount];
    dpos[1] = new int[threadsCount];
    omp_init_lock (&lock_);
    //for (unsigned i = 0; i < threadsCount; ++i)  { dpos[0][i] = BUFFOR -1; dpos[1][i] = 0; }
    for (unsigned i = 0; i < threadsCount; ++i)  dsfmt_init_gen_rand(&states[i], i);
    for (unsigned i = 0; i < threadsCount; ++i)  GenerateRandomNumbers(i, &states[i]);
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
    delete[] dpos[0];
    delete[] dpos[1];
}


void
PrallelDoublePRNG::DetermineBirthAngles (unsigned thid, double & up, double & down)
{
    double angleL, angleB, angle;
    do {
        angleL = M_PI * ( GetUniform(thid) - .5 );
        angleB = M_PI * ( GetUniform(thid) - .5 );
        if (angleL > angleB)  std::swap(angleL, angleB);
    #ifdef CHECK_ASSERTIONS
        assert(angleL <= angleB);
    #endif
        angle = angleB - angleL;
    }
    while( GetUniform(thid) > sin(angle) );
    up = angleB;
    down = angleL;
}


double
PrallelDoublePRNG::DetermineUpdateAngle (unsigned thid)
{
    double u = GetUniform(thid);
    return acos(u) * ((GetUniform(thid) > .5) ? 1 : (-1));
}


double
PrallelDoublePRNG::GetExp (unsigned thid, double lambda)
{
    double u = GetUniform(thid);
    if (u < .00001)  u = .00001;
    if (u > .99999)  u = .99999;
    return -log(u) / lambda;
}


double
PrallelDoublePRNG::GetUniform (unsigned thid, double a, double b)
{
    if (b < a)  std::swap(a, b);
    return (b-a) * GetUniform(thid) + a;
}


double
PrallelDoublePRNG::GetUniform (unsigned thid)
{
    double rnd = dnums[thid][dpos[0][thid]];
    ++dpos[0][thid];
    if (dpos[0][thid] == dpos[1][thid])
    {
        GenerateRandomNumbers(thid, &states[thid]);
    }
    if (dpos[0][thid] == BUFFOR) dpos[0][thid] = 0;
    return rnd;
}


void
PrallelDoublePRNG::GenerateRandomNumbers(unsigned thid, dsfmt_t *dsfmt)
{
    #pragma omp critical
    {
        //omp_set_lock (&lock_);
        printf("-{%u}-> in\n", thid);
        //for (int i = 0; i < BUFFOR; ++i) dnums[thid][i] = dsfmt_genrand_open_open (dsfmt);
        dsfmt_fill_array_open_open (&states[thid], &dnums[thid][0], BUFFOR);
        printf("<-{%u}- out\n", thid);
        //omp_unset_lock (&lock_);
    }
    dpos[0][thid] = 0;
    dpos[1][thid] = BUFFOR;
}


void
PrallelDoublePRNG::GenerateRadoms()
{
    const int GT = 2000;

    for (unsigned i = 0; i < threadsCount; ++i)
    {
        if (dpos[0][i] > dpos[1][i])
        {
            int size = dpos[0][i] - dpos[1][i];
            if (size > GT)
            {
                for (int k = dpos[1][i]; k < dpos[0][i]; ++k)
                    dnums[i][k] = dsfmt_genrand_open_open (&states[i]);
                //dsfmt_fill_array_open_open (&states[i], &dnums[i][dpos[1][i]], size);
                dpos[1][i] += size;
            }
        }
        else if (dpos[0][i] < dpos[1][i]) {
            int size = BUFFOR - dpos[1][i];
            if (size > GT)
            {
                for (int k = dpos[1][i]; k < BUFFOR; ++k)
                    dnums[i][k] = dsfmt_genrand_open_open (&states[i]);
                //dsfmt_fill_array_open_open (&states[i], &dnums[i][dpos[1][i]], size);
            }
            else {
                for (int k = dpos[1][i]; k < BUFFOR; ++k)
                    dnums[i][k] = dsfmt_genrand_open_open (&states[i]);
            }

            size = dpos[0][i];
            //if (size > GT)
            {
                for (int k = 0; k < dpos[0][i]; ++k)
                    dnums[i][k] = dsfmt_genrand_open_open (&states[i]);
                //dsfmt_fill_array_open_open (&states[i], &dnums[i][0], size);
            }

            dpos[1][i] = dpos[0][i];
        }
    }

}

