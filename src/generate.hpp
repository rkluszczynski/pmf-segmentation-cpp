#ifndef GENERATE_HPP_INCLUDED
#define GENERATE_HPP_INCLUDED

#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

#include "birthslist.hpp"
#include "intersectionslist.hpp"
#include "configurationlist.hpp"
#undef DEBUG
#include "blockslists.hpp"

#include "probability.hpp"
#include "accesslists.hpp"

#define REAL double


inline
long pmf_generate_initial_births (
                                    BirthsList<REAL> * list,
                                    REAL fieldHeight,
                                    REAL fieldWidth,
                                    BlocksLists<REAL> * blocks
                                )
{
    REAL polowaPI = 0.5 * M_PI;
    long id = 0;
    // Generating left birth sites ...
    cout << "[ INFO ] : Generating boundary left birth sites ... " << endl;
    REAL vertical = Exp<REAL>(2.0);
    while (vertical <= fieldHeight) {
        //REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        pmf_point<REAL> * pt = new pmf_point<REAL>(0.0, vertical, 0.0, 0.0, ++id, PT_BIRTH_LEFT);
        if (blocks) {
            pt->block = blocks->determine_point_block(pt);
            blocks->push(pt);
        }
        list->push_back(pt);
        vertical += Exp<REAL>(2.0);
    }
    // Generating upper birth sites ...
    cout << "[ INFO ] : Generating upper birth sites ...  " << endl;
    REAL horizontal = Exp<REAL>(2.0);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        if (angle < 0.0) {
            pmf_point<REAL> * pt = new pmf_point<REAL>(horizontal, 0.0, 0.0, 0.0, ++id, PT_BIRTH_UP);
            if (blocks) {
                pt->block = blocks->determine_point_block(pt);
                blocks->push(pt);
            }
            list->push_back(pt);
        }
        horizontal += Exp<REAL>(2.0);
    }
    // Generating lower birth sites ...
    cout << "[ INFO ] : Generating lower birth sites ...  " << endl;
    horizontal = Exp<REAL>(2.0);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        if (angle > 0.0) {
            pmf_point<REAL> * pt = new pmf_point<REAL>(horizontal, fieldWidth, 0.0, 0.0, ++id, PT_BIRTH_DOWN);
            list->push_in_order(pt, blocks);
        }
        horizontal += Exp<REAL>(2.0);
    }
    return(id);
}


inline
void pmf_correct_intersection_point ( pmf_point<REAL> * pt, long id1, long id2 )
{
#ifdef DEBUG
    cout << "########### " << id1 << "  " << id2 << " -------------------------" << endl;
#endif
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id1)
    {
        //cout << "########### 1" << endl;
        pt->n1->n1 = pt;
        pt->l1 = pt->n1->l1;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id1)
    {
        //cout << "########### 2" << endl;
        pt->n1->n2 = pt;
        pt->l1 = pt->n1->l2;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id2)
    {
        //cout << "########### 3" << endl;
        pt->n1->n1 = pt;
        pt->l1 = pt->n1->l1;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id2)
    {
        //cout << "########### 4" << endl;
        pt->n1->n2 = pt;
        pt->l1 = pt->n1->l2;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
}


