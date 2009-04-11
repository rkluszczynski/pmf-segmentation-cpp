#ifndef SEGMENT_HPP_INCLUDED
#define SEGMENT_HPP_INCLUDED

#define REAL double
#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_LE(PP1,PP2,SSIN,CCOS) (! PT_LT(PP2,PP1,SSIN,CCOS))
inline
bool
pmf_store_point_during_segment (
                        pmf_point<REAL> * newPt,
                        BirthsHeap<REAL> * bHeap,
                        IntersectionsHeap<REAL> * iHeap,
                        pmf_point<REAL> * parentPt,
                        long & id,
                        REAL fieldHeight,
                        REAL fieldWidth,
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
        if( cross3<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, 0.0f, fieldHeight) != 0 ) {
            REAL cx, cy;
            crosspoint2<REAL>(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, 0.0f, 0.0f, 0.0f, fieldHeight, cx, cy);
            newPt->x = cx;
            newPt->y = cy;
            newPt->n2 = NULL;
            newPt->type = PT_BORDER;
        }

        for (int i = 0; i < bHeap->size(); i++)
        {
            pmf_point<REAL> * pEl = bHeap->get(i);
            int c1 = -1;
            int c2 = -1;

            if (newPt != pEl->n1  &&  pEl->n1 != NULL  &&
                //pEl->data->n1->x < pEl->data->x  &&
                pEl->n1 != newPt->n1 &&
                //PT_LT(pEl->n1, pEl, sinL, cosL) )
                PT_LE(pEl->n1, pEl, sinL, cosL) )
            {
                c1 = cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y);
                if (c1 == 1)
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y, xx, yy);

                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n1, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n1->l1 : pEl->n1->l2;

                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n1, ll1, ll2, ++id, PT_INTERSECTION);

                    iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                    out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                }
                if (c1 == 2  &&  pEl->type != PT_INTERSECTION)
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y, xx, yy);

                    if (PT_LT(pEl->n1, pEl, sinL, cosL))
                    {
                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n1, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n1->l1 : pEl->n1->l2;

                        pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n1, ll1, ll2, ++id, PT_INTERSECTION);
                        iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                        out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                    } /*
                    else {
                        if (pEl->n2) {
                            pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n2, 0.0, 0.0, ++id, PT_INTERSECTION);
                            iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                            out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                        }
                    } //*/
                }
                if (c1 == 3)
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n1->x, pEl->n1->y, xx, yy);

                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n1, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n1->l1 : pEl->n1->l2;


                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n1, ll1, ll2, ++id, PT_INTERSECTION);
                    iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                    out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                }
            }

            if (newPt != pEl->n2  &&  pEl->n2 != NULL  &&
                //pEl->data->n2->x < pEl->data->x  &&
                pEl->n1 != newPt->n1 &&
                //PT_LT(pEl->n2, pEl, sinL, cosL) )
                PT_LE(pEl->n2, pEl, sinL, cosL) )
            {
                c2 = cross3(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y);
                if (c2 == 1)
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y, xx, yy);

                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n2, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n2->l1 : pEl->n2->l2;

                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n2, ll1, ll2, ++id, PT_INTERSECTION);

                    iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                    out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                }
                if (c2 == 2  &&  pEl->type != PT_INTERSECTION)
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y, xx, yy);

                    if (PT_LT(pEl->n2, pEl, sinL, cosL))
                    {
                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n2, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n2->l1 : pEl->n2->l2;

                        pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n2, ll1, ll2, ++id, PT_INTERSECTION);
                        iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                        out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                    } /*
                    else {
                        if (pEl->n1) {
                            pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n1, 0.0, 0.0, ++id, PT_INTERSECTION);
                            iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                            out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                        }
                    } //*/
                }
                if (c2 == 3)
                {
                    REAL xx, yy;
                    crosspoint2(newPt->x, newPt->y, newPt->n1->x, newPt->n1->y, pEl->x, pEl->y, pEl->n2->x, pEl->n2->y, xx, yy);

                        REAL ll1 = newPt->l1;

                        int ll2ans = pmf_which_neighbor_is_id(pEl->n2, pEl->id);
                        assert(ll2ans == 1  ||  ll2ans == 2);
                        REAL ll2 = (ll2ans == 1) ? pEl->n2->l1 : pEl->n2->l2;


                    pmf_point<REAL> * newpt2 = new pmf_point<REAL>(xx, yy, parentPt, pEl->n2, ll1, ll2, ++id, PT_INTERSECTION);
                    iHeap->insert (newpt2, newPt->id, pEl->id);
#if pmf_LOG_ADD
                    out << " -- CROSS -- : " << *newpt2 << "  :: " << newPt->id << " , " << pEl->id << std::endl;
#endif
                }

            }
