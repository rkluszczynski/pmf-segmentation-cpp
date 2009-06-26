

template <class REAL>
PMF<REAL> :: PMF (REAL fWidth, REAL fHeight)
{ //ctor
    seed = 0;
    cf   = new Configuration<REAL> (fWidth, fHeight);
}


template <class REAL>
PMF<REAL> :: ~PMF ()
{ //dtor
    delete cf;
}


#include "generate.hpp"

