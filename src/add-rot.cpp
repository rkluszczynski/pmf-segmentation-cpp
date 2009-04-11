#include "configurationlist.hpp"
#include "birthslist.hpp"
#include "intersectionslist.hpp"
#include "probability.hpp"
#include "accesslists.hpp"
#include "modifylists.hpp"

#include "birthsheap.hpp"
#include "intersectionsheap.hpp"
#include "modifyheaps.hpp"

#include "rotate.cpp"

#define LOG 1
#define REAL double

/*
#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
inline
pmf_point<REAL> *
pmf_put_new_neighbor (pmf_point<REAL> * ppt, REAL angle, long & ptId, REAL sinL, REAL cosL)
{
    REAL length = Exp<REAL>(2.0);
    //cerr << " LENGTH = " << length << endl;

    REAL rotx = X_ROTATED(ppt->x, ppt->y, sinL, cosL);
    REAL roty = Y_ROTATED(ppt->x, ppt->y, sinL, cosL);
    //cerr << rotx << "  " << roty << endl;

    REAL coordX = rotx + length * cos(angle);
    REAL coordY = roty + length * sin(angle);
    //cerr << coordX << "  " << coordY << endl;

    REAL newX = X_ROTATED(coordX, coordY, -sinL, cosL);
    REAL newY = Y_ROTATED(coordX, coordY, -sinL, cosL);
    //cerr << newX << "  " << newY << endl;

    pmf_point<REAL> * newPt = new pmf_point<REAL>(newX, newY, ppt, NULL, length, 0.0, ++ptId, PT_UPDATE);
    return newPt;
}
//*/

#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_LE(PP1,PP2,SSIN,CCOS) (! PT_LT(PP2,PP1,SSIN,CCOS))
void pmf_add_rotated_point (
            ConfigurationList<REAL> * PMF,
            ConfigurationList<REAL> * newPMF,
            REAL xx,
            REAL yy
        )
{
#if LOG
    FILE * flog = freopen("output/log-add-rot.txt", "w", stdout);
    int iterationCounter = 0;
#endif
    REAL fieldWidth  = PMF->get_field_width();
    REAL fieldHeight = PMF->get_field_height();
    PMF->set_points_ids();

    long oldSize = PMF->get_size() + 1;
    long ptId = oldSize;
    REAL  sinL = 0.0;//-1.0;
    REAL  cosL = 1.0;//0.0;

    REAL alpha = -M_PI_2;
    sinL = sin(alpha);
    cosL = cos(alpha);
    cerr << "[ alfa ] : " << alpha << "  ~  " << radians2degree(alpha) << endl;
    cerr << "[  sin ] : " << sinL << endl;
    cerr << "[  cos ] : " << cosL << endl;
    if (alpha != 0.0) {
        pmf_rotate_point_types(PMF, sinL, cosL);
        cerr << "[ SAVE ] : saving rotated configuration to a file" << endl;
        ofstream fout("output/PMF-R.txt");
        PMF->save_configuration(fout);
        fout.close();
    }

    assert(sinL*sinL + cosL*cosL == 1.0);
    REAL rotxx = X_ROTATED(xx, yy, sinL, cosL);
    REAL rotyy = Y_ROTATED(xx, yy, sinL, cosL);
    cerr << "[  ADD ] : oldSize = " << ptId << "   {" << xx << ";" << yy << "}  : " << rotxx << endl;

    BirthsHeap<REAL> *        bHeap = new BirthsHeap<REAL> (sinL, cosL);
    IntersectionsHeap<REAL> * iHeap = new IntersectionsHeap<REAL> (sinL, cosL);

    pmf_point<REAL> * pt;
    while (! PMF->empty())
    {
        pt = PMF->front();
        PMF->pop_front();
        bHeap->insert(pt);
    }
    //bHeap->remove_point_with_id(8);

    // Creating new point
    pt = new pmf_point<REAL>(xx, yy, NULL, NULL, 0.0, 0.0, ++ptId, PT_BIRTH_NORMAL);
    while (! bHeap->empty() && PT_LT(bHeap->top(), pt, sinL, cosL))
    {
#if LOG
        cout << *bHeap->top() << "_" << X_ROTATED(bHeap->top()->x, bHeap->top()->y, sinL, cosL) << endl;
#endif
        //newPMF->push_in_order(bHeap->extract_min());
        newPMF->push_back(bHeap->extract_min());
    }
    //bHeap->insert(pt);
    newPMF->push_back(pt);
    cerr << bHeap << endl;

    // Determining neighbors of a new point
    REAL lowerAngle, upperAngle;
    pmf_point<REAL> * newPt;

    determineBirthAngles(upperAngle, lowerAngle);

    cerr << *pt << endl;

    newPt = pmf_put_new_neighbor(pt, upperAngle, ptId, sinL, cosL);
    pt->n1 = newPt;
    pt->l1 = newPt->l1;
    // TODO: store point
    //bHeap->insert(newPt);
    pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);

    cerr << *pt->n1 << endl;

    newPt = pmf_put_new_neighbor(pt, lowerAngle, ptId, sinL, cosL);
    pt->n2 = newPt;
    pt->l2 = newPt->l1;
    // TODO: store point
    //bHeap->insert(newPt);
    pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);

    cerr << *pt->n2 << endl;

    /* ************************************************************************************** */
    // and the riots start again ...
    long id1, id2;
    double angle, newAngle;
