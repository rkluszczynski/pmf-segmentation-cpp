
#include "configurationlist.hpp"
#include "birthslist.hpp"
#include "intersectionslist.hpp"
#include "probability.hpp"
#include "accesslists.hpp"
#include "modifylists.hpp"

#define REAL double

#define PT_LT(X, Y) ((X) < (Y))
void pmf_add_point (
            ConfigurationList<REAL> * PMF,
            ConfigurationList<REAL> * newPMF,
            REAL xx,
            REAL yy
        )
{
    REAL fieldWidth  = PMF->get_field_width();
    REAL fieldHeight = PMF->get_field_height();
    PMF->set_points_ids();

    long oldSize = PMF->get_size() + 1;
    long ptId = oldSize;

    pmf_point<REAL> * pt;
    while (! PMF->empty())
    {
        pt = PMF->front();
        PMF->pop_front();
        newPMF->push_back(pt);
    }
/*
    pmf_point<REAL> * pt;
    while ((pt = PMF->front()) != NULL  &&  PT_LT(pt->x, xx))
    {
        PMF->pop_front();
        newPMF->push_back(pt);
    }

    pt = new pmf_point<REAL>(xx, yy, NULL, NULL, 0.0, 0.0, ++ptId, PT_BIRTH_NORMAL);
    PMF->push_back(pt);

    BirthsList<REAL> *        birthList = new BirthsList<REAL> ();
    IntersectionsList<REAL> * crossList = new IntersectionsList<REAL> ();
    BlocksLists<REAL> *     blocksLists = NULL;//new BlocksLists<REAL> (fieldWidth, fieldHeight, 1.1);

    while (! PMF->empty())
    {
        birthList->push_back(PMF->front());
        PMF->pop_front();
    }

    REAL lowerAngle, upperAngle;
    pmf_point<REAL> * newPt;
    REAL length, coordX, coordY;

    determineBirthAngles(upperAngle, lowerAngle);

    length = Exp<REAL>(2.0);
    coordX = pt->x + length * cos(upperAngle);
    coordY = pt->y + length * sin(upperAngle);
    newPt = new pmf_point<REAL>(coordX, coordY, pt, NULL, length, 0.0, ++ptId, PT_UPDATE);
    pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
    pt->n1 = newPt;
    pt->l1 = newPt->l1;

    length = Exp<REAL>(2.0);
    coordX = pt->x + length * cos(lowerAngle);
    coordY = pt->y + length * sin(lowerAngle);
    newPt = new pmf_point<REAL>(coordX, coordY, pt, NULL, length, 0.0, ++ptId, PT_UPDATE);
    pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
    pt->n2 = newPt;
    pt->l2 = newPt->l1;

    // and the riots start ...
    long id1, id2;
    double angle, newAngle;

    while( !birthList->empty() || !crossList->empty() )
    {
        pt = pmf_do_get( birthList, crossList, id1, id2 );
        PMF->push_back(pt);

        if( pt->id <= oldSize )
        {
            if(pt->n1 != NULL  &&  pt->n2 == NULL)
            {
                angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));

                determineUpdateAngle(newAngle);
                newAngle += angle;
                if(newAngle >  M_PI/2.0) newAngle -= M_PI;
                if(newAngle < -M_PI/2.0) newAngle += M_PI;

                length = Exp<REAL>(2.0);
                coordX = pt->x + length*cos(newAngle);
                coordY = pt->y + length*sin(newAngle);
                newPt = new pmf_point<REAL>(coordX, coordY, pt, NULL, length, 0.0, ++ptId, PT_UPDATE);

                pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
                pt->n2 = newPt;
                pt->l2 = length;
            }
        }
        else {
            if (pt->n1 != NULL  &&  pt->n2 == NULL)
//                &&  isInsideBorder(akt->x,akt->y,Bord) == 0
//                &&  isOnBorder(akt->x,akt->y,Bord) < 0
            {
                angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));

                determineUpdateAngle(newAngle);
                newAngle += angle;
                if(newAngle >  M_PI/2.0) newAngle -= M_PI;
                if(newAngle < -M_PI/2.0) newAngle += M_PI;

                length = Exp<REAL>(2.0);
                coordX = pt->x + length*cos(newAngle);
                coordY = pt->y + length*sin(newAngle);
                newPt = new pmf_point<REAL>(coordX, coordY, pt, NULL, length, 0.0, ++ptId, PT_UPDATE);

                pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
                pt->n2 = newPt;
                pt->l2 = length;
            }

            //if(akt->r1!=NULL && akt->r2!=NULL && akt->r1->x<akt->x  &&  akt->r2->x<akt->x)
            if (pt->id == PT_BIRTH_NORMAL)
            {
                // isIDaNeighbor
                /*
                int i1, i2;
                if( (i1 = isIDaNeighbor(akt->r1, id1)) > 0
                    &&  (i2 = isIDaNeighbor(akt->r2, id2)) > 0 )
                {
                    if(i1 == 1)  akt->r1->r1 = akt;
                    else if(i1 == 2)  akt->r1->r2 = akt;
                    else fprintf(stderr, "&1");

                    if(i2 == 1)  akt->r2->r1 = akt;
                    else if(i2 == 2)  akt->r2->r2 = akt;
                    else fprintf(stderr, "&2");
                }
                else
                    if( (i1 = isIDaNeighbor(akt->r1, id2)) > 0
                        &&  (i2 = isIDaNeighbor(akt->r2, id1)) > 0)
                    {
                    if(i1 == 1)  akt->r1->r1 = akt;
                    else if(i1 == 2)  akt->r1->r2 = akt;
                    else fprintf(stderr, "&1");

                    if(i2 == 1)  akt->r2->r1 = akt;
                    else if(i2 == 2)  akt->r2->r2 = akt;
                    else fprintf(stderr, "&2");
                    }
                    else
                    fprintf(stderr, "#");

                delPathS(akt, id1, qB, qI, &idPktu, Bord);
                delPathS(akt, id2, qB, qI, &idPktu, Bord);
                *
            }
            // TODO :
            /*
        qB->remove(akt->id);
        qI->remove(akt->id);
            *
            ;
        }
    }
// */
    return;
}
#undef PT_LT

