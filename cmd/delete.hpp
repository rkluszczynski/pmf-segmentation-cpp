#ifndef DELETE_HPP_INCLUDED
#define DELETE_HPP_INCLUDED

template <class REAL>
void
PMF<REAL> :: DeleteFuturePath ()
{

    return;
}

#if 0

#define DIST2(PT1, PT2) (((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y))
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
    if (saveOp)  out << " DELETEING PATH : " << *dpt << endl;
#endif
    while (true) {
        /* Point to delete has 2 neighbors - the worst case */
#if DEL_PATH_LOG
        if (saveOp)  out << " " << *dpt;
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
                REAL length2 = (dpt->n1->id == dptn->id) ? dpt->l2 : dpt->l1;
                length2 = Exp<REAL>(2.0);
// TODO (Rafal#1#): use length to recreate point
// DONE : 27-12-2008

                cout << " >> !dptn ! >> " << *dptn << endl;
                cout << " >> ! dpt ! >> " << *dpt << endl;
                cout << " >>!length1!>> " << length1 << endl;
                cout << " >>!length2!>> " << length2 << endl;
                REAL dist = sqrt( DIST2(dptn, dpt) );
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
                //newPt = new pmf_point<REAL>(xx, yy, dptn, NULL, length1, 0.0, dpt->id, PT_UPDATE);
                if (dptn->n1 != NULL  &&  dptn->n1->id == dpt->id)  dptn->n1 = newPt;
                else
                    if (dptn->n2 != NULL  &&  dptn->n2->id == dpt->id)  dptn->n2 = newPt;

                cout << " >>! newPt! >> " << *newPt << endl;
                cout << " >>! n1   ! >> " << *newPt->n1 << endl;
                assert( sqrt( DIST2(newPt, newPt->n1) ) <= length1 + EPSILON);

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
    if (saveOp)  out << endl << " IDS : ";
#endif
    while (! st.empty()) {
#if DEL_PATH_LOG
        if (saveOp)  out << " " << st.top();
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
    if (saveOp)
    {
        out << endl;
        if (newPt)  out << " RENEW : " << *newPt << endl;
    }
#endif
    if (newPt != NULL)
        pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, newPt->n1, ptId, fieldHeight, fieldWidth, blocks, sinL, cosL);

    return newPt;
}
#undef DIST2

#endif


#endif // DELETE_HPP_INCLUDED
