
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
            REAL sinL,
            REAL cosL
            //REAL alpha
        )
{
    Element<pmf_point<REAL> > * elem = PMF->getHead();
    //REAL sinL = sin(alpha);
    //REAL cosL = cos(alpha);
    cerr << " sinL = " << sinL << endl;
    cerr << " cosL = " << cosL << endl;
    while (elem) {
        pmf_point<REAL> * pt = elem->data;

        pmf_point<REAL> * n1 = pt->n1;
        pmf_point<REAL> * n2 = pt->n2;

        REAL rotX = X_ROTATED(pt->x, pt->y, sinL, cosL);
        switch (pt->type) {
            case PT_BIRTH_NORMAL :
            case PT_INTERSECTION :
            case PT_UPDATE :
                assert(n1 != NULL);
                assert(n2 != NULL);
                REAL rotX1 = X_ROTATED(n1->x, n1->y, sinL, cosL);
                REAL rotX2 = X_ROTATED(n2->x, n2->y, sinL, cosL);

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
                REAL rotX0 = X_ROTATED(n1->x, n1->y, sinL, cosL);

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
#undef X_ROTATED
#undef Y_ROTATED

#undef PT_LT

#undef REAL
