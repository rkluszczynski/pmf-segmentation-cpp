#ifndef MODIFYLISTS_HPP_INCLUDED
#define MODIFYLISTS_HPP_INCLUDED

#include "point.hpp"

#define REAL double
int pmf_which_neighbor_is_id(pmf_point<REAL> * pt, long id)
{
    if (pt->n1 != NULL  &&  pt->n1->id == id) return 1;
    if (pt->n2 != NULL  &&  pt->n2->id == id) return 2;
    return 0;
}


#include <stack>
pmf_point<REAL> * pmf_delete_path (
                                    pmf_point<REAL> * cpt,
                                    pmf_point<REAL> * dpt,
                                    BirthsList<REAL> * bList,
                                    IntersectionsList<REAL> * iList,
                                    BlocksLists<REAL> * blocks,
                                    long & ptId,
                                    REAL fieldHeight,
                                    REAL fieldWidth
                                )
{
    pmf_point<REAL> * newPt = NULL;
    std::stack<long int> st;

    while (true) {
        /* Point to delete has 2 neighbors - the worst case */
        if (dpt->n1 != NULL  &&  dpt->n2 != NULL) {
            if (dpt->type == PT_INTERSECTION) {
                pmf_point<REAL> * dptn;
                // TODO
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
                newPt = new pmf_point<REAL>(dpt->x, dpt->y, dptn, NULL, 0.0, 0.0, ++ptId, PT_UPDATE);
                if (dptn->n1 != NULL  &&  dptn->n1->id == dpt->id)  dptn->n1 = newPt;
                else
                    if (dptn->n2 != NULL  &&  dptn->n2->id == dpt->id)  dptn->n2 = newPt;
                break;
            }
            else {
                st.push(dpt->id);
                if (dpt->n1->x > dpt->x) { dpt = dpt->n1; continue; }
                if (dpt->n2->x > dpt->x) { dpt = dpt->n2; continue; }
                assert(false);
            }
        }
        else { st.push(dpt->id); }

        /* Point to delete has only 1 neighbor at first pointer */
        if (dpt->n1 != NULL  &&  dpt->n2 == NULL) {
            if (dpt->n1->x <= dpt->x) break;
            dpt = dpt->n1;
            continue;
        }

        /* Point to delete has only 1 neighbor at 2nd pointer */
        if (dpt->n1 == NULL  &&  dpt->n2 != NULL) {
            if (dpt->n2->x <= dpt->x) break;
            dpt = dpt->n2;
            continue;
        }

        assert(dpt->n1 || dpt->n2);
        break;
    }

    /* And now we remove all the points from lists */
    while (! st.empty()) {
        long int id = st.top();
        st.pop();
        /* ... do the goods ... */
        bList->remove_point_with_id(id, blocks);
        iList->remove_intersection_with_id(id, blocks);
    }

    if (newPt != NULL)
        pmf_store_points_in_blocks(newPt, bList, iList, newPt->n1, ptId, fieldHeight, fieldWidth, blocks);
    return newPt;
}
#undef REAL

#endif // MODIFYLISTS_HPP_INCLUDED
