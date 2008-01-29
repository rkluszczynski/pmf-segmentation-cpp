#ifndef ACCESSLISTS_HPP_INCLUDED
#define ACCESSLISTS_HPP_INCLUDED

#include "geometry.hpp"

inline void pmf_check_crossings_in_block (
                                            PMF_POINT * newPt,
                                            IntersectionsList<REAL> * iList,
                                            long & id,
                                            PMF_POINT * parentPt,
                                            BlocksLists<REAL> * blocks,
                                            int index
                                        )
{
    Element<pmf_point<REAL> > * iter = blocks->getBlockList(index)->getHead();
    while (iter) {
        pmf_point<REAL> * pt = iter->data;
        // TODO : check all the crossings
        if ( (pt->type == PT_UPDATE  ||  pt->type == PT_BORDER)
           )// &&  pt->n2 == NULL )// &&  pt->n1 != newPt  &&  pt->n1 != NULL)
        {
            if (cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y) == 1)
            {
                REAL xx, yy;
                crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pt->x, pt->y, pt->n1->x, pt->n1->y, xx, yy);
#ifdef DEBUG
                cout << " CROSSED:" << pt->id << "-" << pt->n1->id << " " << endl;
#endif
                PMF_POINT * newpt2 = new PMF_POINT(xx, yy, parentPt, pt->n1, 0.0, 0.0, ++id, PT_INTERSECTION);
                newpt2->block = blocks->determine_point_block(newpt2);
                blocks->push(newpt2);
                iList->push_in_order (newpt2, newPt->id, pt->id);
            }
        }
        //out << " " << *iter->data;
        iter = iter->next;
    }
}


bool pmf_store_points_in_blocks (
                        PMF_POINT * newPt,
                        BirthsList<REAL> * bList,
                        IntersectionsList<REAL> * iList,
                        PMF_POINT * parentPt,
                        long & id,
                        REAL fieldHeight,
                        REAL fieldWidth,
                        BlocksLists<REAL> * blocks
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
            Element<pmf_point<REAL> > * pEl = bList->getHead();
            while (pEl) {

                if (newPt != pEl->data->n1  &&  pEl->data->n1 != NULL &&
                    cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y) == 1 )
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y, xx, yy);
                    PMF_POINT * newpt2 = new PMF_POINT(xx, yy, parentPt, pEl->data->n1, 0.0, 0.0, ++id, PT_INTERSECTION);
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
