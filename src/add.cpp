
#include "configurationlist.hpp"
#include "birthslist.hpp"
#include "intersectionslist.hpp"
#include "probability.hpp"
#include "accesslists.hpp"
#include "modifylists.hpp"

#define LOG 1
#define REAL double

//#define TYLKO_KOPIUJ
#define PT_LT(X, Y) ((X) < (Y))
void pmf_add_point (
            ConfigurationList<REAL> * PMF,
            ConfigurationList<REAL> * newPMF,
            REAL xx,
            REAL yy
        )
{
    using std::cout;
    using std::cerr;
    using std::endl;
#ifdef TYLKO_KOPIUJ
    while (! PMF->empty()) { newPMF->push_back(PMF->front()); PMF->pop_front(); }
    return;
#endif
    REAL fieldWidth  = PMF->get_field_width();
    REAL fieldHeight = PMF->get_field_height();
    PMF->set_points_ids();

    long oldSize = PMF->get_size() + 1;
    long ptId = oldSize;
    pmf_point<REAL> * pt;
    while ((pt = PMF->front()) != NULL  &&  PT_LT(pt->x, xx))
    {
        PMF->pop_front();
        newPMF->push_back(pt);
    }
    cerr << "[  ADD ] : oldSize = " << ptId << endl;

    pt = new pmf_point<REAL>(xx, yy, NULL, NULL, 0.0, 0.0, ++ptId, PT_BIRTH_NORMAL);
    newPMF->push_back(pt);

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
    //pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
    pmf_store_modified_points_in_blocks (newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
    pt->n1 = newPt;
    pt->l1 = newPt->l1;

    length = Exp<REAL>(2.0);
    coordX = pt->x + length * cos(lowerAngle);
    coordY = pt->y + length * sin(lowerAngle);
    newPt = new pmf_point<REAL>(coordX, coordY, pt, NULL, length, 0.0, ++ptId, PT_UPDATE);
    //pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
    pmf_store_modified_points_in_blocks (newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
    pt->n2 = newPt;
    pt->l2 = newPt->l1;

    // and the riots start ...
    long id1, id2;
    double angle, newAngle;

#if LOG
    FILE * flog = freopen("output/log2.txt", "w", stdout);
    int iterationCounter = 0;
#endif
    while( !birthList->empty() || !crossList->empty() )
    {
        pt = pmf_do_get( birthList, crossList, id1, id2 );
        newPMF->push_back(pt);
#if LOG
        cout << " ---------------------------------------------------------------------------" << endl;
        cout << " ------------------------------ STEP " << ++iterationCounter << "----------------------------------" << endl;
        cout << "  pt = " << (*pt) << "    [" << id1 << ";" << id2 << "] " << endl;
        cout << birthList << endl;
        cout << crossList << endl;
        if (true) {
            cout << endl << "### Lista punktow w bloku ###" << endl;
            if (blocksLists)  blocksLists->print_lists();
            cout << endl << "### Atualne PMF ###" << endl;
            cout << newPMF << endl;
        }
#endif

        if( pt->id <= oldSize )
        {
            /* if(akt->r1 != NULL  &&  akt->r2 == NULL  &&  isInsideBorder(akt->x,akt->y, Bord) == 0
                    &&  isOnBorder(akt->x,akt->y,Bord) < 0
             */
            if (pt->type == PT_UPDATE  &&  pt->n2 == NULL)
            //if(pt->n1 != NULL  &&  pt->n2 == NULL)
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

                //pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
                pmf_store_modified_points_in_blocks (newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
                pt->n2 = newPt;
                pt->l2 = length;
            }
        }
        else {
            if (pt->type == PT_UPDATE)
//            if (pt->n1 != NULL  &&  pt->n2 == NULL)
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

                //pmf_store_points_in_blocks(newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
                pmf_store_modified_points_in_blocks (newPt, birthList, crossList, pt, ptId, fieldHeight, fieldWidth, blocksLists);
                pt->n2 = newPt;
                pt->l2 = length;
            }
            //if(akt->r1!=NULL && akt->r2!=NULL && akt->r1->x<akt->x  &&  akt->r2->x<akt->x)
            if (pt->type == PT_INTERSECTION)
            {
#if LOG
            cout << "INTERSECTION  : " << (*pt) << "  " << endl;
            cout << "INTERSECTION  : " << *(pt->n1) << "  " << endl;
            cout << "INTERSECTION  : " << *(pt->n2) << "  " << endl;
#endif
                pmf_correct_new_intersection_point(pt, id1, id2);
                assert(birthList->get_point_with_id(id1) != NULL);
                assert(birthList->get_point_with_id(id2) != NULL);
                pmf_delete_path(pt, birthList->get_point_with_id(id1), birthList, crossList, blocksLists, ptId, fieldHeight, fieldWidth);
                pmf_delete_path(pt, birthList->get_point_with_id(id2), birthList, crossList, blocksLists, ptId, fieldHeight, fieldWidth);
            }

            birthList->remove_point_with_id (pt->id, blocksLists);
            crossList->remove_intersection_with_id (pt->id, blocksLists);
        }
    }
    delete birthList;
    delete crossList;
    if (blocksLists) delete blocksLists;
#if LOG
    //fclose(flog);
#endif
    return;
}
#undef PT_LT

#undef REAL
/*
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
