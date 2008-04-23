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
#define PMF_POINT pmf_point<REAL>

#undef DEBUG
//#define DEBUG
#define LOG 0


/**
 * Funkcja generuje lewe punkty narodzin, zapisuje je do listy narodzin,
 * a nastepnie zwraca ilosc tych punktow bedaca nastepnym wolny ID punktu.
 *
 **/
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
        REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        PMF_POINT * pt = new PMF_POINT(0.0, vertical, 0.0, 0.0, ++id, PT_BIRTH_LEFT);
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
            PMF_POINT * pt = new PMF_POINT(horizontal, 0.0, 0.0, 0.0, ++id, PT_BIRTH_UP);
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
            PMF_POINT * pt = new PMF_POINT(horizontal, fieldWidth, 0.0, 0.0, ++id, PT_BIRTH_DOWN);
            list->push_in_order(pt, blocks);
        }
        horizontal += Exp<REAL>(2.0);
    }
    return(id);
}



void pmf_correct_intersection_point ( pmf_point<REAL> * pt, long id1, long id2 )
{
#ifdef DEBUG
    cout << "########### " << id1 << "  " << id2 << " -------------------------" << endl;
#endif
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id1)
    {
        //cout << "########### 1" << endl;
        pt->n1->n1 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  pt->n2->n2 = pt;
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id1)
    {
        //cout << "########### 2" << endl;
        pt->n1->n2 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  pt->n2->n2 = pt;
    }
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id2)
    {
        //cout << "########### 3" << endl;
        pt->n1->n1 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  pt->n2->n2 = pt;
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id2)
    {
        //cout << "########### 4" << endl;
        pt->n1->n2 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  pt->n2->n2 = pt;
    }
/*
    if (pt->n1->n1 == NULL || pt->n1->n1->id == id1)
    {
        pt->n1->n1 = pt;
        if (pt->n2->n1 == NULL || pt->n2->n1->id == id2)  pt->n2->n1 = pt;
        if (pt->n2->n2 == NULL || pt->n2->n2->id == id2)  pt->n2->n2 = pt;
    }
    if (pt->n1->n2 == NULL || pt->n1->n2->id == id1)
    {
        pt->n1->n2 = pt;
        if (pt->n2->n1 == NULL || pt->n2->n1->id == id2)  pt->n2->n1 = pt;
        if (pt->n2->n2 == NULL || pt->n2->n2->id == id2)  pt->n2->n2 = pt;
    }
    if (pt->n1->n1 == NULL || pt->n1->n1->id == id2)
    {
        pt->n1->n1 = pt;
        if (pt->n2->n1 == NULL || pt->n2->n1->id == id1)  pt->n2->n1 = pt;
        if (pt->n2->n2 == NULL || pt->n2->n2->id == id1)  pt->n2->n2 = pt;
    }
    if (pt->n1->n2 == NULL || pt->n1->n2->id == id2)
    {
        pt->n1->n2 = pt;
        if (pt->n2->n1 == NULL || pt->n2->n1->id == id1)  pt->n2->n1 = pt;
        if (pt->n2->n2 == NULL || pt->n2->n2->id == id1)  pt->n2->n2 = pt;
    }
 */
}



