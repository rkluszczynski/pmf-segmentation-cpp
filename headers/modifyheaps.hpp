#ifndef MODIFYHEAPS_HPP_INCLUDED
#define MODIFYHEAPS_HPP_INCLUDED

#include "point.hpp"
#include "birthsheap.hpp"
#include "intersectionsheap.hpp"
#include "modifylists.hpp"

#define REAL double

#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_LE(PP1,PP2,SSIN,CCOS) (! PT_LT(PP2,PP1,SSIN,CCOS))
inline
bool
pmf_store_rotated_point_in_blocks (
                        pmf_point<REAL> * newPt,
                        BirthsHeap<REAL> * bHeap,
                        IntersectionsHeap<REAL> * iHeap,
                        pmf_point<REAL> * parentPt,
                        long & id,
                        REAL fieldHeight,
                        REAL fieldWidth,
                        BlocksLists<REAL> * blocks,
                        REAL sinL = 0.0,
                        REAL cosL = 1.0
                    )
{
    if (newPt->type > 4) {
        /* Check if coordinates are not outside the field. */
        if( parentPt->type != PT_BIRTH_UP  &&
            cross3<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, fieldWidth, 0.0f) != 0 )
        {
            REAL cx, cy;
            crosspoint2<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, fieldWidth, 0.0f, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }
        if( parentPt->type != PT_BIRTH_DOWN &&
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
        if( cross3<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, 0.0f, fieldHeight) != 0 ) {
            REAL cx, cy;
            crosspoint2<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, 0.0f, fieldHeight, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }

        /* Check if pointer to blocks is NULL */
        if (blocks)
        {
            assert(false);
        }
        else {
            for (int i = 0; i < bHeap->size(); i++)
            {
                pmf_point<REAL> * pEl = bHeap->get(i);

                if (newPt != pEl->n1  &&  pEl->n1 != NULL  &&
                    //pEl->data->n1->x < pEl->data->x  &&
                    PT_LT(pEl->n1, pEl, sinL, cosL)  &&
                    cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y) == 1 )
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y, xx, yy);
                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n1, 0.0, 0.0, ++id, PT_INTERSECTION);

                    iHeap->insert (newpt2, newPt->id, pEl->id);

                    cerr << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << endl;
                }

                if (newPt != pEl->n2  &&  pEl->n2 != NULL  &&
                    //pEl->data->n2->x < pEl->data->x  &&
                    PT_LT(pEl->n2, pEl, sinL, cosL)  &&
                    cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y) == 1 )
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y, xx, yy);
                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n2, 0.0, 0.0, ++id, PT_INTERSECTION);

                    iHeap->insert (newpt2, newPt->id, pEl->id);

                    cerr << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << endl;
                }
            }
        }
    }
    /* Adding point to the list */
    cerr << " --- INS --- : " << *newPt << endl;
    bHeap->insert(newPt);
    //bHeap->insert(newPt, blocks);
    return true;
}


