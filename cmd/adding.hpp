#ifndef ADDING_HPP_INCLUDED
#define ADDING_HPP_INCLUDED

template <class REAL>
void
PMF<REAL> :: AddBirthPoint (REAL xx, REAL yy, REAL alpha = 0.0)
{

    PrepareEvolution ();


    EvolveRestOfField ();


    return;
}

#endif // ADDING_HPP_INCLUDED
