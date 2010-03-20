#include "GaussianRandomClass.hpp"

#include <cstdlib>
#include <cmath>

GaussianRandomClass::GaussianRandomClass() : haveNextNextGaussian(false)
{
    //ctor
    randMax_1 = 1. / double(RAND_MAX);
}

GaussianRandomClass::~GaussianRandomClass()
{
    //dtor
}

double
GaussianRandomClass::nextDouble()
{
    return double(rand()) * randMax_1;
}

double
GaussianRandomClass::nextGaussian()
{
    if (haveNextNextGaussian)
    {
        haveNextNextGaussian = false;
        return nextNextGaussian;
    }
    else
    {
        double v1, v2, s;
        do
        {
            v1 = 2. * nextDouble() - 1.;   // between -1.0 and 1.0
            v2 = 2. * nextDouble() - 1.;   // between -1.0 and 1.0
            s = v1 * v1 + v2 * v2;
        }
        while (s >= 1 || s == 0);

        double multiplier = sqrt(-2 * log(s)/s);
        nextNextGaussian = v2 * multiplier;
        haveNextNextGaussian = true;
        return v1 * multiplier;
    }
}
