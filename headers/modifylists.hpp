#ifndef MODIFYLISTS_HPP_INCLUDED
#define MODIFYLISTS_HPP_INCLUDED

#include "point.hpp"

#define REAL double
inline
int pmf_which_neighbor_is_id(pmf_point<REAL> * pt, long id)
{
    if (pt->n1 != NULL  &&  pt->n1->id == id) return 1;
    if (pt->n2 != NULL  &&  pt->n2->id == id) return 2;
    return 0;
}


inline
void pmf_correct_new_intersection_point (pmf_point<REAL> * pt, long id1, long id2)
{
    int i1 = pmf_which_neighbor_is_id (pt->n1, id1);
    int i2 = pmf_which_neighbor_is_id (pt->n2, id2);
    if (i1 > 0  &&  i2 > 0) {
        if (i1 == 1)  { pt->n1->n1 = pt;  pt->l1 = pt->n1->l1; }
        else if (i1 == 2)  { pt->n1->n2 = pt;  pt->l1 = pt->n1->l2; }
        else assert(false);

        if (i2 == 1)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        else if (i2 == 2)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
        else assert(false);

        return;
    }
    i1 = pmf_which_neighbor_is_id (pt->n1, id2);
    i2 = pmf_which_neighbor_is_id (pt->n2, id1);
    if (i1 > 0  &&  i2 > 0) {
        if (i1 == 1)  { pt->n1->n1 = pt;  pt->l1 = pt->n1->l1; }
        else if (i1 == 2)  { pt->n1->n2 = pt;  pt->l1 = pt->n1->l2; }
        else assert(false);

        if (i2 == 1)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        else if (i2 == 2)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
        else assert(false);

        return;
    }
    assert(false);
}


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_LE(PP1,PP2,SSIN,CCOS) (! PT_LT(PP2,PP1,SSIN,CCOS))
inline
bool pmf_store_modified_points_in_blocks (
                        pmf_point<REAL> * newPt,
                        BirthsList<REAL> * bList,
                        IntersectionsList<REAL> * iList,
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

        /* Check if pointer to blocks is NULL */
        if (blocks)
        {
            assert(false);
        }
        else {
            Element<pmf_point<REAL> > * pEl = bList->getHead();
            while (pEl)
            {
                if (newPt != pEl->data->n1  &&  pEl->data->n1 != NULL  &&
                    //pEl->data->n1->x < pEl->data->x  &&
                    PT_LT(pEl->data->n1, pEl->data, sinL, cosL)  &&
                    cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y) == 1 )
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n1->x, pEl->data->n1->y, xx, yy);
                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->data->n1, 0.0, 0.0, ++id, PT_INTERSECTION);
                    iList->push_in_order (newpt2, newPt->id, pEl->data->id);
                }
                if (newPt != pEl->data->n2  &&  pEl->data->n2 != NULL  &&
                    //pEl->data->n2->x < pEl->data->x  &&
                    PT_LT(pEl->data->n2, pEl->data, sinL, cosL)  &&
                    cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n2->x, pEl->data->n2->y) == 1 )
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->data->x, pEl->data->y, pEl->data->n2->x, pEl->data->n2->y, xx, yy);
                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->data->n2, 0.0, 0.0, ++id, PT_INTERSECTION);
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


