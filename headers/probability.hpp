#ifndef __PROBABILITY_HPP__
#define __PROBABILITY_HPP__

#include <cstdlib>
#include <cmath>
#include <algorithm>

template <class T_REAL> void determineBirthAngles(T_REAL & g, T_REAL & d)
{
    T_REAL katL, katB, kat;
    do {
        katL = M_PI * ((T_REAL)rand() / (T_REAL)RAND_MAX) - ((T_REAL)M_PI * (T_REAL)0.5);
        katB = M_PI * ((T_REAL)rand() / (T_REAL)RAND_MAX) - ((T_REAL)M_PI * (T_REAL)0.5);
        if(katL > katB) std::swap(katL, katB);
        // Teraz zawsze mamy katB <= katL
#ifdef CHECK_ASSERTIONS
        assert(katB <= katL);
#endif
        kat = katB - katL;
    }
    while( (((T_REAL)rand()) / (T_REAL)RAND_MAX) > (T_REAL)sin(kat) );
    g = katB;
    d = katL;
}


template <class T_REAL> void determineUpdateAngle(T_REAL & fi)
{
    T_REAL zm = ((T_REAL)rand()) / ((T_REAL)RAND_MAX);
    fi = (T_REAL)acos(zm) * ((((T_REAL)rand()/(T_REAL)RAND_MAX) > 0.5)?1:(-1));
}


template <class T_REAL> T_REAL Exp (T_REAL lambda)
{
    T_REAL u = (((T_REAL)rand()) / (T_REAL)RAND_MAX);
    if(u < ((T_REAL)0.00001)) u = (T_REAL)0.00001;
    if(u > ((T_REAL)0.99999)) u = (T_REAL)0.99999;
    return((T_REAL) (- log(u) / lambda));
}


template <class T_REAL> T_REAL Uniform (T_REAL a, T_REAL b)
{
	if (b < a) { swap(a, b); }
	T_REAL u = (((T_REAL)rand()) / (T_REAL)RAND_MAX);
	return((b-a) * u  +  a);
}

#endif /* __PROBABILITY_HPP__ */
