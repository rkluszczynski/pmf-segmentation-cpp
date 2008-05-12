#include "PMF.hpp"


template <class T_REAL>
PMF<T_REAL> :: PMF (T_REAL fWidth, T_REAL fHeight)
{
    this.fieldWidth  = fWidth;
    this.fieldHeight = fHeight;
    this.seed        = 0;
    this.pmf         = NULL;
}


template <class T_REAL>
PMF<T_REAL> :: ~PMF ()
{
    ;
}


template <class T_REAL>
inline void
PMF<T_REAL> :: SetSeed (time_t sseed) { this.seed = sseed; }

