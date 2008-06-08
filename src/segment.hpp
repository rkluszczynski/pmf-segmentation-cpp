#ifndef SEGMENT_HPP_INCLUDED
#define SEGMENT_HPP_INCLUDED


#define PT_DIST2(PT1, PT2) (((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y))
template <class T_REAL>
inline
void
PMF<T_REAL> :: SetPerpendicularNeighbor (
                            BirthsHeap<T_REAL> * bbHeap,
                            IntersectionsHeap<T_REAL> * iiHeap,
                            pmf_point<T_REAL> * npt,
                            pmf_point<T_REAL> * ppt,
                            long &ptId,
                            T_REAL ssinL,
                            T_REAL ccosL
                        )
{
    T_REAL fHeight = pmfConf->get_field_height();
    T_REAL  fWidth = pmfConf->get_field_width();

    IntersectionsHeap<T_REAL> * tmp_iHeap = new IntersectionsHeap<T_REAL> (ssinL, ccosL);

    pmf_store_rotated_point_in_blocks(npt, bbHeap, tmp_iHeap, ppt, ptId, fHeight, fWidth, NULL, ssinL, ccosL);
    if (! tmp_iHeap->empty())
    {
#if pmf_LOG_ADD
        out << "PRZECIECIE : " << *npt << endl;
        out << tmp_iHeap << endl;
#endif
        long id1, id2;
        pmf_point<T_REAL> * pt_min = tmp_iHeap->extract_min(id1, id2);
        T_REAL dist_min = PT_DIST2(ppt, pt_min);
        while (! tmp_iHeap->empty())
        {
            if (PT_DIST2(ppt, tmp_iHeap->top()) < dist_min)
            {
                dist_min = PT_DIST2(ppt, tmp_iHeap->top());
                delete pt_min;
                pt_min = tmp_iHeap->extract_min(id1, id2);
            }
            else {
                long tmp1, tmp2;
                delete tmp_iHeap->extract_min(tmp1, tmp2);
            }
        }
        pmf_correct_new_intersection_point(pt_min, id1, id2);
        long id = (id1 == npt->id) ? id2 : id1;
#if pmf_LOG_ADD
        out << "PRZECIECIA : " << id << endl;
        out << tmp_iHeap << endl;
#endif
#if CHECK_ASSERTIONS
        assert(tmp_iHeap->empty());
        assert(id != npt->id);
        assert(bbHeap->get_point_with_id(id) != NULL);
#endif
        pmf_delete_rotated_path(pt_min, bbHeap->get_point_with_id(id), bbHeap, iiHeap, NULL, ptId, fHeight, fWidth, ssinL, ccosL);
        bbHeap->remove_point_with_id((id == id1) ? id2 : id1);

        pmfConf->push_back(pt_min);
    }
    delete tmp_iHeap;
}
#undef PT_DIST2


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
template <class T_REAL>
void
PMF<T_REAL> :: AddBirthSegment (T_REAL xx, T_REAL yy, T_REAL alpha)
{
    //T_REAL fieldWidth  = pmfConf->get_field_width();
    //T_REAL fieldHeight = pmfConf->get_field_height();

    long oldSize = pmfConf->get_size() + 1;
    long ptId = oldSize;

    T_REAL sinL = sin(alpha);
    T_REAL cosL = cos(alpha);

    BirthsHeap<T_REAL> *        bHeap = new BirthsHeap<T_REAL> (sinL, cosL);
    IntersectionsHeap<T_REAL> * iHeap = new IntersectionsHeap<T_REAL> (sinL, cosL);
    /// TODO : BlocksLists<T_REAL> *      blocks = NULL;

    PrepareEvolution(bHeap, alpha, sinL, cosL);


    /* ************************************************************************************** */
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

    SetPerpendicularNeighbor(bHeap, iHeap, pt1, pt, ptId, sinL, cosL);
    SetPerpendicularNeighbor(bHeap, iHeap, pt2, pt, ptId, sinL, cosL);

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
#undef PT_LT
#undef Y_ROTATED
#undef X_ROTATED


#endif // SEGMENT_HPP_INCLUDED