#if pmf_LOG_ADD
            if (c1 > 1)  out << " -- ----- -- :  c1=" << c1 << "  :: " << *pEl << " ~ " << *pEl->n1 << std::endl;
            if (c2 > 1)  out << " -- ----- -- :  c2=" << c2 << "  :: " << *pEl << " ~ " << *pEl->n2 << std::endl;
#endif
        }
    }
    /* Adding point to the list */
#if pmf_LOG_ADD
    out << " --- INS --- : " << *newPt << std::endl;
#endif
    //bHeap->insert(newPt, blocks);
    //*
    bHeap->insert(newPt);
    //*/
    return true;
}
#undef REAL
#undef X_ROTATED
#undef PT_LT
#undef PT_LE


#define PT_DIST2(PT1, PT2) (((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y))
template <class T_REAL>
inline
void
PMF<T_REAL> :: SetPerpendicularNeighbor (
                            BirthsHeap<T_REAL> * bbHeap,
                            IntersectionsHeap<T_REAL> * iiHeap,
                            pmf_point<T_REAL> * npt, // new point
                            pmf_point<T_REAL> * ppt, // parent point
                            long &ptId,
                            EdgePoints<T_REAL> * ep,
                            T_REAL ssinL,
                            T_REAL ccosL
                        )
{
    T_REAL fHeight = pmfConf->get_field_height();
    T_REAL  fWidth = pmfConf->get_field_width();

    IntersectionsHeap<T_REAL> * tmp_iHeap = new IntersectionsHeap<T_REAL> (ssinL, ccosL);

    pmf_store_point_during_segment(npt, bbHeap, tmp_iHeap, ppt, ptId, fHeight, fWidth, ssinL, ccosL);
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

        bbHeap->remove_point_with_id(npt->id);

#if CHECK_ASSERTIONS
        assert(tmp_iHeap->empty());
        assert(id != npt->id);
        assert(bbHeap->get_point_with_id(id) != NULL);
#endif
        pmf_delete_rotated_path(pt_min, bbHeap->get_point_with_id(id), bbHeap, iiHeap, NULL, ptId, ep, fHeight, fWidth, ssinL, ccosL);
        bbHeap->remove_point_with_id((id == id1) ? id2 : id1);

#if pmf_LOG_ADD
        out << "[ final neighbour ] : " << *pt_min << endl << endl;
#endif
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
PMF<T_REAL> :: AddBirthSegment (T_REAL xx, T_REAL yy, T_REAL alpha, EdgePoints<T_REAL> * ep = NULL)
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
    PrepareEvolution(bHeap, alpha, sinL, cosL, NULL);


    /* ************************************************************************************** */
#if pmf_LOG_ADD
    T_REAL rotxx = X_ROTATED(xx, yy, sinL, cosL);
    T_REAL rotyy = Y_ROTATED(xx, yy, sinL, cosL);
    out << "[  ADD ] : oldSize = " << ptId << "   {" << xx << ";" << yy << "}  : " << rotxx << " , " << rotyy << std::endl;
#endif

    // Creating new point
    pmf_point<T_REAL> * pt = new pmf_point<T_REAL>(xx, yy, NULL, NULL, 0.0, 0.0, ++ptId, PT_BIRTH_NORMAL);

    if (ep != NULL) {
        /// TODO : set the point in edge_element
        int index = ep->getPointIndex(xx, yy);
        assert(index >= 0);
        ep->get(index)->pt = pt;
    }

    while (! bHeap->empty() && PT_LT(bHeap->top(), pt, sinL, cosL))
    {
#if pmf_LOG_ADD
        out << *bHeap->top() << "_" << X_ROTATED(bHeap->top()->x, bHeap->top()->y, sinL, cosL) << std::endl;
#endif
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

    SetPerpendicularNeighbor(bHeap, iHeap, pt1, pt, ptId, ep, sinL, cosL);
    SetPerpendicularNeighbor(bHeap, iHeap, pt2, pt, ptId, ep, sinL, cosL);

#if pmf_LOG_ADD
    out << *pt1 << endl;
    out << *pt2 << endl;
    out << "-----<<" << endl;
    out << iHeap << endl;
#endif

    /* ************************************************************************************** */
    // and the riots start again ...
    EvolveRestOfField(bHeap, iHeap, sinL, cosL, oldSize, ptId, NULL, ep);

    delete bHeap;
    delete iHeap;

    return;
}
#undef PT_LT
#undef Y_ROTATED
#undef X_ROTATED


#endif // SEGMENT_HPP_INCLUDED
