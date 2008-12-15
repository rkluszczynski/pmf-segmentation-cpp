#ifndef ROTATE_HPP_INCLUDED
#define ROTATE_HPP_INCLUDED

#include "point.hpp"


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class T_REAL>
void
PMF<T_REAL> :: RotatePointTypes (T_REAL sinL = 0.0, T_REAL cosL = 1.0)
{
    Element<pmf_point<T_REAL> > * elem = pmfConf->getHead();
#if PRINT_INFO
    std::cerr << " sinL = " << sinL << std::endl;
    std::cerr << " cosL = " << cosL << std::endl;
#endif
    while (elem) {
        pmf_point<T_REAL> * pt = elem->data;

        pmf_point<T_REAL> * n1 = pt->n1;
        pmf_point<T_REAL> * n2 = pt->n2;

        REAL rotX = X_ROTATED(pt->x, pt->y, sinL, cosL);
        switch (pt->type) {
            case PT_BIRTH_NORMAL :
            case PT_INTERSECTION :
            case PT_UPDATE :
#if CHECK_ASSERTIONS
                assert(n1 != NULL);
                assert(n2 != NULL);
#endif
                {
                    T_REAL rotX1 = X_ROTATED(n1->x, n1->y, sinL, cosL);
                    T_REAL rotX2 = X_ROTATED(n2->x, n2->y, sinL, cosL);

                    if (rotX < rotX1) {
                        if (rotX < rotX2)  { pt->type = PT_BIRTH_NORMAL; }
                        else  {
                            pt->type = PT_UPDATE;
                            std::swap(pt->n1, pt->n2);
                            std::swap(pt->l1, pt->l2);
                        }
                    }
                    else {
                        if (rotX < rotX2)  {
                            pt->type = PT_UPDATE;
                            //std::swap(pt->n1, pt->n2);
                        }
                        else  { pt->type = PT_INTERSECTION; }
                    }
                }
                break;;
            case PT_BIRTH_LEFT :
            case PT_BIRTH_DOWN :
            case PT_BIRTH_UP :
            case PT_BORDER :
#if CHECK_ASSERTIONS
                assert(n1 != NULL);
                assert(n2 == NULL);
#endif
                {
                    T_REAL rotX0 = X_ROTATED(n1->x, n1->y, sinL, cosL);

                    if (rotX < rotX0)  pt->type = PT_BIRTH_LEFT;
                    else  pt->type = PT_BORDER;
                }
                break;;
#if CHECK_ASSERTIONS
            default :
                assert(false);
#endif
        }
        elem = elem->next;
    }
    return;
}
#undef X_ROTATED
#undef Y_ROTATED


template <class T_REAL>
void
PMF<T_REAL> :: DetermineTypesFromLeftToRight ()
{
    Element<pmf_point<T_REAL> > * elem = pmfConf->getHead();
    while (elem) {
        pmf_point<T_REAL> * pt = elem->data;
        pmf_point<T_REAL> * n1 = pt->n1;
        pmf_point<T_REAL> * n2 = pt->n2;

        if (n1 && n2)
        {
            if (pt->x < pt->n1->x) {
                if (pt->x < pt->n2->x)  { pt->type = PT_BIRTH_NORMAL; }
                else {
                    pt->type = PT_UPDATE;
                    std::swap(pt->n1, pt->n2);
                    std::swap(pt->l1, pt->l2);
                }
            }
            else {
                if (pt->x < pt->n2->x) pt->type = PT_UPDATE;
                else pt->type = PT_INTERSECTION;
            }
        }
        else if (n1)
        {
#if CHECK_ASSERTIONS
                assert(n2 == NULL);
#endif
            if (pt->x < pt->n1->x) {
                if (pt->x == 0.0) pt->type = PT_BIRTH_LEFT;
                else if (pt->y == 0.0) pt->type = PT_BIRTH_UP;
                else pt->type = PT_BIRTH_DOWN;
            }
            else pt->type = PT_BORDER;
        }
        else {
#if CHECK_ASSERTIONS
                assert(n1 == NULL  &&  n2 == NULL  &&  false);
#endif
            ;
        }
        elem = elem->next;
    }
    return;
}


#endif // ROTATE_HPP_INCLUDED
