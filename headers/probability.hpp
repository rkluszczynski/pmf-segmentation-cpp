#ifndef __PROBABILITY_HPP__
#define __PROBABILITY_HPP__

#include <cstdlib>
#include <cmath>


template <class REAL> void determineBirthAngles(REAL *g, REAL *d)
{
    REAL katL, katB, kat, tmp;
    do {
        katL = M_PI * ((REAL)rand() / (REAL)RAND_MAX) - ((REAL)M_PI * (REAL)0.5);
        katB = M_PI * ((REAL)rand() / (REAL)RAND_MAX) - ((REAL)M_PI * (REAL)0.5);
        if(katL > katB) { tmp = katL;  katL = katB;  katB = tmp; }
        // Teraz zawsze mamy katB <= katL
        kat = katB - katL;
    }
    while( (((REAL)rand()) / (REAL)RAND_MAX) > (REAL)sin(kat) );
    *g = katB;
    *d = katL;
}


template <class REAL> void determineUpdateAngle(REAL *fi)
{
    REAL zm = ((REAL)rand()) / ((REAL)RAND_MAX);
    *fi = (REAL)acos(zm) * ((((REAL)rand()/(REAL)RAND_MAX) > 0.5)?1:(-1));
}


template <class REAL> REAL Exp (REAL lambda)
{
    REAL u = (((REAL)rand()) / (REAL)RAND_MAX);
    if(u < ((REAL)0.00001)) u = (REAL)0.00001;
    if(u > ((REAL)0.99999)) u = (REAL)0.99999;
    return((REAL) (- log(u) / lambda));
}


template <class REAL> REAL Uniform (REAL a, REAL b)
{
	if (b < a) {
		REAL tmp = b;
		b = a;
		a = tmp;
	}
	REAL u = (((REAL)rand()) / (REAL)RAND_MAX);
	return((b-a) * u  +  a);
}

#endif /* __PROBABILITY_HPP__ */
