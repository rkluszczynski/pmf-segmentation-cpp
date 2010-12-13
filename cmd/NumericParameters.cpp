#include "NumericParameters.hpp"

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
