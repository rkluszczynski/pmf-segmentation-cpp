
template <class T_REAL>
PMF<T_REAL> :: PMF (T_REAL fWidth, T_REAL fHeight)
{
    fieldWidth  = fWidth;
    fieldHeight = fHeight;
    seed        = 0;
    pmfConf     = NULL;
}


template <class T_REAL>
PMF<T_REAL> :: ~PMF ()
{
    ;
}


template <class T_REAL>
void
PMF<T_REAL> :: SetSeed (time_t sseed) { seed = sseed; }


#include "generate.hpp"

