#include "DoubleProbability.h"

#include "dSFMT.c"


DoubleProbability::DoubleProbability()  { OnInit(); }
DoubleProbability::DoubleProbability(time_t seed) : _seed(seed)  { OnInit(); }

DoubleProbability::~DoubleProbability()
{
    //dtor
}

void
DoubleProbability::OnInit ()
{
    dsfmt_init_gen_rand(&dsfmt, _seed);
    srand(_seed);
    _norm = 1. / ((double)RAND_MAX);
}


void
DoubleProbability::DetermineBirthAngles (double & up, double & down)
{
    double angleL, angleB, angle;
    do {
        angleL = M_PI * ( GetUniform() - .5 );
        angleB = M_PI * ( GetUniform() - .5 );
        if (angleL > angleB)  std::swap(angleL, angleB);
    #ifdef CHECK_ASSERTIONS
        assert(angleL <= angleB);
    #endif
        angle = angleB - angleL;
    }
    while( GetUniform() > sin(angle) );
    up = angleB;
    down = angleL;
}


double
DoubleProbability::DetermineUpdateAngle ()
{
    double u = GetUniform();
    return acos(u) * ((GetUniform() > .5) ? 1 : (-1));
}


double
DoubleProbability::GetExp (double lambda)
{
    double u = GetUniform();
    if (u < .00001)  u = .00001;
    if (u > .99999)  u = .99999;
    return -log(u) / lambda;
}


double
DoubleProbability::GetUniform (double a, double b)
{
    if (b < a)  std::swap(a, b);
    return (b-a) * GetUniform() + a;
}

