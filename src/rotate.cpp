
#include "configurationlist.hpp"
#include "birthslist.hpp"
#include "intersectionslist.hpp"
#include "probability.hpp"
#include "accesslists.hpp"
#include "modifylists.hpp"

#define REAL double

#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))

void pmf_rotate_point_types (
            ConfigurationList<REAL> * PMF,
            REAL alpha
        )
{
    Element<pmf_point<REAL> > * elem = PMF->getHead();
    REAL sinL = sin(alpha);
    REAL cosL = cos(alpha);
    cerr << " sinL = " << sinL << endl;
    cerr << " cosL = " << cosL << endl;
    while (elem) {
        pmf_point<REAL> * pt = elem->data;

        pmf_point<REAL> * n1 = pt->n1;
        pmf_point<REAL> * n2 = pt->n2;

        REAL rotX = pt->x * cosL - pt->y * sinL;
        switch (pt->type) {
            case PT_BIRTH_NORMAL :
            case PT_INTERSECTION :
            case PT_UPDATE :
                assert(n1 != NULL);
                assert(n2 != NULL);
                REAL rotX1 = n1->x * cosL - n1->y * sinL;
                REAL rotX2 = n2->x * cosL - n2->y * sinL;

                if (rotX < rotX1) {
                    if (rotX < rotX2)  pt->type = PT_BIRTH_NORMAL;
                    else  pt->type = PT_UPDATE;
                }
                else {
                    if (rotX < rotX2)  pt->type = PT_UPDATE;
                    else  pt->type = PT_INTERSECTION;
                }
                break;;
            case PT_BIRTH_LEFT :
            case PT_BIRTH_DOWN :
            case PT_BIRTH_UP :
            case PT_BORDER :
                assert(n1 != NULL);
                assert(n2 == NULL);
                REAL rotX0 = n1->x * cosL - n1->y * sinL;

                if (rotX < rotX0)  pt->type = PT_BIRTH_LEFT;
                else  pt->type = PT_BORDER;
                break;;
            default :
                assert(false);
        }
        elem = elem->next;
    }
    return;
}
#undef PT_LT

#undef REAL
