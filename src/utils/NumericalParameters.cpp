#include "NumericalParameters.h"

namespace pmf {



NumericalParameters::NumericalParameters(NumericalParameters::REAL epsilon)
{
    //ctor
    _AxisEpsilon = epsilon;
    _DistanceEpsilon = epsilon * NumericalParameters::REAL( sqrt(2.) );

    printf("[  ctor  ] : epsDist = %.9lf  ( %.9lf )\n", _DistanceEpsilon, _AxisEpsilon);
}

NumericalParameters::~NumericalParameters()
{
    //dtor
}

NumericalParameters::NumericalParameters (const NumericalParameters & other)
{
    //copy ctor
    _AxisEpsilon = other.GetAxisEpsilon();
    _DistanceEpsilon = other.GetDistEpsilon();

    printf("[ cpctor ] : epsDist = %.9lf  ( %.9lf )\n", _DistanceEpsilon, _AxisEpsilon);
}

NumericalParameters & NumericalParameters::operator = (const NumericalParameters & rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    _AxisEpsilon = rhs.GetAxisEpsilon();
    _DistanceEpsilon = rhs.GetDistEpsilon();
    return *this;
}



} // namespace pmf
