#ifndef __INTERSECTIONSLIST_HPP__
#define __INTERSECTIONSLIST_HPP__

#include <stdio.h>
#include <iostream>

#include "templatelist.hpp"
#include "crosselement.hpp"
#include "blockslists.hpp"

#define null 0


template <class T_REAL>
class IntersectionsList : public TemplateList<CrosspointElement<T_REAL> >
{
    public:
        void push_back (pmf_point<T_REAL> *, long, long);
        void push_in_order (pmf_point<T_REAL> *, long, long);
        bool remove_intersection_with_id (long, BlocksLists<T_REAL> *);
        bool remove_intersection_with_one_id_of (long, long, BlocksLists<T_REAL> *);
};


template <class T_REAL>
bool IntersectionsList<T_REAL>::remove_intersection_with_id (long ptId, BlocksLists<T_REAL> * blocksLists)
{
    using std::cout;
    using std::endl;
    Element<CrosspointElement<T_REAL> > * iter = TemplateList<CrosspointElement<T_REAL> >::head;
    Element<CrosspointElement<T_REAL> > * pop = iter;//, * tmp = iter;
    bool deleted = false;
    while (iter) {
        if (iter->data->p1 == ptId  ||  iter->data->p2 == ptId)
        {
            // Added 16.01.2008
            if (blocksLists) {
                blocksLists->pop(iter->data->pt);
#ifdef DEBUG
                cout << blocksLists << endl;
#endif
            }
            // -- RK
            delete iter->data->pt;
            delete iter->data;
            if (iter == TemplateList<CrosspointElement<T_REAL> >::head)
            {
                TemplateList<CrosspointElement<T_REAL> >::head = iter->next;
                delete iter;
                pop = TemplateList<CrosspointElement<T_REAL> >::head;
                iter = TemplateList<CrosspointElement<T_REAL> >::head;
            }
            else {
                pop->next = iter->next;
                delete iter;
                iter = pop->next;
            }
            deleted = true;
        }
        else {
            pop = iter;
            iter = iter->next;
        }
    }
    return deleted;
}


template <class T_REAL>
bool IntersectionsList<T_REAL>::remove_intersection_with_one_id_of (long ptId1, long ptId2, BlocksLists<T_REAL> * blocksLists)
{
    using std::cout;
    using std::endl;
    Element<CrosspointElement<T_REAL> > * iter = TemplateList<CrosspointElement<T_REAL> >::head;
    Element<CrosspointElement<T_REAL> > * pop = iter;//, * tmp = iter;
    bool deleted = false;
    while (iter) {
        if ( !(iter->data->p1 == ptId1  &&  iter->data->p2 == ptId2)  &&
             !(iter->data->p1 == ptId2  &&  iter->data->p2 == ptId1)  &&
             ( iter->data->p1 == ptId1  ||  iter->data->p2 == ptId1  ||
               iter->data->p1 == ptId2  ||  iter->data->p2 == ptId2 ) )
        {
            // Added 16.01.2008
            if (blocksLists) {
                blocksLists->pop(iter->data->pt);
#ifdef DEBUG
                cout << blocksLists << endl;
#endif
            }
            // -- RK
            delete iter->data->pt;
            delete iter->data;
            if (iter == TemplateList<CrosspointElement<T_REAL> >::head)
            {
                TemplateList<CrosspointElement<T_REAL> >::head = iter->next;
                delete iter;
                pop = TemplateList<CrosspointElement<T_REAL> >::head;
                iter = TemplateList<CrosspointElement<T_REAL> >::head;
            }
            else {
                pop->next = iter->next;
                delete iter;
                iter = pop->next;
            }
            deleted = true;
        }
        else {
            pop = iter;
            iter = iter->next;
        }
    }
    return deleted;
}


/**
 * Wstawia element w kolejnosci rosnacej.
 **/
template <class T_REAL>
void IntersectionsList<T_REAL>::push_in_order (pmf_point<T_REAL> * pt, long p1, long p2)
{
    CrosspointElement<T_REAL> * cpt = new CrosspointElement<T_REAL> (pt, p1, p2);
    TemplateList<CrosspointElement<T_REAL> >::push_in_order (cpt);
}


/**
 * Wstawia element na koniec listy.
 **/
template <class T_REAL>
void IntersectionsList<T_REAL>::push_back (pmf_point<T_REAL> * pt, long p1, long p2)
{
    CrosspointElement<T_REAL> * cpt = new CrosspointElement<T_REAL> (pt, p1, p2);
    TemplateList<CrosspointElement<T_REAL> >::push_back (cpt);
}


#undef null

#endif /* __INTERSECTIONSLIST_HPP__ */
