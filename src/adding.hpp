#ifndef ADDING_HPP_INCLUDED
#define ADDING_HPP_INCLUDED

#include "modifyheaps.hpp"


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
#define PT_LT(PP1,PP2,SSIN,CCOS) (X_ROTATED((PP1)->x,(PP1)->y,SSIN,CCOS) < X_ROTATED((PP2)->x,(PP2)->y,SSIN,CCOS))
#define PT_LE(PP1,PP2,SSIN,CCOS) (! PT_LT(PP2,PP1,SSIN,CCOS))
template <class T_REAL>
void
PMF<T_REAL> :: AddBirthPoint (T_REAL xx, T_REAL yy, T_REAL alpha = 0.0, T_REAL bSize = 0.0, EdgePoints<T_REAL> * ep = NULL)
{
    T_REAL fieldWidth  = pmfConf->get_field_width();
    T_REAL fieldHeight = pmfConf->get_field_height();

    long oldSize = pmfConf->get_size() + 1;
    long ptId = oldSize;

    T_REAL sinL = sin(alpha);
    T_REAL cosL = cos(alpha);

    BirthsHeap<T_REAL> *        bHeap = new BirthsHeap<T_REAL> (sinL, cosL);
    IntersectionsHeap<T_REAL> * iHeap = new IntersectionsHeap<T_REAL> (sinL, cosL);

    BlocksLists<T_REAL> * blocks = NULL;
    if (bSize > 0.0)
        blocks = new BlocksLists<T_REAL> (fieldWidth, fieldHeight, bSize);

#if pmf_LOG_ADD
    //ofstream fout("output/log-add.txt");
    //out.rdbuf(fout.rdbuf());
    ;
#endif
    PrepareEvolution(bHeap, alpha, sinL, cosL, blocks);

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
        pmf_point<T_REAL> * ppt = bHeap->extract_min();
        //pmfConf->push_back(pt, blocks);
        pmfConf->push_back(ppt);
        /*
        if (blocks) {
            pt->block = blocks->determine_point_block(pt);
            blocks->push(pt);
        }
        pmfConf->push_back(pt);
        //*/
    }

    //pmfConf->push_back(pt, blocks);
    //*
    if (blocks) {
        pt->block = blocks->determine_point_block(pt);
        blocks->push(pt);
    }
    pmfConf->push_back(pt);
    //*/
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
    /*
    newPt = pmf_put_new_neighbor<T_REAL>(pt, upperAngle, ptId, sinL, cosL);
    pt->n1 = newPt;
    pt->l1 = newPt->l1;
    //*/
    newPt = pt->pmf_put_new_neighbor(1, upperAngle, ptId, sinL, cosL);
    assert(newPt->l1 == pt->l1);
    pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, blocks, sinL, cosL);
#if pmf_LOG_ADD
    out << *pt->n1 << endl;
#endif
    /*
    newPt = pmf_put_new_neighbor<T_REAL>(pt, lowerAngle, ptId, sinL, cosL);
    pt->n2 = newPt;
    pt->l2 = newPt->l1;
    //*/
    newPt = pt->pmf_put_new_neighbor(2, lowerAngle, ptId, sinL, cosL);
    assert(newPt->l1 == pt->l2);
    pmf_store_rotated_point_in_blocks(newPt, bHeap, iHeap, pt, ptId, fieldHeight, fieldWidth, blocks, sinL, cosL);
#if pmf_LOG_ADD
    out << *pt->n2 << endl;
    out << pmfConf << std::endl;
#endif


    /* ************************************************************************************** */
    // and the riots start again ...
    EvolveRestOfField(bHeap, iHeap, sinL, cosL, oldSize, ptId, blocks, ep);

    std::cerr<<"QWEAQWE"<<std::endl;

    delete bHeap;
    delete iHeap;
    if (blocks) delete blocks;

    TestConfigurationPoints();

    return;
}
#undef PT_LE
#undef PT_LT
#undef Y_ROTATED
#undef X_ROTATED


#endif // ADDING_HPP_INCLUDED
