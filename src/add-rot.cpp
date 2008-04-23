#include "configurationlist.hpp"
#include "birthslist.hpp"
#include "intersectionslist.hpp"
#include "probability.hpp"
#include "accesslists.hpp"
#include "modifylists.hpp"

#include "birthsheap.hpp"

#define LOG 1
#define REAL double

#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_LE(PP1,PP2,SSIN,CCOS) (! PT_LT(PP2,PP1,SSIN,CCOS))
void pmf_add_rotated_point (
            ConfigurationList<REAL> * PMF,
            ConfigurationList<REAL> * newPMF,
            REAL xx,
            REAL yy
        )
{
#if LOG
    FILE * flog = freopen("output/log-add-rot.txt", "w", stdout);
    int iterationCounter = 0;
#endif
    REAL fieldWidth  = PMF->get_field_width();
    REAL fieldHeight = PMF->get_field_height();
    PMF->set_points_ids();

    long oldSize = PMF->get_size() + 1;
    long ptId = oldSize;
    REAL sinL = 0.0;
    REAL cosL = 1.0;
    assert(sinL*sinL + cosL*cosL == 1.0);
    REAL rotxx = X_ROTATED(xx, yy, sinL, cosL);
    cerr << "[  ADD ] : oldSize = " << ptId << "   {" << xx << ";" << yy << "}  : " << rotxx << endl;

    BirthsHeap<REAL> * bHeap = new BirthsHeap<REAL> (sinL, cosL);
    pmf_point<REAL> * pt;
    while (! PMF->empty())
    {
        pt = PMF->front();
        PMF->pop_front();
        bHeap->insert(pt);
    }

    pt = new pmf_point<REAL>(xx, yy, NULL, NULL, 0.0, 0.0, ++ptId, PT_BIRTH_NORMAL);
    while (! bHeap->empty() && PT_LT(bHeap->top(), pt, sinL, cosL))
    {
#if LOG
        cout << *bHeap->top() << "_" << X_ROTATED(bHeap->top()->x, bHeap->top()->y, sinL, cosL) << endl;
#endif
        //newPMF->push_in_order(bHeap->extract_min());
        newPMF->push_back(bHeap->extract_min());
    }

    return;
}
#undef X_ROTATED
#undef LOG

#undef REAL
