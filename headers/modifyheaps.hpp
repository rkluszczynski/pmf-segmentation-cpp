#ifndef MODIFYHEAPS_HPP_INCLUDED
#define MODIFYHEAPS_HPP_INCLUDED

#include "point.hpp"
#include "birthsheap.hpp"
#include "intersectionsheap.hpp"
#include "modifylists.hpp"

#define REAL double


//#define DEBUG 1
template <class T_REAL>
inline
void pmf_check_crossings (
                            pmf_point<T_REAL> * newPt,
                            IntersectionsHeap<T_REAL> * iHeap,
                            long & id,
                            pmf_point<T_REAL> * parentPt,
                            BlocksLists<T_REAL> * blocks,
                            int index
                        )
{
    Element<pmf_point<T_REAL> > * iter = blocks->getBlockList(index)->getHead();
    assert(parentPt == newPt->n1);
    while (iter) {
        pmf_point<T_REAL> * pt = iter->data;
        switch (pt->type) {
            case PT_BIRTH_NORMAL :
                if (pt->n1)
                {
                    if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y) == 1)
                    {
                        T_REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y, xx, yy);
#ifdef DEBUG
                        out << " CROSSED~1:" << pt->id << "-" << pt->n1->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iHeap->insert(newpt2, newPt->id, pt->n1->id);
                    }
                }
                if (pt->n2)
                {
                    if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n2->x, pt->n2->y) == 1)
                    {
                        T_REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n2->x, pt->n2->y, xx, yy);
#ifdef DEBUG
                        out << " CROSSED~2:" << pt->id << "-" << pt->n2->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iHeap->insert(newpt2, newPt->id, pt->n2->id);
                    }
                }
                break;;
            case PT_BIRTH_LEFT   :
            case PT_BIRTH_UP     :
            case PT_BIRTH_DOWN   :
                if (pt->n1)//&&  newPt != pt->n1 && pt != newPt)
                {
                    if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y) == 1)
                    {
                        T_REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y, xx, yy);
#ifdef DEBUG
                        out << " CROSSED~0:" << pt->id << "-" << pt->n1->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iHeap->insert(newpt2, newPt->id, pt->n1->id);
                    }
                }
                break;;
            case PT_UPDATE       :
                if (pt->n2)//&&  newPt != pt->n1 && pt != newPt)
                {
                    if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n2->x, pt->n2->y) == 1)
                    {
                        T_REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n2->x, pt->n2->y, xx, yy);
#ifdef DEBUG
                        out << " CROSSED~U:" << pt->id << "-" << pt->n2->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iHeap->insert(newpt2, newPt->id, pt->n2->id);
                    }
                }
            case PT_UNKNOWN      :
            case PT_BORDER       :
            case PT_INTERSECTION :
            default :
                ;;
        }
        iter = iter->next;
    }
    return;
}


#define DIST(PT1, PT2) (sqrt(((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y)))
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
            newPt->block = blocks->determine_point_block(newPt);
#ifdef DEBUG
            out << std::endl << " New point block = " << newPt->block << std::endl;
            out << blocks << std::endl;
            out << " checking ... ";
#endif
            // Calculate intersection points with other segments in neighbours' blocks
            int ll, rr, uu, dd;
            vector<int> vec;
            vec.push_back(-1);

            int block = parentPt->block;
            ll = blocks->left_from(block);
            rr = blocks->right_from(block);
            uu = blocks->up_from(block);
            dd = blocks->down_from(block);

            if (uu != BLOCK_UNDEFINED) {
                int lll = uu-1, rrr = uu+1;
                if (ll == BLOCK_UNDEFINED) lll = uu;
                if (rr == BLOCK_UNDEFINED) rrr = uu;
                //  for i = lll .. rrr
                for (int index = lll; index <= rrr; index++) vec.push_back(index);
            }
            if (dd != BLOCK_UNDEFINED) {
                int lll = dd-1, rrr = dd+1;
                if (ll == BLOCK_UNDEFINED) lll = dd;
                if (rr == BLOCK_UNDEFINED) rrr = dd;
                //  for i = lll .. rrr
                for (int index = lll; index <= rrr; index++) vec.push_back(index);
            }
            if (ll == BLOCK_UNDEFINED) ll = block;
            if (rr == BLOCK_UNDEFINED) rr = block;
            //  for i = ll .. rr
            for (int index = ll; index <= rr; index++) vec.push_back(index);
