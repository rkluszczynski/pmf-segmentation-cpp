#ifndef ADDING_HPP_INCLUDED
#define ADDING_HPP_INCLUDED

#include "modifyheaps.hpp"

#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
#define REAL double
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
#undef REAL


#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_LE(PP1,PP2,SSIN,CCOS) (! PT_LT(PP2,PP1,SSIN,CCOS))
template <class T_REAL>
void
PMF<T_REAL> :: AddBirthPoint(T_REAL xx, T_REAL yy, T_REAL alpha = 0.0)
{
#if pmf_LOG_ADD
//#ifdef pmf_LOG_ADD_FILENAME
//    ofstream fout(pmf_LOG_ADD_FILENAME);
//    ostream out(fout.rdbuf());
//#else
//    ostream out(std::cout.rdbuf());
//#endif
    int iterationCounter = 0;
#endif
    T_REAL fieldWidth  = pmfConf->get_field_width();
    T_REAL fieldHeight = pmfConf->get_field_height();
    pmfConf->set_points_ids();

    long oldSize = pmfConf->get_size() + 1;
    long ptId = oldSize;

    T_REAL sinL = sin(alpha);
    T_REAL cosL = cos(alpha);
#if pmf_LOG_ADD
    out << "[ alfa ] : " << alpha << "  ~  " << radians2degree(alpha) << std::endl;
    out << "[  sin ] : " << sinL << std::endl;
    out << "[  cos ] : " << cosL << std::endl;
#endif
    if (alpha != 0.0) {
        //pmf_rotate_point_types(pmfConf, sinL, cosL);
        RotatePointTypes(sinL, cosL);
#if pmf_LOG_ADD
        out << "[ SAVE ] : saving rotated configuration to a file" << std::endl;
        SaveConfiguration("output/PMF-R.txt");
#endif
    }
#if CHECK_ASSERTIONS
    //assert(sinL*sinL + cosL*cosL == 1.0);
    assert( abs(sinL*sinL + cosL*cosL - 1.0) < EPSILON );
#endif
    T_REAL rotxx = X_ROTATED(xx, yy, sinL, cosL);
    T_REAL rotyy = Y_ROTATED(xx, yy, sinL, cosL);
#if pmf_LOG_ADD
    out << "[  ADD ] : oldSize = " << ptId << "   {" << xx << ";" << yy << "}  : " << rotxx << " , " << rotyy << std::endl;
#endif
    BirthsHeap<T_REAL> *        bHeap = new BirthsHeap<T_REAL> (sinL, cosL);
    IntersectionsHeap<T_REAL> * iHeap = new IntersectionsHeap<T_REAL> (sinL, cosL);

    pmf_point<T_REAL> * pt;
    while (! pmfConf->empty())
    {
        pt = pmfConf->front();
        pmfConf->pop_front();
        bHeap->insert(pt);
    }
    //bHeap->remove_point_with_id(8);

    // Creating new point
    pt = new pmf_point<T_REAL>(xx, yy, NULL, NULL, 0.0, 0.0, ++ptId, PT_BIRTH_NORMAL);
    while (! bHeap->empty() && PT_LT(bHeap->top(), pt, sinL, cosL))
    {
#if pmf_LOG_ADD
        out << *bHeap->top() << "_" << X_ROTATED(bHeap->top()->x, bHeap->top()->y, sinL, cosL) << std::endl;
#endif
        //newPMF->push_in_order(bHeap->extract_min());
        pmfConf->push_back(bHeap->extract_min());
    }
    pmfConf->push_back(pt);
#if pmf_LOG_ADD
    out << bHeap << std::endl;
#endif

    // Determining neighbors of a new point
    T_REAL lowerAngle, upperAngle;
    pmf_point<T_REAL> * newPt;

    determineBirthAngles(upperAngle, lowerAngle);
#if pmf_LOG_ADD
    out << *pt << endl;
#endif
    newPt = pmf_put_new_neighbor(pt, upperAngle, ptId, sinL, cosL);
    pt->n1 = newPt;
    pt->l1 = newPt->l1;
    //bHeap->insert(newPt);
    pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);
#if pmf_LOG_ADD
    out << *pt->n1 << endl;
#endif
    newPt = pmf_put_new_neighbor(pt, lowerAngle, ptId, sinL, cosL);
    pt->n2 = newPt;
    pt->l2 = newPt->l1;
    //bHeap->insert(newPt);
    pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);
#if pmf_LOG_ADD
    out << *pt->n2 << endl;
#endif
    /* ************************************************************************************** */
    // and the riots start again ...
    long id1, id2;
    double angle, newAngle;
//*
    while (!bHeap->empty()  ||  !iHeap->empty())
    {
        id1 = id2 = 0;
        pt = pmf_do_heaps_get( bHeap, iHeap, id1, id2, sinL, cosL );
        pmfConf->push_back(pt);
#if pmf_LOG_ADD
        ++iterationCounter;
        out << " ---------------------------------------------------------------------------" << std::endl;
        out << " ------------------------------ STEP " << iterationCounter << "----------------------------------" << std::endl;
        out << *pt << "   ::  " << id1 << " , " << id2 << std::endl;
        out << bHeap << std::endl;
        out << iHeap << std::endl;
        out << "[ STEP ] : " << iterationCounter << std::endl;
#endif
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
                determineUpdateAngle<T_REAL>(newAngle);
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
#if pmf_LOG_ADD
                out << " INTERSECTION :: " << *pt << "  ~  " << id1 << "  " << id2 << std::endl;
#endif
                pmf_correct_new_intersection_point(pt, id1, id2);
                //assert(false == true);
#if CHECK_ASSERTIONS
                assert(bHeap->get_point_with_id(id1) != NULL);
                assert(bHeap->get_point_with_id(id2) != NULL);
#endif
                pmf_delete_rotated_path(pt, bHeap->get_point_with_id(id1), bHeap, iHeap, NULL, ptId, fieldHeight, fieldWidth, sinL, cosL);
                pmf_delete_rotated_path(pt, bHeap->get_point_with_id(id2), bHeap, iHeap, NULL, ptId, fieldHeight, fieldWidth, sinL, cosL);

            bHeap->remove_point_with_id(pt->id);
            iHeap->remove_intersections_with_id(pt->id);
            }
        }
        //*/
    }
    delete bHeap;
    delete iHeap;

//#if pmf_LOG_ADD
//	if (out.rdbuf() != std::cout.rdbuf()) { ((ofstream *)&out)->close(); }
//#endif
    return;
}
#undef PT_LE
#undef PT_LT
#undef Y_ROTATED
#undef X_ROTATED


#endif // ADDING_HPP_INCLUDED
