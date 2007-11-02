#ifndef __INTERSECTIONSLIST_HPP__
#define __INTERSECTIONSLIST_HPP__

#include <stdio.h>
#include <iostream>

#include "templatelist.hpp"
#include "crosselement.hpp"

#define null 0


template <class REAL>
class IntersectionsList : public TemplateList<CrosspointElement<REAL> >
{
    public:
        void push_back(pmf_point<REAL> *, long, long);
        void push_in_order(pmf_point<REAL> *, long, long);

    private:
        int i;

};


/**
 * Wstawia element w kolejnosci rosnacej.
 **/
template <class REAL>
void IntersectionsList<REAL>::push_in_order (pmf_point<REAL> * pt, long p1, long p2)
{
    CrosspointElement<REAL> * cpt = new CrosspointElement<REAL> (pt, p1, p2);
    TemplateList<CrosspointElement<REAL> >::push_in_order (cpt);
}


/**
 * Wstawia element na koniec listy.
 **/
template <class REAL>
void IntersectionsList<REAL>::push_back (pmf_point<REAL> * pt, long p1, long p2)
{
    CrosspointElement<REAL> * cpt = new CrosspointElement<REAL> (pt, p1, p2);
    TemplateList<CrosspointElement<REAL> >::push_back (cpt);
}


#undef null

#endif /* __INTERSECTIONSLIST_HPP__ */