//*
    while (!bHeap->empty()  ||  !iHeap->empty())
    {
        id1 = id2 = 0;
        pt = pmf_do_heaps_get( bHeap, iHeap, id1, id2, sinL, cosL );
        newPMF->push_back(pt);
#if LOG
        ++iterationCounter;
        cout << " ---------------------------------------------------------------------------" << endl;
        cout << " ------------------------------ STEP " << iterationCounter << "----------------------------------" << endl;
        cout << *pt << "   ::  " << id1 << " , " << id2 << endl;
        cout << bHeap << endl;
        cout << iHeap << endl;
#endif
        cerr << "[ STEP ] : " << iterationCounter << endl;
        //*
        if (pt->id <= oldSize) {
            if (pt->type == PT_UPDATE  &&  pt->n2 == NULL)
            {
                //assert(false == true);
                angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));
                determineUpdateAngle(newAngle);
                newAngle += angle;
                if (newAngle > M_PI_2)  newAngle -= M_PI;
                if (newAngle < -M_PI_2)  newAngle += M_PI;

                newPt = pmf_put_new_neighbor(pt, newAngle, ptId, sinL, cosL);
                // TODO: store point
                pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);

                pt->n2 = newPt;
                pt->l2 = 17.17; //TODO
            }
        }
        else {
            if (pt->type == PT_UPDATE)
            {
                angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));
                determineUpdateAngle<REAL>(newAngle);
                newAngle += angle;
                if (newAngle > M_PI_2)  newAngle -= M_PI;
                if (newAngle < -M_PI_2)  newAngle += M_PI;

                newPt = pmf_put_new_neighbor(pt, newAngle, ptId, sinL, cosL);
                // TODO: store point
                pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);

                pt->n2 = newPt;
                pt->l2 = 17.17; //TODO
            }
            else if (pt->type == PT_INTERSECTION)
            {
                cerr << " INTERSECTION :: " << *pt << "  ~  " << id1 << "  " << id2 << endl;
                pmf_correct_new_intersection_point(pt, id1, id2);
                //assert(false == true);
                assert(bHeap->get_point_with_id(id1) != NULL);
                assert(bHeap->get_point_with_id(id2) != NULL);
                ;
                pmf_delete_rotated_path(pt, bHeap->get_point_with_id(id1), bHeap, iHeap, NULL, ptId, fieldHeight, fieldWidth, sinL, cosL);
                pmf_delete_rotated_path(pt, bHeap->get_point_with_id(id2), bHeap, iHeap, NULL, ptId, fieldHeight, fieldWidth, sinL, cosL);


            bHeap->remove_point_with_id(pt->id);
            iHeap->remove_intersections_with_id(pt->id);
            }
        }
        //*/
    }

    /* ************************************************************************************** */
    /* ************************************************************************************** */
#if 0
    /* Test of IntersectionHeap */
    cerr << " TESTING  ...  'IntersectionsHeap' ... " << endl;
    IntersectionsHeap<REAL> * iiHeap = new IntersectionsHeap<REAL> (sinL, cosL);
    pmf_point<REAL> * ppt = new pmf_point<REAL>(1.0, 2.0, NULL, NULL, 0.0, 0.0, 171717, 17);
    long ii1 = 7, ii2 = 117;
    long ti1, ti2;
    iiHeap->insert(ppt, ii1, ii2);
    cerr << " TESTING  ...   top =  " << *iiHeap->top() << endl;
    cerr << " TESTING  ...  " << *iiHeap->extract_min(ti1, ti2) << endl;
    cerr << " TESTING  ...   intersection of " << ti1 << " and " << ti2 << endl;
    delete iiHeap;
    cerr << " TESTING  ...  done" << endl;
#endif

    return;
}
#undef Y_ROTATED
#undef X_ROTATED
#undef LOG

#undef REAL
