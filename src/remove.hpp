#ifndef REMOVE_HPP_INCLUDED
#define REMOVE_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
template <class T_REAL>
void
PMF<T_REAL> :: RemoveBirthPoint (long id, T_REAL alpha = 0.0)
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

    PrepareEvolution(bHeap, alpha, sinL, cosL );

    /* ************************************************************************************** */
    /// Removing a birth site

    // looking for point to remove
#if pmf_LOG_ADD
    out << "[  REM ] : oldSize = " << ptId << "   [ " << id << " ]" << std::endl;
#endif
    long updatePointCounter = 0;
    pmf_point<T_REAL> * pt;
    while (! bHeap->empty())
    {
#if pmf_LOG_ADD
        out << *bHeap->top() << "_" << X_ROTATED(bHeap->top()->x, bHeap->top()->y, sinL, cosL) << std::endl;
#endif
        pt = bHeap->extract_min();

        if (pt->type == PT_BIRTH_NORMAL  ||  pt->type == PT_BIRTH_LEFT
            ||  pt->type == PT_BIRTH_UP  ||  pt->type == PT_BIRTH_DOWN)
        {
            ++updatePointCounter;
            //if (updatePointCounter == id) break;
            if (pt->id == id)  break;
        }
        pmfConf->push_back(pt, NULL);
    }
#if pmf_LOG_ADD
    out << bHeap << std::endl;
    out << "[ REMOVING POINT ] : " << *pt << std::endl << std::endl;
#endif

    // Tricky trick : duplicate point to remove to delete its paths
    pmf_point<T_REAL> * tricky = new pmf_point<T_REAL>(pt->x, pt->y, pt, NULL, 0.0, 0.0, pt->id, PT_UNKNOWN);
    pmf_point<T_REAL> * tmp;
#if CHECK_ASSERTIONS
    assert( PT_LT(pt, pt->n1, sinL, cosL) );
    //assert( ! PT_LT(pt->n1, pt, sinL, cosL) );
    if (pt->n2)  assert( PT_LT(pt, pt->n2, sinL, cosL) );
#endif

    if (pt->n2) {
        tmp = pt->n2;
        pt->n2 = NULL;
        pmf_delete_rotated_path(tricky, tmp, bHeap, iHeap, NULL, ptId, NULL, fieldHeight, fieldWidth, sinL, cosL);
    }
    tmp = pt->n1;
    pt->n1 = NULL;
    pmf_delete_rotated_path(tricky, tmp, bHeap, iHeap, NULL, ptId, NULL, fieldHeight, fieldWidth, sinL, cosL);

    delete tricky;
    delete pt;

#if pmf_LOG_ADD
    //out << " WAS TO DELETE : " << *tmp << std::endl << std::endl;
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

/**
 *
  los = pkt->id - 1;
  tmp = allocPoint(pkt->x, pkt->y, pkt, NULL, 0, 0, &los);

  id1 = pkt->r1->id;
  delPathS(tmp, id1, qB, qI, &idPktu, Bord);

  tmp->r1 = NULL;
  tmp->r2 = pkt;
  id2 = pkt->r2->id;
  delPathS(tmp, id2, qB, qI, &idPktu, Bord);
  delete(pkt);
  delete(tmp);

 */

#endif // REMOVE_HPP_INCLUDED
