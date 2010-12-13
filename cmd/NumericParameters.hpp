#ifndef NUMERICPARAMETERS_HPP
#define NUMERICPARAMETERS_HPP

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <memory>


class NumericParameters
{
    public:
        typedef std::auto_ptr<NumericParameters> InstancePointer;
        typedef double REAL;

        inline static InstancePointer & Instance()
        {
            static InstancePointer singleton(new NumericParameters( 0.0000001 ));
            return singleton;
        }
        virtual ~NumericParameters();

        static REAL GetEpsilonDistance() { return Instance()->m_AxisEpsilon; }
        static REAL         GetEpsilon() { return Instance()->m_DistanceEpsilon; }

    protected:
        NumericParameters(REAL);

    private:
        REAL m_AxisEpsilon;
        REAL m_DistanceEpsilon;
};

#endif // NUMERICPARAMETERS_HPP
