#ifndef __BIRTHSLIST_HPP__
#define __BIRTHSLIST_HPP__

#include <stdio.h>
#include <iostream>

#include "point.hpp"
#include "templatelist.hpp"

#define null 0
/*
typedef struct pmf_point<double> * PMF_PT;


bool operator< (PMF_PT pt1, PMF_PT pt2)
{
    return(pt1->x < pt2->x);
}
*/

template <class REAL>
class BirthsList : public TemplateList<pmf_point<REAL> >
{
    /*
        virtual bool lt(const pmf_point<REAL> * & a, const pmf_point<REAL> * & b)
        {
            std::cout << "B ";
            return(a->x < b->x);
        }
        */
};

#undef null

#endif /* __BIRTHSLIST_HPP__ */
