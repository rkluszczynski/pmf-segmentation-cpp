#ifndef NUMERICPARAMETERS_H
#define NUMERICPARAMETERS_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <memory>


namespace pmf
{

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
            //NumericParameters(const NumericParameters& other);
            //NumericParameters& operator=(const NumericParameters& other);

            static REAL GetEpsilonDistance() { return Instance()->m_AxisEpsilon; }
            static REAL         GetEpsilon() { return Instance()->m_DistanceEpsilon; }

        protected:
            NumericParameters(REAL);

        private:
            REAL m_AxisEpsilon;
            REAL m_DistanceEpsilon;
    };


} // namespace pmf

#endif // NUMERICPARAMETERS_H
