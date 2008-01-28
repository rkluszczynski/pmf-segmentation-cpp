#ifndef __BIRTHSLIST_HPP__
#define __BIRTHSLIST_HPP__

#include <stdio.h>
#include <iostream>

#include "point.hpp"
#include "templatelist.hpp"
#include "blockslists.hpp"

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
        void push_in_order (pmf_point<T_REAL> *, BlocksLists<T_REAL> *);
        bool remove_point_with_id (long, BlocksLists<T_REAL> *);
};


template <class T_REAL>
void BirthsList<T_REAL>::push_in_order (pmf_point<T_REAL> * pt, BlocksLists<T_REAL> * blocks)
{
    if (blocks) {
        pt->block = blocks->determine_point_block(pt);
        blocks->push(pt);
    }
    TemplateList<pmf_point<T_REAL> >::push_in_order (pt);
}


template <class T_REAL>
bool BirthsList<T_REAL>::remove_point_with_id (long ptId, BlocksLists<T_REAL> * blocksLists)
{
    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
    Element<pmf_point<T_REAL> > * pop = iter, * tmp = iter;
    while (iter) {
        if (iter->data->id == ptId) {
            // Added 16.01.2008
            if (blocksLists)  blocksLists->pop(iter->data);
            // -- RK
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