#define DEL_PATH_LOG 1
#include <stack>
inline
pmf_point<REAL> * pmf_delete_rotated_path (
                                    pmf_point<REAL> * cpt,
                                    pmf_point<REAL> * dpt,
                                    BirthsHeap<REAL> * bHeap,
                                    IntersectionsHeap<REAL> * iHeap,
                                    BlocksLists<REAL> * blocks,
                                    long & ptId,
                                    REAL fieldHeight,
                                    REAL fieldWidth,
                                    REAL sinL = 0.0,
                                    REAL cosL = 1.0
                                )
{
    pmf_point<REAL> * newPt = NULL;
    std::stack<long int> st;
#if DEL_PATH_LOG
    cout << " DELETEING PATH : " << *dpt << endl;
#endif
    while (true) {
        /* Point to delete has 2 neighbors - the worst case */
#if DEL_PATH_LOG
        cout << " " << *dpt;
#endif
        if (dpt->n1 != NULL  &&  dpt->n2 != NULL) {
            if (dpt->type == PT_INTERSECTION) {
                pmf_point<REAL> * dptn;
                if (st.empty()) {
                    assert(cpt != NULL);
                    if (cpt->n1 != NULL  &&  cpt->n1->id == dpt->n1->id)  dptn = dpt->n2;
                    if (cpt->n1 != NULL  &&  cpt->n1->id == dpt->n2->id)  dptn = dpt->n1;
                    if (cpt->n2 != NULL  &&  cpt->n2->id == dpt->n1->id)  dptn = dpt->n2;
                    if (cpt->n2 != NULL  &&  cpt->n2->id == dpt->n2->id)  dptn = dpt->n1;
                }
                else {
                    if (dpt->n1->id == st.top())  dptn = dpt->n2;
                    else  dptn = dpt->n1;
                }

                st.push(dpt->id);
                newPt = new pmf_point<REAL>(dpt->x, dpt->y, dptn, NULL, 0.0, 0.0, dpt->id, PT_UPDATE);
                if (dptn->n1 != NULL  &&  dptn->n1->id == dpt->id)  dptn->n1 = newPt;
                else
                    if (dptn->n2 != NULL  &&  dptn->n2->id == dpt->id)  dptn->n2 = newPt;
                break;
            }
            else {
                st.push(dpt->id);
                //if (dpt->n1->x > dpt->x) { dpt = dpt->n1; continue; }
                if (PT_LT(dpt, dpt->n1, sinL, cosL)) { dpt = dpt->n1; continue; }
                //if (dpt->n2->x > dpt->x) { dpt = dpt->n2; continue; }
                if (PT_LT(dpt, dpt->n2, sinL, cosL)) { dpt = dpt->n2; continue; }
                assert(false);
            }
        }
        else { st.push(dpt->id); }

        /* Point to delete has only 1 neighbor at first pointer */
        if (dpt->n1 != NULL  &&  dpt->n2 == NULL) {
            //if (dpt->n1->x <= dpt->x) break;
            if (PT_LE(dpt->n1, dpt, sinL, cosL)) break;
            dpt = dpt->n1;
            continue;
        }
        /* Point to delete has only 1 neighbor at 2nd pointer */
        if (dpt->n1 == NULL  &&  dpt->n2 != NULL) {
            //if (dpt->n2->x <= dpt->x) break;
            if (PT_LE(dpt->n2, dpt, sinL, cosL)) break;
            dpt = dpt->n2;
            continue;
        }
        /* It should never execute this */
        assert(dpt->n1 || dpt->n2);
        break;
    }

    /* And now we remove all the points from lists */
#if DEL_PATH_LOG
    cout << endl << " IDS : ";
#endif
    while (! st.empty()) {
#if DEL_PATH_LOG
        cout << " " << st.top();
#endif
        long int id = st.top();
        st.pop();
        /* ... do the goods ... */
// TODO:
        //bHeap->remove_point_with_id(id, blocks);
        //iHeap->remove_intersection_with_id(id, blocks);
        bHeap->remove_point_with_id(id);
        iHeap->remove_intersections_with_id(id);
    }

#if DEL_PATH_LOG
    cout << endl;
    if (newPt)  cout << " RENEW : " << *newPt << endl;
#endif
    if (newPt != NULL)
        pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, newPt->n1, ptId, fieldHeight, fieldWidth, blocks);
    return newPt;
}

/*
inline
pmf_point<REAL> * pmf_do_heaps_top (
                                BirthsHeap<REAL> * bHeap,
                                IntersectionsHeap<REAL> * iHeap,
                                long & id1,
                                long & id2,
                                REAL sinL,
                                REAL cosL
                            )
{
    if (bHeap->empty() && iHeap->empty()) { return NULL; }
    else if (!bHeap->empty() && iHeap->empty()) { return bHeap->top(); }
    else if (bHeap->empty() && !iHeap->empty())
    {
        id1 = iHeap->topP1();
        id2 = iHeap->topP2();
        return iHeap->top();
    }
    else {
        if (PT_LE(bHeap->top(), iHeap->top(), sinL, cosL)) { return bHeap->top(); }
        else {
            id1 = iHeap->topP1();
            id2 = iHeap->topP2();
            return iHeap->top();
        }
    }
}
*/

inline
pmf_point<REAL> * pmf_do_heaps_get (
                        BirthsHeap<REAL> * bHeap,
                        IntersectionsHeap<REAL> * iHeap,
                        long & id1,
                        long & id2,
                        REAL sinL,
                        REAL cosL
                    )
{
    if (bHeap->empty() && iHeap->empty()) { return NULL; }
    else if (!bHeap->empty() && iHeap->empty()) {
        pmf_point<REAL> * pt = bHeap->extract_min();
        return pt;
    }
    else if (bHeap->empty() && !iHeap->empty()) {
        /*
        pmf_point<REAL> * pt = iHeap->top();
        id1 = iHeap->topP1();
        id2 = iHeap->topP2();
        return pt;
        */
        return iHeap->extract_min(id1, id2);
    }
    else {
        pmf_point<REAL> * bpt = bHeap->top();
        pmf_point<REAL> * ipt = iHeap->top();
        if (PT_LE(bpt, ipt, sinL, cosL)) {
            return bHeap->extract_min();
        }
        else {
            return iHeap->extract_min(id1, id2);
        }
    }
}


#undef X_ROTATED
#undef PT_LT
#undef PT_LE

#undef REAL

#endif // MODIFYHEAPS_HPP_INCLUDED
