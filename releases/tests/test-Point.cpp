#include <cstdio>
#include <iostream>

#include "point.hpp"

#define REAL double

using namespace std;

int main()
{
    pmf_point<REAL> * pt = new pmf_point<REAL>(17.0, 17.0, 0, 0, 17, PT_UNKNOWN);
    cout << &(*pt) << endl;
    cout << pt << endl;
    cout << (*pt) << endl;
    return(0);
}
