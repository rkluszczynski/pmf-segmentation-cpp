#ifndef ACCESSLISTS_HPP_INCLUDED
#define ACCESSLISTS_HPP_INCLUDED

#include "geometry.hpp"

bool pmf_store_points ( PMF_POINT * newPt,
                        BirthsList<REAL> * bList,
                        IntersectionsList<REAL> * iList,
                        PMF_POINT * pt,
                        long & id,
                        REAL fieldHeight,
                        REAL fieldWidth
                    )
{
    if (newPt->type > 4) {
        /* Check if coordinates are not outside the field. */
        if( pt->type != PT_BIRTH_UP  &&
            cross3<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, fieldWidth, 0.0f) != 0 )
        {
            REAL cx, cy;
            crosspoint2<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, fieldWidth, 0.0f, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }
        if( pt->type != PT_BIRTH_DOWN &&
            cross3<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, fieldHeight, fieldWidth, fieldHeight) != 0 )
        {
            REAL cx, cy;
            crosspoint2<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, fieldHeight, fieldWidth, fieldHeight, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }
        if( cross3<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, fieldWidth, 0.0f, fieldWidth, fieldHeight) != 0 ) {
            REAL cx, cy;
            crosspoint2<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, fieldWidth, 0.0f, fieldWidth, fieldHeight, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }

        /* Calculate intersection points with other segments */
        Element<pmf_point<REAL> > * pEl = bList->getHead();
        while (pEl) {

            if (newPt != pEl->data->n1  &&  pEl->data->n1 != NULL &&
                cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y) == 1 )
            {
                REAL xx, yy;
                crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y, xx, yy);
                PMF_POINT * newpt2 = new PMF_POINT(xx, yy, pt, pEl->data->n1, 0.0, 0.0, ++id, PT_INTERSECTION);
                iList->push_in_order (newpt2, newPt->id, pEl->data->id);
            }
            pEl = pEl->next;
        }
    }
    bList->push_in_order(newPt);

    return true;
}



PMF_POINT * pmf_do_get (
                        BirthsList<REAL> * bList,
                        IntersectionsList<REAL> * iList,
                        long & id1,
                        long & id2
                    )
{
    if (bList->empty() && iList->empty()) { return NULL; }
    else if (!bList->empty() && iList->empty()) {
        PMF_POINT * pt = bList->front();
        bList->pop_front();
        return pt;
    }
    else if (bList->empty() && !iList->empty()) {
        PMF_POINT * pt = (iList->front())->pt;
        id1 = iList->front()->p1;
        id2 = iList->front()->p2;
        iList->pop_front();
        return pt;
    }
    else {
        PMF_POINT * bpt = bList->front();
        PMF_POINT * ipt = iList->front()->pt;
        if (bpt->x < ipt->x) {
            bList->pop_front();
            return bpt;
        }
        else {
            id1 = iList->front()->p1;
            id2 = iList->front()->p2;
            iList->pop_front();
            return ipt;
        }
    }
}


PMF_POINT * pmf_do_top (
                        BirthsList<REAL> * bList,
                        IntersectionsList<REAL> * iList,
                        long & id1,
                        long & id2
                    )
{
    if (bList->empty() && iList->empty()) { return NULL; }
    else if (!bList->empty() && iList->empty()) { return bList->front(); }
    else if (bList->empty() && !iList->empty())
    {
        id1 = iList->front()->p1;
        id2 = iList->front()->p2;
        return iList->front()->pt;
    }
    else {
        if (bList->front()->x < iList->front()->pt->x) { return bList->front();}
        else {
            id1 = iList->front()->p1;
            id2 = iList->front()->p2;
            return iList->front()->pt;
        }
    }
}


#endif // ACCESSLISTS_HPP_INCLUDED
