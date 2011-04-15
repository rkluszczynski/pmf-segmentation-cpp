#ifndef NUMERICALPARAMETERS_H
#define NUMERICALPARAMETERS_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <memory>

namespace pmf
{

    class NumericalParameters
    {
        public:
            typedef double REAL;

            NumericalParameters(REAL);
            virtual ~NumericalParameters();

            NumericalParameters(const NumericalParameters& other);
            NumericalParameters& operator=(const NumericalParameters& other);

            inline
            REAL GetAxisEpsilon() const { return _AxisEpsilon; }
            inline
            REAL GetDistEpsilon() const { return _DistanceEpsilon; }

        protected:

        private:
            REAL _AxisEpsilon;
            REAL _DistanceEpsilon;
    };


} // namespace pmf

#endif // NUMERICALPARAMETERS_H
