#ifndef ROTATE_HPP_INCLUDED
#define ROTATE_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
void
PMF<REAL> :: RotatePoints (REAL sinL = 0.0, REAL cosL = 1.0)
{
    /*
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
    //*/
    return;
}
#undef X_ROTATED
#undef Y_ROTATED


#endif // ROTATE_HPP_INCLUDED
