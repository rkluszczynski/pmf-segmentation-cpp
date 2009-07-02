#ifndef PROBABILITY_HPP_INCLUDED
#define PROBABILITY_HPP_INCLUDED

#include "../headers/macros.hpp"

namespace pmf
{
    namespace Probability
    {

        template <class REAL>
        void
        DetermineBirthAngles(REAL & up, REAL & down)
        {
            REAL angleL, angleB, angle;
            do {
                angleL = M_PI * ((REAL)rand() / (REAL)RAND_MAX) - ((REAL)M_PI * (REAL)0.5);
                angleB = M_PI * ((REAL)rand() / (REAL)RAND_MAX) - ((REAL)M_PI * (REAL)0.5);
                if(angleL > angleB) std::swap(angleL, angleB);
        #ifdef CHECK_ASSERTIONS
                assert(angleL <= angleB);
        #endif
                angle = angleB - angleL;
            }
            while( (((REAL)rand()) / (REAL)RAND_MAX) > (REAL)sin(angle) );
            up = angleB;
            down = angleL;
        }


        template <class REAL>
        void
        DetermineUpdateAngle(REAL & fi)
        {
            REAL zm = ((REAL)rand()) / ((REAL)RAND_MAX);
            fi = (REAL)acos(zm) * ((((REAL)rand()/(REAL)RAND_MAX) > 0.5)?1:(-1));
        }


        template <class REAL>
        REAL
        Exp (REAL lambda)
        {
            REAL u = (((REAL)rand()) / (REAL)RAND_MAX);
            if(u < ((REAL)0.00001)) u = (REAL)0.00001;
            if(u > ((REAL)0.99999)) u = (REAL)0.99999;
            return((REAL) (- log(u) / lambda));
        }


        template <class REAL>
        REAL
        Uniform (REAL a, REAL b)
        {
            if (b < a)  swap(a, b);
            REAL u = (((REAL)rand()) / (REAL)RAND_MAX);
            return((b-a) * u  +  a);
        }

    }
}

#endif // PROBABILITY_HPP_INCLUDED