#include <stack>
inline
pmf_point<REAL> * pmf_delete_path (
                                    pmf_point<REAL> * cpt,
                                    pmf_point<REAL> * dpt,
                                    BirthsList<REAL> * bList,
                                    IntersectionsList<REAL> * iList,
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

    while (true) {
        /* Point to delete has 2 neighbors - the worst case */
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
                if (dpt->n1->x > dpt->x) { dpt = dpt->n1; continue; }
                if (dpt->n2->x > dpt->x) { dpt = dpt->n2; continue; }
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
    while (! st.empty()) {
        long int id = st.top();
        st.pop();
        /* ... do the goods ... */
        bList->remove_point_with_id(id, blocks);
        iList->remove_intersection_with_id(id, blocks);
    }

    if (newPt != NULL)
        pmf_store_modified_points_in_blocks(newPt, bList, iList, newPt->n1, ptId, fieldHeight, fieldWidth, blocks);
    return newPt;
}
#undef X_ROTATED
#undef PT_LT
#undef PT_LE

#undef REAL


/*
void storePoints2(struct Tpoint *newPt, listB *qB, listI *qI, long *idPktu, struct borderPoint * Bord)
{
    struct TlistBEement *tmpW;
    float pX, pY;
    struct Tpoint *point;

    if (isInsideBorder(newPt->x, newPt->y, Bord) == 1)
    {
	for(int i = 0; i < 4; i++)
	{
	    if( cross3(newPt->x, newPt->y, newPt->r1->x, newPt->r1->y,
			Bord[i].x, Bord[i].y, Bord[i+1].x, Bord[i+1].y)
		    != 0 )
	    {
		crosspoint(newPt->x, newPt->y, newPt->r1->x, newPt->r1->y,
			Bord[i].x, Bord[i].y, Bord[i+1].x, Bord[i+1].y, &pX, &pY);
		newPt->x = pX;
		newPt->y = pY;
		newPt->r2 = NULL;
		break;
	    }
	}
    }
    tmpW = qB->getHead();
    while(tmpW)
    {
	if(newPt->r1 != tmpW->point->r1  && tmpW->point->r1 != NULL &&
		tmpW->point->r1->x < tmpW->point->x &&
		cross3(newPt->x, newPt->y, newPt->r1->x, newPt->r1->y,
		    tmpW->point->x, tmpW->point->y, tmpW->point->r1->x,
		    tmpW->point->r1->y) == 1 )
	{
	    crosspoint(newPt->x, newPt->y, newPt->r1->x, newPt->r1->y,
		    tmpW->point->x, tmpW->point->y, tmpW->point->r1->x,
		    tmpW->point->r1->y, &pX, &pY);
	    point = allocPoint(pX, pY, newPt->r1, tmpW->point->r1, 0, 0, idPktu);
	    qI->put(point, tmpW->point->id, newPt->id);
	}
	if(newPt->r1 != tmpW->point->r2  && tmpW->point->r2 != NULL &&
		tmpW->point->r2->x < tmpW->point->x &&
		cross3(newPt->x, newPt->y, newPt->r1->x, newPt->r1->y,
		    tmpW->point->x, tmpW->point->y, tmpW->point->r2->x,
		    tmpW->point->r2->y) == 1 )
	{
	    crosspoint(newPt->x, newPt->y, newPt->r1->x, newPt->r1->y,
		    tmpW->point->x, tmpW->point->y, tmpW->point->r2->x,
		    tmpW->point->r2->y, &pX, &pY);
	    point = allocPoint(pX, pY, newPt->r1, tmpW->point->r2, 0, 0, idPktu);
	    qI->put(point, tmpW->point->id, newPt->id);
	}
	tmpW = tmpW->next;
    }
    qB->put(newPt);
    return;
}


#include <vector>
#include <stack>
struct Tpoint *delPathS(struct Tpoint *akt, long id, listB *qB, listI *qI, long *idPktu, struct borderPoint * Bord)
{
    struct Tpoint *res = NULL, *pt, *addr;
//    std::vector<long int> st;
		//modified 8th Dec 2006
    std::stack<long int> st;

    //st.reserve(200);
    pt = qB->getPoint(id);
    while(1)
    {
	if(pt->r1 != NULL  &&  pt->r2 != NULL)
	{
	    //if( pt->r1->x <= pt->x  &&  pt->r2->x <= pt->x )
		//modified 8th Dec 2006
	    if( pt->r1->x < pt->x  &&  pt->r2->x < pt->x )
	    {
		if(st.empty())
		{
		    if(akt == NULL)
		    {
			fprintf(stderr, "[ERROR] sth's wrong !!!");
			exit(-2);
		    }
		    else
		    {
			if(akt->r1 != NULL && akt->r1->id == pt->r1->id) addr = pt->r2;
			if(akt->r1 != NULL && akt->r1->id == pt->r2->id) addr = pt->r1;
			if(akt->r2 != NULL && akt->r2->id == pt->r1->id) addr = pt->r2;
			if(akt->r2 != NULL && akt->r2->id == pt->r2->id) addr = pt->r1;
		    }
		}
		else {
		    //if(pt->r1->id == st.back()) addr = pt->r2;
		    if(pt->r1->id == st.top()) addr = pt->r2;
		    else addr = pt->r1;
		}

		//st.push_back(id);
		st.push(id);

		res = allocPoint(pt->x, pt->y, addr, NULL, 0, 0, idPktu);
		if(addr->r1 != NULL  &&  addr->r1->id == id)  addr->r1 = res;
		else if(addr->r2 != NULL  &&  addr->r2->id == id)  addr->r2 = res;

//		   fprintf(fstep, "(%.3f;%.3f;%li;%li;id=%li)!", res->x, res->y,
//		   (res->r1) ? res->r1->id : 0,
//		   (res->r2) ? res->r2->id : 0,
//		   res->id);
		break;
	    }
	    else {
		//st.push_back(id);
		st.push(id);
		if(pt->r1->x + EPSILON >= pt->x)
		//modified 8th Dec 2006
		//if(pt->r2->id == id)
		{
		    id = pt->r1->id;
		    pt = pt->r1;
		    continue;
		}
		if(pt->r2->x + EPSILON >= pt->x)
		//modified 8th Dec 2006
		//if(pt->r1->id == id)
		{
		    id = pt->r2->id;
		    pt = pt->r2;
		    continue;
		}
		fprintf(stderr, " [ this should not be ever read ] \n");  fflush(stderr);
	    }
	}
	//else  { st.push_back(id); }
	else  { st.push(id); }

	if(pt->r1 != NULL  &&  pt->r2 == NULL)
	{
	    //if(pt->r1->x < pt->x) break;
		//modified 8th Dec 2006
	    if(pt->r1->x <= pt->x) break;
	    id = pt->r1->id;
	    pt = pt->r1;
	    continue;
	}
	if(pt->r2 != NULL  &&  pt->r1 == NULL)
	{
	    //if(pt->r2->x < pt->x) break;
		//modified 8th Dec 2006
	    if(pt->r2->x <= pt->x) break;
	    id = pt->r2->id;
	    pt = pt->r2;
	    continue;
	}
	break;
    }

    while(! st.empty())
    {
	//id = st.back();
	id = st.top();
	//st.pop_back();
	st.pop();

	qB->remove(id);
	qI->remove(id);
    }

    if(res != NULL) Points2(res, qB, qI, idPktu, Bord);
    return(res);
}

 */
#endif // MODIFYLISTS_HPP_INCLUDED
