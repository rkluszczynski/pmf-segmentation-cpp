#ifndef SEGMENT_HPP_INCLUDED
#define SEGMENT_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_DIST(PT1, PT2) (((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y))
template <class T_REAL>
void
PMF<T_REAL> :: AddBirthSegment (T_REAL xx, T_REAL yy, T_REAL alpha)
{
    T_REAL fieldWidth  = pmfConf->get_field_width();
    T_REAL fieldHeight = pmfConf->get_field_height();

    long oldSize = pmfConf->get_size() + 1;
    long ptId = oldSize;

    T_REAL sinL = sin(alpha);
    T_REAL cosL = cos(alpha);

    BirthsHeap<T_REAL> *        bHeap = new BirthsHeap<T_REAL> (sinL, cosL);
    IntersectionsHeap<T_REAL> * iHeap = new IntersectionsHeap<T_REAL> (sinL, cosL);
    /// TODO : BlocksLists<T_REAL> *      blocks = NULL;

    PrepareEvolution(bHeap, alpha, sinL, cosL);


    /* ************************************************************************************** */
    RotatePointTypes(sinL, cosL);

#if pmf_LOG_ADD
    T_REAL rotxx = X_ROTATED(xx, yy, sinL, cosL);
    T_REAL rotyy = Y_ROTATED(xx, yy, sinL, cosL);
    out << "[  ADD ] : oldSize = " << ptId << "   {" << xx << ";" << yy << "}  : " << rotxx << " , " << rotyy << std::endl;
#endif

    // Creating new point
    pmf_point<T_REAL> * pt = new pmf_point<T_REAL>(xx, yy, NULL, NULL, 0.0, 0.0, ++ptId, PT_BIRTH_NORMAL);
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
    pmf_point<T_REAL> * pt1 = pmf_put_new_neighbor<T_REAL>(pt, -M_PI_2, ptId, sinL, cosL);
    pmf_point<T_REAL> * pt2 = pmf_put_new_neighbor<T_REAL>(pt,  M_PI_2, ptId, sinL, cosL);
    pt->n1 = pt1;
    pt->n2 = pt2;
    pt->l1 = pt1->l1;
    pt->l2 = pt2->l1;
#if pmf_LOG_ADD
    out << *pt << endl;
    out << *pt1 << endl;
    out << *pt2 << endl;
    out << "----<" << endl;
#endif

    pmf_store_rotated_point_in_blocks(pt1, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);
    if (iHeap->size() > 0)
    {
        out << "PRZECIECIE 1" << endl;
        out << iHeap << endl;
        long id1, id2;
        pmf_point<T_REAL> * pt_min = iHeap->extract_min(id1, id2);
        T_REAL dist_min = PT_DIST(pt, pt_min);
        while (! iHeap->empty())
        {
            if (PT_DIST(pt, iHeap->top()) < dist_min)
            {
                dist_min = PT_DIST(pt, iHeap->top());
                delete pt_min;
                pt_min = iHeap->extract_min(id1, id2);
            }
            else {
                long tmp1, tmp2;
                delete iHeap->extract_min(tmp1, tmp2);
            }
        }

        pmf_correct_new_intersection_point(pt_min, id1, id2);
        long id = (id1 == pt1->id) ? id2 : id1;

        out << "PRZECIECIA : " << id << endl;
        out << iHeap << endl;
#if CHECK_ASSERTIONS
        assert(iHeap->empty());
        assert(id != pt1->id);
        assert(bHeap->get_point_with_id(id) != NULL);
#endif
        pmf_delete_rotated_path(pt_min, bHeap->get_point_with_id(id), bHeap, iHeap, NULL, ptId, fieldHeight, fieldWidth, sinL, cosL);
        bHeap->remove_point_with_id((id == id1) ? id2 : id1);

        pmfConf->push_back(pt_min);
    }
    pmf_store_rotated_point_in_blocks(pt2, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, NULL, sinL, cosL);
    if (iHeap->size() > 0)
    {
        /// TODO
        ;
    }

#if pmf_LOG_ADD
    out << *pt1 << endl;
    out << *pt2 << endl;
    out << " --- " << endl;
    out << iHeap << endl;
#endif


    /* ************************************************************************************** */
    // and the riots start again ...
    EvolveRestOfField(bHeap, iHeap, sinL, cosL, oldSize, ptId);

    delete bHeap;
    delete iHeap;

    return;
}
#undef PT_DIST
#undef PT_LT
#undef Y_ROTATED
#undef X_ROTATED


#endif // SEGMENT_HPP_INCLUDED
