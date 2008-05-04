#ifndef BIRTHSHEAP_HPP_INCLUDED
#define BIRTHSHEAP_HPP_INCLUDED

#include <cmath>
#include <algorithm>
#include "abstractheap.hpp"

#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))

template <class T_REAL>
class BirthsHeap : public AbstractHeap<pmf_point<T_REAL> *>
{
    private :
        double sinL, cosL;

        virtual inline bool __less_then (pmf_point<T_REAL> * pt1, pmf_point<T_REAL> * pt2)
        {
            return X_ROTATED(pt1->x, pt1->y, sinL, cosL) < X_ROTATED(pt2->x, pt2->y, sinL, cosL);
        };

    public :
        BirthsHeap() : sinL(0.0), cosL(1.0) {};
        BirthsHeap(double ssinL, double ccosL) : sinL(ssinL), cosL(ccosL) {};

        bool remove_point_with_id(long);
        pmf_point<T_REAL> * get_point_with_id(long);
};


template <class T_REAL>
bool
BirthsHeap<T_REAL>::remove_point_with_id (long ptId)
{
    for (int i = 0; i < AbstractHeap<pmf_point<T_REAL> *>::size(); i++)
    {
        pmf_point<T_REAL> * pt = AbstractHeap<pmf_point<T_REAL> *>::get(i);
        if (pt->id == ptId) {
            //if (blocksLists)  blocksLists->pop(iter->data);
            std::swap ((*AbstractHeap<pmf_point<T_REAL> *>::data)[i], (*AbstractHeap<pmf_point<T_REAL> *>::data)[AbstractHeap<pmf_point<T_REAL> *>::size()-1]);
            AbstractHeap<pmf_point<T_REAL> *>::data->pop_back();
            AbstractHeap<pmf_point<T_REAL> *>::min_heapify (i);
            return true;
        }
    }
    return false;
}


template <class T_REAL>
pmf_point<T_REAL> *
BirthsHeap<T_REAL>::get_point_with_id (long ptId)
{
    for (int i = 0; i < AbstractHeap<pmf_point<T_REAL> *>::size(); i++)
    {
        pmf_point<T_REAL> * pt = AbstractHeap<pmf_point<T_REAL> *>::get(i);
        if (pt->id == ptId) { return pt; }
    }
    return NULL;
}


#undef X_ROTATED

#endif // BIRTHSHEAP_HPP_INCLUDED