#if 1
            block = newPt->block;
            ll = blocks->left_from(block);
            rr = blocks->right_from(block);
            uu = blocks->up_from(block);
            dd = blocks->down_from(block);

            if (uu != BLOCK_UNDEFINED) {
                int lll = uu-1, rrr = uu+1;
                if (ll == BLOCK_UNDEFINED) lll = uu;
                if (rr == BLOCK_UNDEFINED) rrr = uu;
                //  for i = lll .. rrr
                for (int index = lll; index <= rrr; index++) vec.push_back(index);
            }
            if (dd != BLOCK_UNDEFINED) {
                int lll = dd-1, rrr = dd+1;
                if (ll == BLOCK_UNDEFINED) lll = dd;
                if (rr == BLOCK_UNDEFINED) rrr = dd;
                //  for i = lll .. rrr
                for (int index = lll; index <= rrr; index++) vec.push_back(index);
            }
            if (ll == BLOCK_UNDEFINED) ll = block;
            if (rr == BLOCK_UNDEFINED) rr = block;
            //  for i = ll .. rr
            for (int index = ll; index <= rr; index++) vec.push_back(index);
#endif
            // Checking the blocks
            sort(vec.begin(), vec.end());
            for (unsigned int i = 1; i < vec.size(); i++)
            {
                if (vec[i] != vec[i-1])
                    pmf_check_crossings<REAL> (newPt, iHeap, id, parentPt, blocks, vec[i]);
#ifdef DEBUG
                out << vec[i] << " ";
#endif
            }
            blocks->push( newPt );
#ifdef DEBUG
            out << std::endl;
#endif
        }
        else {
            for (int i = 0; i < bHeap->size(); i++)
            {
                pmf_point<REAL> * pEl = bHeap->get(i);

                int c1 = -1;
                int c2 = -1;

                if (newPt != pEl->n1  &&  pEl->n1 != NULL  &&
                    //pEl->data->n1->x < pEl->data->x  &&
                    PT_LT(pEl->n1, pEl, sinL, cosL) )
                {
                    c1 = cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y);
                    if (c1 == 1)
                    {
                        REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y, xx, yy);

                //int ans = pmf_which_neighbor_is_id(parentPt, newPt->id);
                //assert(ans > 0);
                //REAL parentLength = (ans == 1) ? parentPt->l1 : parentPt->l2;
                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n1, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n1->l1 : pEl->n1->l2;

                        pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n1, ll1, ll2, ++id, PT_INTERSECTION);

                        iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                        out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
                        double ddist1 = DIST(newpt2, newpt2->n1);
                        double ddist2 = DIST(newpt2, newpt2->n2);
                        out << " --   1   -- : " << ddist1 << std::endl;
                        out << " --   2   -- : " << ddist2 << std::endl;
                        assert(ddist1 <= newpt2->l1 + EPSILON);
                        assert(ddist2 <= newpt2->l2 + EPSILON);
#endif
                    }
                }

                if (newPt != pEl->n2  &&  pEl->n2 != NULL  &&
                    //pEl->data->n2->x < pEl->data->x  &&
                    PT_LT(pEl->n2, pEl, sinL, cosL) )
                {
                    c2 = cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y);
                    if (c2 == 1) {
                        REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y, xx, yy);

                //int ans = pmf_which_neighbor_is_id(parentPt, newPt->id);
                //assert(ans == 1 || ans == 2);
                //REAL parentLength = (ans == 1) ? parentPt->l1 : parentPt->l2;
                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n2, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n2->l1 : pEl->n2->l2;

                        pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n2, ll1, ll2, ++id, PT_INTERSECTION);

                        iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                        out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
                        double ddist1 = DIST(newpt2, newpt2->n1);
                        double ddist2 = DIST(newpt2, newpt2->n2);
                        out << " --   1   -- : " << ddist1 << std::endl;
                        out << " --   2   -- : " << ddist2 << std::endl;
                        assert(ddist1 <= newpt2->l1 + EPSILON);
                        assert(ddist2 <= newpt2->l2 + EPSILON);
