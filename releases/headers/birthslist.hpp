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

template <class T_REAL>
class BirthsList : public TemplateList<pmf_point<T_REAL> >
{
    /*
        virtual bool lt(const pmf_point<REAL> * & a, const pmf_point<REAL> * & b)
        {
            std::cout << "B ";
            return(a->x < b->x);
        }
        */
    public :
        bool remove_point_with_id (long);
};


template <class T_REAL>
bool BirthsList<T_REAL>::remove_point_with_id (long ptId)
{
    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
    Element<pmf_point<T_REAL> > * pop = iter, * tmp = iter;
    while (iter) {
        if (iter->data->id == ptId) {
            delete iter->data;
            if (iter == TemplateList<pmf_point<T_REAL> >::head) {
                TemplateList<pmf_point<T_REAL> >::head = iter->next;
                delete(iter);
                pop = TemplateList<pmf_point<T_REAL> >::head;
                iter = TemplateList<pmf_point<T_REAL> >::head;
            }
            else {
                pop->next = iter->next;
                delete(iter);
                iter = pop->next;
            }
            return true;
        }
        else {
            pop = iter;
            iter = iter->next;
        }
    }
    return false;
}


#undef null

#endif /* __BIRTHSLIST_HPP__ */
