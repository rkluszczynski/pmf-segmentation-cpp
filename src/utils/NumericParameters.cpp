#include "NumericParameters.h"

namespace pmf {



NumericParameters::NumericParameters(NumericParameters::REAL epsilon)
{
    //ctor
    m_AxisEpsilon = epsilon;
    m_DistanceEpsilon = epsilon * NumericParameters::REAL( sqrt(2.) );

    printf(" epsDist = %.9lf  ( %.9lf )\n", m_DistanceEpsilon, m_AxisEpsilon);
}

NumericParameters::~NumericParameters()
{
    //dtor
}


/*
NumericParameters::NumericParameters(const NumericParameters& other)
{
    //copy ctor
}

NumericParameters& NumericParameters::operator=(const NumericParameters& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/// */



} // namespace pmf
