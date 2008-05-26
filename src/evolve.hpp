#ifndef EVOLVE_HPP_INCLUDED
#define EVOLVE_HPP_INCLUDED


template <class T_REAL>
inline
void
PMF<T_REAL> :: EvolveRestOfField (
                            BirthsHeap<T_REAL> * bHeap,
                            IntersectionsHeap<T_REAL> * iHeap,
                            T_REAL sinL,
                            T_REAL cosL,
                            long oldSize,
                            long & ptId
                        )
{

    long id1, id2;
    double angle, newAngle;
//*
    while (!bHeap->empty()  ||  !iHeap->empty())
    {
        id1 = id2 = 0;
        pmf_point<T_REAL> * pt = pmf_do_heaps_get( bHeap, iHeap, id1, id2, sinL, cosL );
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

                pmf_point<T_REAL> * newPt = pmf_put_new_neighbor(pt, newAngle, ptId, sinL, cosL);
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

                pmf_point<T_REAL> * newPt = pmf_put_new_neighbor(pt, newAngle, ptId, sinL, cosL);
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

}


#endif // EVOLVE_HPP_INCLUDED
