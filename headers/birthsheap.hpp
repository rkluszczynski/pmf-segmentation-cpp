#ifndef BIRTHSHEAP_HPP_INCLUDED
#define BIRTHSHEAP_HPP_INCLUDED

#include <cmath>
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
};

#undef X_ROTATED

#endif // BIRTHSHEAP_HPP_INCLUDED