#endif
                    }
                }
#if pmf_LOG_ADD
                if (c1 > 1)  out << " -- ----- -- :  c1=" << c1 << "  :: " << *pEl << " ~ " << *pEl->n1 << std::endl;
                if (c2 > 1)  out << " -- ----- -- :  c2=" << c2 << "  :: " << *pEl << " ~ " << *pEl->n2 << std::endl;
#endif
            }
        }
    }
    /* Adding point to the list */
#if pmf_LOG_ADD
    out << " --- INS --- : " << *newPt << std::endl;
#endif
    //bHeap->insert(newPt, blocks);
    //*
    bHeap->insert(newPt);
    //*/
    return true;
}
#undef DIST


#define DIST(PT1, PT2) (sqrt(((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y)))
#include <stack>
inline
pmf_point<REAL> * pmf_delete_rotated_path (
                                    pmf_point<REAL> * cpt,
                                    pmf_point<REAL> * dpt,
                                    BirthsHeap<REAL> * bHeap,
                                    IntersectionsHeap<REAL> * iHeap,
                                    BlocksLists<REAL> * blocks,
                                    long & ptId,
                                    EdgePoints<REAL> * ep,
                                    REAL fieldHeight,
                                    REAL fieldWidth,
                                    REAL sinL = 0.0,
                                    REAL cosL = 1.0
                                )
{
    pmf_point<REAL> * newPt = NULL;
    std::stack<long int> st;
#if DEL_PATH_LOG
    out << " DELETEING PATH : " << *dpt << endl;
#endif
    while (true) {
        /* Point to delete has 2 neighbors - the worst case */
#if DEL_PATH_LOG
        out << " " << *dpt;
#endif
        if (dpt->n1 != NULL  &&  dpt->n2 != NULL) {
            if (dpt->type == PT_INTERSECTION) {
                pmf_point<REAL> * dptn = NULL;
                if (st.empty()) {
#if CHECK_ASSERTIONS
                    assert(cpt != NULL);
#endif
                    if (cpt->n1 != NULL  &&  cpt->n1->id == dpt->n1->id)  dptn = dpt->n2;
                    if (cpt->n1 != NULL  &&  cpt->n1->id == dpt->n2->id)  dptn = dpt->n1;
                    if (cpt->n2 != NULL  &&  cpt->n2->id == dpt->n1->id)  dptn = dpt->n2;
                    if (cpt->n2 != NULL  &&  cpt->n2->id == dpt->n2->id)  dptn = dpt->n1;
                }
                else {
                    if (dpt->n1->id == st.top())  dptn = dpt->n2;
                    else  dptn = dpt->n1;
                }
#if CHECK_ASSERTIONS
                assert(dptn != NULL);
#endif
                st.push(dpt->id);
                REAL length1 = (dptn->n1->id == dpt->id) ? dptn->l1 : dptn->l2;
                REAL length2 = (dptn->n1->id == dpt->id) ? dptn->l2 : dptn->l1;
                length2 = 0.0;//Exp<REAL>(2.0);
// TODO (Rafal#1#): use length to recreate point
// DONE : 27-12-2008

                cout << " >> !dptn ! >> " << *dptn << endl;
                cout << " >> ! dpt ! >> " << *dpt << endl;
                cout << " >>!length1!>> " << length1 << endl;
                REAL dist = DIST(dptn, dpt);
                cout << " >>! dist ! >> " << dist << endl;
                /// TODO (Rafel#1#): sprawdzic nieostrosc nierownosci
                assert(dist <= length1 + EPSILON);

                REAL xx = dptn->x;
                REAL yy = dptn->y;

                double scale = (length1 / dist);
                xx += scale * ( dpt->x - dptn->x );
                yy += scale * ( dpt->y - dptn->y );
                cout << " >>! scale! >> " << scale << endl;

                newPt = new pmf_point<REAL>(xx, yy, dptn, NULL, length1, length2, dpt->id, PT_UPDATE);
                if (dptn->n1 != NULL  &&  dptn->n1->id == dpt->id)  dptn->n1 = newPt;
                else
                    if (dptn->n2 != NULL  &&  dptn->n2->id == dpt->id)  dptn->n2 = newPt;

                cout << " >>! newPt! >> " << *newPt << endl;
                cout << " >>! n1   ! >> " << *newPt->n1 << endl;
                assert(DIST(newPt, newPt->n1) <= length1 + EPSILON);

                break;
            }
            else {
                st.push(dpt->id);
                //if (dpt->n1->x > dpt->x) { dpt = dpt->n1; continue; }
                //if (PT_LT(dpt, dpt->n1, sinL, cosL)) { dpt = dpt->n1; continue; }
                if (PT_LE(dpt, dpt->n1, sinL, cosL)) { dpt = dpt->n1; continue; }
                //if (dpt->n2->x > dpt->x) { dpt = dpt->n2; continue; }
                //if (PT_LT(dpt, dpt->n2, sinL, cosL)) { dpt = dpt->n2; continue; }
                if (PT_LE(dpt, dpt->n2, sinL, cosL)) { dpt = dpt->n2; continue; }
#if CHECK_ASSERTIONS
                assert(false);
#endif
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
#if CHECK_ASSERTIONS
        assert(dpt->n1 || dpt->n2);
#endif
        break;
    }

    /* And now we remove all the points from lists */
#if DEL_PATH_LOG
    out << endl << " IDS : ";
#endif
    while (! st.empty()) {
#if DEL_PATH_LOG
        out << " " << st.top();
#endif
        long int id = st.top();
        st.pop();
        /* ... do the goods ... */
        if (ep) {
            long index = ep->hasPointWithID(id);
            if (index >= 0) {
                cerr << "[ ERROR ] : removing previous segment point" << endl;
                ep->get(index)->pt = NULL;
            }
        }
        bHeap->remove_point_with_id(id, blocks);
        iHeap->remove_intersections_with_id(id, blocks);
    }

#if DEL_PATH_LOG
    out << endl;
    if (newPt)  out << " RENEW : " << *newPt << endl;
#endif
    if (newPt != NULL)
        pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, newPt->n1, ptId, fieldHeight, fieldWidth, blocks, sinL, cosL);

    return newPt;
}
#undef DIST

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
    else
        if (!bHeap->empty() && iHeap->empty()) { return bHeap->extract_min(); }
    else
        if (bHeap->empty() && !iHeap->empty()) { return iHeap->extract_min(id1, id2); }
    else {
        pmf_point<REAL> * bpt = bHeap->top();
        pmf_point<REAL> * ipt = iHeap->top();
        //if (PT_LE(bpt, ipt, sinL, cosL)) { return bHeap->extract_min(); }
        REAL xb = X_ROTATED(bpt->x, bpt->y, sinL, cosL);
        REAL xi = X_ROTATED(ipt->x, ipt->y, sinL, cosL);
#if pmf_LOG_ADD
        out.precision(128);  out << " bpt = " << xb << "  :: " << *bpt << endl;
        out.precision(128);  out << " ipt = " << xi << "  :: " << *ipt << endl;
        out << "[    ]  " << ((xb < xi) ? " LT " : " ! LT ")  << endl;
        //out << "[ PT ]  " << (PT_LT(bpt, ipt, sinL, cosL) ? " LT " : " ! LT ")  << endl;
#endif
        //if (PT_LT(bpt, ipt, sinL, cosL)) { return bHeap->extract_min(); }
        if (xb < xi) { return bHeap->extract_min(); }
        else { return iHeap->extract_min(id1, id2); }
    }
}

#undef X_ROTATED
#undef PT_LT
#undef PT_LE

#undef REAL

#endif // MODIFYHEAPS_HPP_INCLUDED
