#ifndef EVOLVE_HPP_INCLUDED
#define EVOLVE_HPP_INCLUDED

//*
#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
template <class T_REAL>
inline
pmf_point<T_REAL> *
pmf_put_new_neighbor (pmf_point<T_REAL> * ppt, T_REAL angle, long & ptId, T_REAL sinL, T_REAL cosL, T_REAL llength = 0.0)
{
    T_REAL length = (llength == 0.0) ? Exp<REAL>(2.0) : llength;
    //cerr << " LENGTH = " << length << endl;

    T_REAL rotx = X_ROTATED(ppt->x, ppt->y, sinL, cosL);
    T_REAL roty = Y_ROTATED(ppt->x, ppt->y, sinL, cosL);
    //cerr << rotx << "  " << roty << endl;

    T_REAL coordX = rotx + length * cos(angle);
    T_REAL coordY = roty + length * sin(angle);
    //cerr << coordX << "  " << coordY << endl;

    T_REAL newX = X_ROTATED(coordX, coordY, -sinL, cosL);
    T_REAL newY = Y_ROTATED(coordX, coordY, -sinL, cosL);
    //cerr << newX << "  " << newY << endl;

    pmf_point<REAL> * newPt = new pmf_point<REAL>(newX, newY, ppt, NULL, length, 0.0, ++ptId, PT_UPDATE);
    return newPt;
}
#undef Y_ROTATED
#undef X_ROTATED
//*/

template <class T_REAL>
inline
void
PMF<T_REAL> :: EvolveRestOfField (
                            BirthsHeap<T_REAL> * bHeap,
                            IntersectionsHeap<T_REAL> * iHeap,
                            T_REAL sinL,
                            T_REAL cosL,
                            long oldSize,
                            long & ptId,
                            BlocksLists<T_REAL> * blocks = NULL,
                            EdgePoints<T_REAL> * ep = NULL
                        )
{
    // Do the evolution!
#if pmf_LOG_ADD
//#ifdef pmf_LOG_ADD_FILENAME
//    ofstream fout(pmf_LOG_ADD_FILENAME);
//    ostream out(fout.rdbuf());
//#else
//    ostream out(std::cout.rdbuf());
//#endif
    int iterationCounter = 0;
#endif
    long id1, id2;
    double angle, newAngle;
//*
    while (!bHeap->empty()  ||  !iHeap->empty())
    {
        id1 = id2 = 0;
        pmf_point<T_REAL> * pt = pmf_do_heaps_get( bHeap, iHeap, id1, id2, sinL, cosL );
        pmfConf->push_back(pt);
#if pmf_LOG_ADD
        if (saveOp)
        {
            ++iterationCounter;
            out << " ---------------------------------------------------------------------------" << std::endl;
            out << " ------------------------------ STEP " << iterationCounter << "----------------------------------" << std::endl;
            out << *pt << "   ::  " << id1 << " , " << id2 << std::endl;
            out << bHeap << std::endl;
            out << iHeap << std::endl;
            out << "[ STEP ] : " << iterationCounter << std::endl;
        }
#endif
        //*
        if (pt->id <= oldSize) {
            if (pt->type == PT_UPDATE  &&  pt->n2 == NULL)
            {
#if CHECK_ASSERTIONS
                //assert(false == true);
/// TODO (Rafal#1#): correct assertions and neighbours' lengths
                assert(pt->l1 > 0.0);
                assert(pt->l2 >= 0.0);
#endif
                if (pt->x == pt->n1->x) angle = 0.0;
                else angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));

                determineUpdateAngle(newAngle);
                newAngle += angle;
                if (newAngle > M_PI_2)  newAngle -= M_PI;
                if (newAngle < -M_PI_2)  newAngle += M_PI;

/// TODO (Rafal#1#): put new neighbor based on the length of point
                assert(pt->l2 > 0.0);

                pmf_point<T_REAL> * newPt = pmf_put_new_neighbor(pt, newAngle, ptId, sinL, cosL, pt->l2);
                ///pmf_point<T_REAL> * newPt = pt->pmf_put_new_neighbor(2, newAngle, ptId, sinL, cosL, pt->l2);
                pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, blocks, sinL, cosL);

                pt->n2 = newPt;
                if (pt->l2 == 0.0) { pt->l2 = newPt->l1; cerr << endl << "BENG !!!" << endl << endl; }
                assert(pt->l2 == newPt->l1);
            }
            TestVirtualLengthsOfPoint(pt);
        }
        else {
            if (pt->type == PT_UPDATE)
            {
                if (pt->x == pt->n1->x) angle = 0.0;
                else angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));
                //angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));

                determineUpdateAngle<T_REAL>(newAngle);
                newAngle += angle;
                if (newAngle > M_PI_2)  newAngle -= M_PI;
                if (newAngle < -M_PI_2)  newAngle += M_PI;

                pmf_point<T_REAL> * newPt = pmf_put_new_neighbor(pt, newAngle, ptId, sinL, cosL);
                ///pmf_point<T_REAL> * newPt = pt->pmf_put_new_neighbor(2, newAngle, ptId, sinL, cosL);
                pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, blocks, sinL, cosL);

                pt->n2 = newPt;
                pt->l2 = newPt->l1;

                TestVirtualLengthsOfPoint(pt);
            }
            else if (pt->type == PT_INTERSECTION)
            {
#if pmf_LOG_ADD
                if (saveOp)
                    out << " INTERSECTION :: " << *pt << "  ~  " << id1 << "  " << id2 << std::endl;
#endif
                pmf_correct_new_intersection_point(pt, id1, id2);
                //assert(false == true);
#if CHECK_ASSERTIONS
                assert(bHeap->get_point_with_id(id1) != NULL);
                assert(bHeap->get_point_with_id(id2) != NULL);
#endif
                pmf_delete_rotated_path(pt, bHeap->get_point_with_id(id1), bHeap, iHeap, blocks, ptId, ep, fieldHeight, fieldWidth, sinL, cosL);
                pmf_delete_rotated_path(pt, bHeap->get_point_with_id(id2), bHeap, iHeap, blocks, ptId, ep, fieldHeight, fieldWidth, sinL, cosL);

                bHeap->remove_point_with_id(pt->id, blocks);
                iHeap->remove_intersections_with_id(pt->id, blocks);

            }
        }
        //*/
        //TestVirtualLengthsOfPoint(pt);
        //if (! TestVirtualLengthsOfPoint(pt)) assert(false);
    }
    /* Renumbering points' IDs starting from 1. */
    assert(! BorderArtefactsRemover());
    pmfConf->set_points_ids();
}


#endif // EVOLVE_HPP_INCLUDED