/*
TODO :
template <class REAL> void pmf_generate (
              REAL fieldSize, char * outputFile, time_t seed
		)
*/
ConfigurationList<REAL> *
pmf_generate (
			REAL fieldHeight,
			REAL fieldWidth,
			char * outputFile,
			time_t seed
		)
{
    long id;
    cout << "[ INFO ] : setting seed for random numbers ( " << seed << ")" << endl;
    srand(seed);

    cout << "[ INFO ] : starting generate function ... " << endl;
    BirthsList<REAL> *        birthList = new BirthsList<REAL> ();
    IntersectionsList<REAL> * crossList = new IntersectionsList<REAL> ();
    ConfigurationList<REAL> *       PMF = new ConfigurationList<REAL> (fieldWidth, fieldHeight);
    BlocksLists<REAL> *     blocksLists = new BlocksLists<REAL> (fieldWidth, fieldHeight, 1.1);
    blocksLists = NULL;

    id = pmf_generate_initial_births (birthList, fieldHeight, fieldWidth, blocksLists);
    //cout << birthList << endl;
    //cout << crossList << endl;

#if LOG
    FILE * flog = freopen("output/log.txt", "w", stdout);
#endif
    PMF_POINT * pop = NULL;
    int iterationCounter = 0;
    long id1, id2;
    while (! birthList->empty()  ||  ! crossList->empty()) {
        PMF_POINT * pt = pmf_do_top(birthList, crossList, id1, id2);

        // TODO: w-k konca petli
        if (pt->x > fieldWidth)  break;

        ++iterationCounter;
#ifdef DEBUG
        cout << " ---------------------------------------------------------------------------" << endl;
        cout << " ------------------------------ STEP " << iterationCounter << "----------------------------------" << endl;
        if (pop)  { cout << " poprzedni = " << (*pop) << "  " << endl; }
        cout << "  kandydat = " << (*pt) << "  " << endl;
        //cout << (* pmf_do_get(birthList, crossList, id1, id2)) << endl;
#endif

        /*  Nalezy sprawdzic czy nie jest punktem narodzin.
            Jesli nie jest to trzeba sprawdzic czy do tego momentu
            nie wystapil nowy punkt narodzin.
         */
        if (pt->type != PT_BIRTH_NORMAL && pt->type != PT_BIRTH_LEFT && pt->type != PT_BIRTH_DOWN && pt->type != PT_BIRTH_UP) {
            REAL zm = Exp<REAL>(fieldHeight * M_PI);
#ifdef DEBUG
            cout << "\t\t zm = " << zm << endl;
#endif
            if (pop  &&  pop->x + zm < pt->x) {
                REAL zmY = Uniform<REAL> (0.0, fieldHeight);
                PMF_POINT * newPt = new PMF_POINT(pop->x + zm, zmY, 0.0, 0.0, ++id, PT_BIRTH_NORMAL);
                birthList->push_in_order(newPt, blocksLists);
            }
        }
#ifdef DEBUG
        cout << birthList << endl;
        cout << crossList << endl;
#endif
        pt = pmf_do_get(birthList, crossList, id1, id2);
        PMF->push_back(pt);

#ifdef DEBUG
        cout << "=" << (*pt) << "       " << id1 << " " << id2 << endl;
#endif

        if (pt->type == PT_UPDATE || pt->type == PT_BIRTH_LEFT || pt->type == PT_BIRTH_DOWN || pt->type == PT_BIRTH_UP) {
            REAL angle, newAngle;
            if (pt->type == PT_UPDATE)  angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));
            else  angle = 0.0;
            determineUpdateAngle<REAL> (newAngle);

            newAngle += angle;
            if (newAngle > M_PI_2)  newAngle -= M_PI;
            if (newAngle < -M_PI_2) newAngle += M_PI;

            if (pt->type == PT_BIRTH_UP  &&  newAngle < 0)  newAngle = -newAngle;
            if (pt->type == PT_BIRTH_DOWN  &&  newAngle > 0)  newAngle = -newAngle;

            REAL len = Exp<REAL> (2.0);
            REAL xx = pt->x + len * cos(newAngle);
            REAL yy = pt->y + len * sin(newAngle);

            PMF_POINT * newPt = new PMF_POINT(xx, yy, pt, NULL, len, 0.0, ++id, PT_UPDATE);
            /* Correcting neighbour of parent point */
            if (pt->type == PT_UPDATE)  {
                pt->n2 = newPt;
                pt->l2 = len;
            }
            else  {
                pt->n1 = newPt;
                pt->l1 = len;
            }
#ifdef DEBUG
            cout << " ADDING POINT  :  " << *newPt << endl;
#endif
            // TODO :
            //		if(storePoints(nowy, qB, qI, akt, &idPktu, Bord) == 1) akt->r2 = nowy;
            //		else delete(nowy);
            pmf_store_points_in_blocks (newPt, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);
        }

        /*  W momencie wybrania juz punktu do dalszej pracy nalezy
            wykonac odpowiednie modyfikacje w listach.
         */
        if (pt->type == PT_BIRTH_NORMAL) {
            REAL upperLength = Exp<REAL>(2.0);
            REAL lowerLength = Exp<REAL>(2.0);
            REAL upperAngle, lowerAngle;
            determineBirthAngles (upperAngle, lowerAngle);

            REAL zmX, zmY;
            zmX = pt->x + upperLength * cos(upperAngle);
            zmY = pt->y + upperLength * sin(upperAngle);
            // FIX IT :
            assert(zmX >= pt->x);
            PMF_POINT * newpt1 = new PMF_POINT(zmX, zmY, pt, NULL, upperLength, 0.0, ++id, PT_UPDATE);
            pmf_store_points_in_blocks (newpt1, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);

            zmX = pt->x + lowerLength * cos(lowerAngle);
            zmY = pt->y + lowerLength * sin(lowerAngle);
            // FIX IT :
            assert(zmX >= pt->x);
            PMF_POINT * newpt2 = new PMF_POINT(zmX, zmY, pt, NULL, lowerLength, 0.0, ++id, PT_UPDATE);
            pmf_store_points_in_blocks (newpt2, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);

            pt->n1 = newpt1;  pt->l1 = upperLength;
            pt->n2 = newpt2;  pt->l2 = lowerLength;
#ifdef DEBUG
            cout << " ADDING BIRTH DIRECTION POINT 1  :  " << *newpt1 << endl;
            cout << " ADDING BIRTH DIRECTION POINT 2  :  " << *newpt2 << endl;
            cout << "        BIRTH POINT  :  " << *pt << endl;
#endif
        }

        if (pt->type == PT_INTERSECTION)
        {
            pmf_correct_intersection_point (pt, id1, id2);
#ifdef DEBUG
            cout << "INTERSECTION  : " << (*pt) << "  " << endl;
            cout << "INTERSECTION  : " << *(pt->n1) << "  " << endl;
            cout << "INTERSECTION  : " << *(pt->n2) << "  " << endl;
#endif
            crossList->remove_intersection_with_one_id_of (id1, id2, blocksLists);
            birthList->remove_point_with_id (id1, blocksLists);
            birthList->remove_point_with_id (id2, blocksLists);
        }

        pop = pt;
#ifdef DEBUG
        if (false) {
            cout << endl << "### Lista punktow w bloku ###" << endl;
            if (blocksLists)  blocksLists->print_lists();
            cout << endl << "### Atualne PMF ###" << endl;
            cout << PMF << endl;
        }
        //getchar();
#endif
    }
    delete birthList;
    delete crossList;
    delete blocksLists;

#if LOG
    cout << "[ SAVE ] : saving configuration to a file" << endl;
    ofstream fout("output/PMF.txt");
    PMF->set_points_ids();
    PMF->save_configuration(fout);
    fout.close();
//    delete PMF;
    //fclose(flog);
#endif
    cout << "[ DONE ] : leaving generate function" << endl;
    return PMF;
}
#undef LOG

#undef REAL
