#ifndef UPDATE_HPP_INCLUDED
#define UPDATE_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
template <class T_REAL>
void
PMF<T_REAL> :: UpdatePointVelocity (long id, T_REAL alpha = 0.0)
{
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
    BirthsHeap<T_REAL> *        bHeap = new BirthsHeap<T_REAL> (sinL, cosL);
    IntersectionsHeap<T_REAL> * iHeap = new IntersectionsHeap<T_REAL> (sinL, cosL);

    pmf_point<T_REAL> * pt;
    while (! pmfConf->empty())
    {
        pt = pmfConf->front();
        pmfConf->pop_front();
        bHeap->insert(pt);
    }


    /* ************************************************************************************** */
    // Looking for update point number id
#if pmf_LOG_ADD
    out << "[  CHG ] : oldSize = " << ptId << "   [ " << id << " ]" << std::endl;
#endif
    long updatePointCounter = 0;
    while (! bHeap->empty())
    {
#if pmf_LOG_ADD
        out << *bHeap->top() << "_" << X_ROTATED(bHeap->top()->x, bHeap->top()->y, sinL, cosL) << std::endl;
#endif
        pt = bHeap->extract_min();
        if (pt->type == PT_UPDATE)
        {
            ++updatePointCounter;
            //if (updatePointCounter == id) break;
            if (pt->id == id)  break;
        }
        pmfConf->push_back(pt);
    }
#if pmf_LOG_ADD
    out << bHeap << std::endl;
    out << "[ CHANGING POINT ] : " << *pt << std::endl << std::endl;
#endif

    // Tricky trick : duplicate update point and delete its path,
    //                then put the original one into brith heap
    pmf_point<T_REAL> * tricky = new pmf_point<T_REAL>(pt->x, pt->y, pt, NULL, 0.0, 0.0, pt->id, PT_UNKNOWN);
    pmf_point<T_REAL> * tmp;
#if CHECK_ASSERTIONS
    assert( PT_LT(pt, pt->n1, sinL, cosL) || PT_LT(pt, pt->n2, sinL, cosL) );
#endif
    if (PT_LT(pt, pt->n1, sinL, cosL))
    {
        //idik = pt->n1->id;
        tmp = pt->n1;
        pt->n1 = pt->n2;
        pt->n2 = NULL;
    }
    else //if (PT_LT(pt, pt->n2, sinL, cosL))
    {
        //idik = pt->n2->id;
        tmp = pt->n2;
        pt->n2 = NULL;
    }
    pmf_delete_rotated_path(tricky, tmp, bHeap, iHeap, NULL, ptId, fieldHeight, fieldWidth, sinL, cosL);
    delete tricky;
    bHeap->insert(pt);
#if pmf_LOG_ADD
    out << " WAS TO DELETE : " << *tmp << std::endl << std::endl;
    out << bHeap << std::endl;
#endif


    /* ************************************************************************************** */
    // and the riots start again ...
    EvolveRestOfField(bHeap, iHeap, sinL, cosL, oldSize, ptId);

    delete bHeap;
    delete iHeap;

    return;
}
#undef PT_LT
#undef X_ROTATED


#endif // UPDATE_HPP_INCLUDED
