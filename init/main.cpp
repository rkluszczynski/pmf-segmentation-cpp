#include <cstdio>
#include <iostream>

#include "MosaicPMF.hpp"

using namespace std;


int main(int argc, char ** argv)
{
    double  width = 3.0;
    double height = 3.0;

    unsigned lines = 7;

    //srand(time(NULL));

    MosaicPMF * ipmf = new MosaicPMF(width, height, lines);

    cout << ipmf << endl;

    return 0;
}
