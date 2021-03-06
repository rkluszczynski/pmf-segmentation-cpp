#include <iostream>

#include "generate.cpp"
#define REAL double

using namespace std;

int main()
{
	REAL fieldHeight = 5.0;
	REAL fieldWidth  = 5.0;
    long id;

    cout << "[ INFO ] : starting generate function ... " << endl;
    BirthsList<REAL> * birthList = new BirthsList<REAL> ();
    IntersectionsList<REAL> * crossList = new IntersectionsList<REAL> ();

    id = pmf_generate_initial_births (birthList, fieldHeight, fieldWidth, NULL);
    birthList->remove_point_with_id(3, NULL);
    cout << birthList << endl;

    pmf_point<REAL> * pt = new pmf_point<REAL> (17,1,0,0,++id);
    pmf_point<REAL> * pt2 = new pmf_point<REAL> (1,1,0,0,++id);
    pmf_point<REAL> * pt3 = new pmf_point<REAL> (9,1,0,0,++id);
    crossList->push_back(pt,1L,2L);
    crossList->push_in_order(pt2,3L,2L);
    crossList->push_in_order(pt3,1L,7L);
    crossList->remove_intersection_with_one_id_of (5L, 1L, NULL);
    cout << crossList << endl;

    cout << "[ DONE ] : leaving generate function" << endl;

    return(0);
}