#undef REAL

/*

int isIDaNeighbor(struct Tpoint *pt, long id)
{
    if(pt->r1 != NULL  &&  pt->r1->id == id) return(1);
    else if (pt->r2 != NULL  &&  pt->r2->id == id)  return(2);
    else return(0);
}


void storePoints2(struct Tpoint *newPt, listB *qB, listI *qI, long *idPktu, struct borderPoint * Bord)
{
    struct TlistBElement *tmpW;
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

    if(res != NULL) storePoints2(res, qB, qI, idPktu, Bord);
    return(res);
}


#define ABS(x) (((x) > 0) ? (x) : (-(x)))

bool addPoint(PMF *Hist, PMF *newHist, float x, float y, int zap, int PointThre, struct borderPoint * Bord)
{

    long qq = 0;
    restore = false;
    changed = 0;
    while( !qB->empty() || !qI->empty() )
    {
	akt = doGet( qB, qI, &id1, &id2 );
	newHist->addPoint(akt);
	if( akt->id > oldSize )
	    if( (++changed) > PointThre )
	    {
		restore = true;
		break;
	    }

	if( akt->id <= oldSize )
	{
	    if(akt->r1 != NULL  &&  akt->r2 == NULL  &&  isInsideBorder(akt->x,akt->y, Bord) == 0
		    &&  isOnBorder(akt->x,akt->y,Bord) < 0
	      )
	    {
		if(ABS(akt->x - akt->r1->x) < EPSILON) {
		    //* fprintf(stderr, "\n [OLD] UPSY DAIZY!\n");
		    if(akt->y > akt->r1->y)  kat = 0.5*M_PI;
		    else kat = -0.5*M_PI;
		}
		else
		    kat = atan((akt->y - akt->r1->y) / (akt->x - akt->r1->x));
		determineUpdateAngle(&nkat);
		nkat += kat;
		if(nkat > M_PI/2.0) nkat -= M_PI;
		if(nkat < -M_PI/2.0) nkat += M_PI;

		len = E(2);
		zmX = akt->x + len*cos(nkat);
		zmY = akt->y + len*sin(nkat);
		nowy = allocPoint(zmX, zmY, akt, NULL, 0, 0, &idPktu);
		storePoints2(nowy, qB, qI, &idPktu, Bord);
		akt->r2 = nowy;
	    }
	}
	else {
	    if(akt->r1 != NULL  &&  akt->r2 == NULL  &&  isInsideBorder(akt->x,akt->y,Bord) == 0
		    &&  isOnBorder(akt->x,akt->y,Bord) < 0
	      )
	    {
		if(ABS(akt->x - akt->r1->x) < EPSILON) {
		    if(akt->y > akt->r1->y)  kat = 0.5*M_PI;
		    else kat = -0.5*M_PI;
		}
		else
		    kat = atan((akt->y - akt->r1->y) / (akt->x - akt->r1->x));
		determineUpdateAngle(&nkat);
		nkat += kat;
		if(nkat > M_PI/2.0) nkat -= M_PI;
		if(nkat < -M_PI/2.0) nkat+= M_PI;

		len = E(2);
		zmX = akt->x + len*cos(nkat);
		zmY = akt->y + len*sin(nkat);
		nowy = allocPoint(zmX, zmY, akt, NULL, 0, 0, &idPktu);
		storePoints2(nowy, qB, qI, &idPktu, Bord);
		akt->r2 = nowy;
	    }
	    if(akt->r1 != NULL  &&  akt->r2 != NULL
		    &&  akt->r1->x < akt->x  &&  akt->r2->x < akt->x)
	    {
		// isIDaNeighbor
		int i1, i2;
		if( (i1 = isIDaNeighbor(akt->r1, id1)) > 0
			&&  (i2 = isIDaNeighbor(akt->r2, id2)) > 0 )
		{
		    if(i1 == 1)  akt->r1->r1 = akt;
		    else if(i1 == 2)  akt->r1->r2 = akt;
		    else fprintf(stderr, "&1");

		    if(i2 == 1)  akt->r2->r1 = akt;
		    else if(i2 == 2)  akt->r2->r2 = akt;
		    else fprintf(stderr, "&2");
		}
		else
		    if( (i1 = isIDaNeighbor(akt->r1, id2)) > 0
			    &&  (i2 = isIDaNeighbor(akt->r2, id1)) > 0)
		    {
			if(i1 == 1)  akt->r1->r1 = akt;
			else if(i1 == 2)  akt->r1->r2 = akt;
			else fprintf(stderr, "&1");

			if(i2 == 1)  akt->r2->r1 = akt;
			else if(i2 == 2)  akt->r2->r2 = akt;
			else fprintf(stderr, "&2");
		    }
		    else
			fprintf(stderr, "#");

		delPathS(akt, id1, qB, qI, &idPktu, Bord);
		delPathS(akt, id2, qB, qI, &idPktu, Bord);
	    }
	}
	qB->remove(akt->id);
	qI->remove(akt->id);

	if(zap == 1) {
		fprintf(fstep, "\n\n\nKONFIGURACJA w kroku %li\n\n", qq);
		writeConf(fstep, newHist);
		qB->write(fstep);
		qI->write(fstep);
		fflush(fstep);
	}

	qq++;
	checkPoint(akt, qq);
	//detectSegment(akt, qq);
    }
    qB->free();  qI->free();
    delete(qB);  delete(qI);

       fprintf(stderr, "?");  fflush(stderr);
       struct T_PMFElement *ptr = newHist->giveHead();
       while(ptr)
       {
	       checkPoint(ptr->point, 0);
	       detectSegment(ptr->point, 0);
	       ptr = ptr->next;
       }

    return(restore);
} // end function addPoint
#undef ABS
*/
