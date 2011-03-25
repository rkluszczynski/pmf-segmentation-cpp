#include "DoublePRNG.h"

#include "dSFMT/dSFMT.c"


namespace pmf {

//DoublePRNG * PRNG;


DoublePRNG::DoublePRNG()  { OnInit(); }
DoublePRNG::DoublePRNG(time_t seed) : _seed(seed)  { OnInit(); }

DoublePRNG::~DoublePRNG()
{
    //dtor
}



void
DoublePRNG::OnInit ()
{
    dsfmt_init_gen_rand(&dsfmt, _seed);
    srand(_seed);
    _norm = 1. / ((double)RAND_MAX);
}


void
DoublePRNG::DetermineBirthAngles (double & up, double & down)
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
DoublePRNG::DetermineUpdateAngle ()
{
    double u = GetUniform();
    return acos(u) * ((GetUniform() > .5) ? 1 : (-1));
}


double
DoublePRNG::GetExp (double lambda)
{
    double u = GetUniform();
    if (u < .00001)  u = .00001;
    if (u > .99999)  u = .99999;
    return -log(u) / lambda;
}


double
DoublePRNG::GetUniform (double a, double b)
{
    if (b < a)  std::swap(a, b);
    return (b-a) * GetUniform() + a;
}

double
DoublePRNG::GetUniform ()
{
    return dsfmt_genrand_open_open(&dsfmt);;
    return double(rand()) * _norm;
}


DoublePRNG::DoublePRNG(const DoublePRNG& other)
{
    //copy ctor
    _seed = other._seed;
    _norm = other._norm;
    //dsfmt = other.dsfmt;
    OnInit();
}

DoublePRNG& DoublePRNG::operator=(const DoublePRNG& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    _seed = rhs._seed;
    _norm = rhs._norm;
    //dsfmt = rhs.dsfmt;
    OnInit();
    return *this;
}


} // namespace pmf
