#ifndef ACCESSLISTS_HPP_INCLUDED
#define ACCESSLISTS_HPP_INCLUDED

#include "geometry.hpp"


#define LOOK_FORWARD 1
template <class T_REAL>
inline
void pmf_check_crossings_in_block (
                                        pmf_point<T_REAL> * newPt,
                                        IntersectionsList<T_REAL> * iList,
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
#if LOOK_FORWARD
        switch (pt->type) {
            case PT_BIRTH_NORMAL :
                if (pt->n1)
                {
                    if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y) == 1)
                    {
                        T_REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y, xx, yy);
#ifdef DEBUG
                        cout << " CROSSED~1:" << pt->id << "-" << pt->n1->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iList->push_in_order (newpt2, newPt->id, pt->n1->id);
                    }
                }
                if (pt->n2)
                {
                    if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n2->x, pt->n2->y) == 1)
                    {
                        T_REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n2->x, pt->n2->y, xx, yy);
#ifdef DEBUG
                        cout << " CROSSED~2:" << pt->id << "-" << pt->n2->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iList->push_in_order (newpt2, newPt->id, pt->n2->id);
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
                        cout << " CROSSED~0:" << pt->id << "-" << pt->n1->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iList->push_in_order (newpt2, newPt->id, pt->n1->id);
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
                        cout << " CROSSED~U:" << pt->id << "-" << pt->n2->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iList->push_in_order (newpt2, newPt->id, pt->n2->id);
                    }
                }
            case PT_UNKNOWN      :
            case PT_BORDER       :
            case PT_INTERSECTION :
            default :
                //out << " " << *iter->data;
                ;;
        }
        iter = iter->next;
#else   // LOOK_FORWARD
        switch (pt->type) {
            case PT_UPDATE :
            case PT_BORDER :
                //if ( pt->n2 == NULL )// &&  pt->n1 != newPt  &&  pt->n1 != NULL)
                {
                    if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y) == 1)
                    {
                        T_REAL xx, yy;
                        crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y, xx, yy);
#ifdef DEBUG
                        cout << " CROSSED:" << pt->id << "-" << pt->n1->id << " " << endl;
#endif
                        pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pt->n1, 0.0, 0.0, ++id, PT_INTERSECTION);
                        newpt2->block = blocks->determine_point_block(newpt2);
                        blocks->push(newpt2);
                        iList->push_in_order (newpt2, newPt->id, pt->id);
                    }
                }
            default :
                //out << " " << *iter->data;
                iter = iter->next;
        }
#endif  // LOOK_FORWARD
    }
}
#undef LOOK_FORWARD


template <class T_REAL>
inline
bool pmf_store_points_in_blocks (
                        pmf_point<T_REAL> * newPt,
                        BirthsList<T_REAL> * bList,
                        IntersectionsList<T_REAL> * iList,
                        pmf_point<T_REAL> * parentPt,
                        long & id,
                        T_REAL fieldHeight,
                        T_REAL fieldWidth,
                        BlocksLists<T_REAL> * blocks
                    )
{
    if (newPt->type > 4) {
        /* Check if coordinates are not outside the field. */
        if( parentPt->type != PT_BIRTH_UP  &&
            cross3<T_REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, fieldWidth, 0.0f) != 0 )
        {
            T_REAL cx, cy;
            crosspoint2<T_REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, fieldWidth, 0.0f, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }
        if( parentPt->type != PT_BIRTH_DOWN &&
            cross3<T_REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, fieldHeight, fieldWidth, fieldHeight) != 0 )
        {
            T_REAL cx, cy;
            crosspoint2<T_REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, fieldHeight, fieldWidth, fieldHeight, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }
        if( cross3<T_REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, fieldWidth, 0.0f, fieldWidth, fieldHeight) != 0 ) {
            T_REAL cx, cy;
            crosspoint2<T_REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, fieldWidth, 0.0f, fieldWidth, fieldHeight, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }

        /* Check if pointer to blocks is NULL */
        if (blocks)
        {
            int block = blocks->determine_point_block(newPt);
            newPt->block = block;
            /* Calculate intersection points with other segments in neighbours' blocks */
            int ll, rr, uu, dd;
            block = parentPt->block;
            ll = blocks->left_from(block);
            rr = blocks->right_from(block);
            uu = blocks->up_from(block);
            dd = blocks->down_from(block);

            if (uu != BLOCK_UNDEFINED) {
                int lll = uu-1, rrr = uu+1;
                if (ll == BLOCK_UNDEFINED) lll = uu;
                if (rr == BLOCK_UNDEFINED) rrr = uu;

                /*  for i = lll .. rrr */
                for (int index = lll; index <= rrr; index++)
                    pmf_check_crossings_in_block (newPt, iList, id, parentPt, blocks, index);
            }
            if (dd != BLOCK_UNDEFINED) {
                int lll = dd-1, rrr = dd+1;
                if (ll == BLOCK_UNDEFINED) lll = dd;
                if (rr == BLOCK_UNDEFINED) rrr = dd;

                /*  for i = lll .. rrr */
                for (int index = lll; index <= rrr; index++)
                    pmf_check_crossings_in_block (newPt, iList, id, parentPt, blocks, index);
            }
            if (ll == BLOCK_UNDEFINED) ll = block;
            if (rr == BLOCK_UNDEFINED) rr = block;

            /*  for i = ll .. rr */
            for (int index = ll; index <= rr; index++)
                pmf_check_crossings_in_block (newPt, iList, id, parentPt, blocks, index);
        }
        else {
            Element<pmf_point<T_REAL> > * pEl = bList->getHead();
            while (pEl)
            {
                if (newPt != pEl->data->n1  &&  pEl->data->n1 != NULL  &&
                    cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y) == 1 )
                {
                    T_REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y, xx, yy);
                    pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(xx, yy, parentPt, pEl->data->n1, 0.0, 0.0, ++id, PT_INTERSECTION);
                    iList->push_in_order (newpt2, newPt->id, pEl->data->id);
                }
                pEl = pEl->next;
            }
        }
    }
    /* Adding point to the list */
    bList->push_in_order(newPt, blocks);
    return true;
}


/*
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
        // Check if coordinates are not outside the field.
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

        // Calculate intersection points with other segments
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
*/


template <class T_REAL>
inline
pmf_point<T_REAL> * pmf_do_get (
                        BirthsList<T_REAL> * bList,
                        IntersectionsList<T_REAL> * iList,
                        long & id1,
                        long & id2
                    )
{
    if (bList->empty() && iList->empty()) { return NULL; }
    else if (!bList->empty() && iList->empty()) {
        pmf_point<T_REAL> * pt = bList->front();
        bList->pop_front();
        return pt;
    }
    else if (bList->empty() && !iList->empty()) {
        pmf_point<T_REAL> * pt = (iList->front())->pt;
        id1 = iList->front()->p1;
        id2 = iList->front()->p2;
        iList->pop_front();
        return pt;
    }
    else {
        pmf_point<T_REAL> * bpt = bList->front();
        pmf_point<T_REAL> * ipt = iList->front()->pt;
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


template <class T_REAL>
inline
pmf_point<T_REAL> * pmf_do_top (
                        BirthsList<T_REAL> * bList,
                        IntersectionsList<T_REAL> * iList,
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