template <class T_REAL>
inline void
PMF<T_REAL> :: Generate (T_REAL bSize = 0.0)
{
    if (pmfConf)  delete pmfConf;
    long id;
    srand(seed);

    BirthsList<T_REAL> *        birthList = new BirthsList<T_REAL> ();
    IntersectionsList<T_REAL> * crossList = new IntersectionsList<T_REAL> ();
    //ConfigurationList<T_REAL> *
    pmfConf = new ConfigurationList<T_REAL> (fieldWidth, fieldHeight);
    //BlocksLists<T_REAL> *     blocksLists = NULL;
    if (bSize > 0.0)
        blocksLists = new BlocksLists<T_REAL> (fieldWidth, fieldHeight, bSize);

    id = pmf_generate_initial_births (birthList, fieldHeight, fieldWidth, blocksLists);
    pmf_point<T_REAL> * pop = NULL;
    long id1, id2;
    while (! birthList->empty()  ||  ! crossList->empty()) {
        pmf_point<T_REAL> * pt = pmf_do_top(birthList, crossList, id1, id2);
        // TODO: w-k konca petli
        if (pt->x > fieldWidth)  break;

        /*  Nalezy sprawdzic czy nie jest punktem narodzin.
            Jesli nie jest to trzeba sprawdzic czy do tego momentu
            nie wystapil nowy punkt narodzin.
         */
        if (pt->type != PT_BIRTH_NORMAL && pt->type != PT_BIRTH_LEFT && pt->type != PT_BIRTH_DOWN && pt->type != PT_BIRTH_UP) {
            T_REAL zm = Exp<T_REAL>(fieldHeight * M_PI);
            if (pop  &&  pop->x + zm < pt->x) {
                T_REAL zmY = Uniform<T_REAL> (0.0, fieldHeight);
                pmf_point<T_REAL> * newPt = new pmf_point<T_REAL>(pop->x + zm, zmY, 0.0, 0.0, ++id, PT_BIRTH_NORMAL);
                birthList->push_in_order(newPt, blocksLists);
            }
        }
        pt = pmf_do_get(birthList, crossList, id1, id2);
        pmfConf->push_back(pt);

        if (pt->type == PT_UPDATE || pt->type == PT_BIRTH_LEFT || pt->type == PT_BIRTH_DOWN || pt->type == PT_BIRTH_UP) {
            T_REAL angle, newAngle;
            if (pt->type == PT_UPDATE)  angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));
            else  angle = 0.0;
            determineUpdateAngle<T_REAL> (newAngle);

            newAngle += angle;
            if (newAngle > M_PI_2)  newAngle -= M_PI;
            if (newAngle < -M_PI_2) newAngle += M_PI;

            if (pt->type == PT_BIRTH_UP  &&  newAngle < 0)  newAngle = -newAngle;
            if (pt->type == PT_BIRTH_DOWN  &&  newAngle > 0)  newAngle = -newAngle;

            T_REAL len = Exp<T_REAL> (2.0);
            T_REAL xx = pt->x + len * cos(newAngle);
            T_REAL yy = pt->y + len * sin(newAngle);

            pmf_point<T_REAL> * newPt = new pmf_point<T_REAL>(xx, yy, pt, NULL, len, 0.0, ++id, PT_UPDATE);
            /* Correcting neighbour of parent point */
            if (pt->type == PT_UPDATE)  {
                pt->n2 = newPt;
                pt->l2 = len;
            }
            else  {
                pt->n1 = newPt;
                pt->l1 = len;
            }
            pmf_store_points_in_blocks (newPt, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);
        }
        if (pt->type == PT_BIRTH_NORMAL) {
            T_REAL upperLength = Exp<T_REAL>(2.0);
            T_REAL lowerLength = Exp<T_REAL>(2.0);
            T_REAL upperAngle, lowerAngle;
            determineBirthAngles (upperAngle, lowerAngle);

            T_REAL zmX, zmY;
            zmX = pt->x + upperLength * cos(upperAngle);
            zmY = pt->y + upperLength * sin(upperAngle);
#if CHECK_ASSERTIONS
            // FIX IT :
            assert(zmX >= pt->x);
#endif
            pmf_point<T_REAL> * newpt1 = new pmf_point<T_REAL>(zmX, zmY, pt, NULL, upperLength, 0.0, ++id, PT_UPDATE);
            pmf_store_points_in_blocks (newpt1, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);

            zmX = pt->x + lowerLength * cos(lowerAngle);
            zmY = pt->y + lowerLength * sin(lowerAngle);
#if CHECK_ASSERTIONS
            // FIX IT :
            assert(zmX >= pt->x);
#endif
            pmf_point<T_REAL> * newpt2 = new pmf_point<T_REAL>(zmX, zmY, pt, NULL, lowerLength, 0.0, ++id, PT_UPDATE);
            pmf_store_points_in_blocks (newpt2, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);

            pt->n1 = newpt1;  pt->l1 = upperLength;
            pt->n2 = newpt2;  pt->l2 = lowerLength;
        }
        if (pt->type == PT_INTERSECTION)
        {
            pmf_correct_intersection_point (pt, id1, id2);
            crossList->remove_intersection_with_one_id_of (id1, id2, blocksLists);
            birthList->remove_point_with_id (id1, blocksLists);
            birthList->remove_point_with_id (id2, blocksLists);
        }
        pop = pt;
    }
    delete birthList;
    delete crossList;
    //if (blocksLists) delete blocksLists;

    pmfConf->set_points_ids();

    TestConfigurationPoints();
}


#endif // GENERATE_HPP_INCLUDED
