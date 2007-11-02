#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

#include "birthslist.hpp"
#include "intersectionslist.hpp"

#include "probability.hpp"

#define REAL double
#define PMF_POINT pmf_point<REAL>


/**
 * Funkcja generuje lewe punkty narodzin, zapisuje je do listy narodzin,
 * a nastepnie zwraca ilosc tych punktow bedaca nastepnym wolny ID punktu.
 **/
long pmf_generate_initial_births (
                                    BirthsList<REAL> * list,
                                    REAL fieldHeight,
                                    REAL fieldWidth
                                )
{
    REAL polowaPI = 0.5 * M_PI;
    long id = 0;
    // Generating left birth sites ...
    cout << "[ INFO ] : Generating boundary left birth sites ... " << endl;
    REAL vertical = Exp<REAL>(2.0f);
    while (vertical <= fieldHeight) {
        REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        PMF_POINT * pt = new PMF_POINT(0.0F, vertical, 0.0F, 0.0F, ++id, PT_BIRTH_LEFT);
        list->push_back(pt);
        vertical += Exp<REAL>(2.0f);
    }
    // Generating upper birth sites ...
    cout << "[ INFO ] : Generating upper birth sites ...  " << endl;
    REAL horizontal = Exp<REAL>(2.0f);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        if (angle < 0.0f) {
            PMF_POINT * pt = new PMF_POINT(horizontal, 0.0F, 0.0F, 0.0F, ++id, PT_BIRTH_UP);
            list->push_back(pt);
        }
        horizontal += Exp<REAL>(2.0f);
    }
    // Generating lower birth sites ...
    cout << "[ INFO ] : Generating lower birth sites ...  " << endl;
    horizontal = Exp<REAL>(2.0f);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        if (angle > 0.0f) {
            PMF_POINT * pt = new PMF_POINT(horizontal, fieldWidth, 0.0F, 0.0F, ++id, PT_BIRTH_DOWN);
            list->push_in_order(pt);
        }
        horizontal += Exp<REAL>(2.0f);
    }
    return(id);
}


/*
TODO :
template <class REAL> void pmf_generate (
              REAL fieldSize, char * outputFile, time_t seed
		)
*/
void pmf_generate (
			REAL fieldHeight,
			REAL fieldWidth,
			char * outputFile,
			time_t seed
		)
{
    long id;

    cout << "[ INFO ] : starting generate function ... " << endl;
    BirthsList<REAL> * birthList = new BirthsList<REAL> ();
    IntersectionsList<REAL> * crossList = new IntersectionsList<REAL> ();

    id = pmf_generate_initial_births (birthList, fieldHeight, fieldWidth);
    cout << birthList << endl;
    cout << crossList << endl;






    cout << "[ DONE ] : leaving generate function" << endl;
    return;
}


#undef REAL
