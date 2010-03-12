#include <cstdio>
#include <iostream>

#include "MosaicPMF.hpp"
#include "MosaicGraph.hpp"
#include "../cmd/grayimage.h"
#include "../init/AbstractProperties.hpp"

using namespace std;


void print_usage(char *, bool);


int main(int argc, char ** argv)
{
    double  width = 3.0;
    double height = 3.0;

    AbstractProperties prop("input/mosaic.txt");
    cout << prop;
    return 0;

    //pmf::GrayscaleImage gimg("input/3x3.png");
    //pmf::GrayscaleImage gimg("input/30x30.png");
    pmf::GrayscaleImage gimg("input/ring-spread.png");
    /*
    for(int i = 0; i < 30; ++i)
    {
        for(int k = 0; k < 30; ++k)
            printf("%c", (gimg[i][k][0] < 128) ? 'X' : '.');
        printf("\n");
    }
    return 0;
    // */
    unsigned lines = 53;
    ///srand(13);
    //srand(time(NULL));

    MosaicPMF * ipmf = new MosaicPMF(width, height, lines, gimg);

    cout << ipmf << endl;

    return 0;
}


void print_usage(char * prog_name, bool cond = false)
{
    if (cond) {
		fprintf(stderr, "\n");
        fprintf(stderr, "[ ERROR ] : Parameters: -s, -n or -r and -p are mandatory !\n");
    }
	fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -i file with initial segments ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	fprintf(stderr, "       \t\t  [ -p picture to simulate ]\n");
	fprintf(stderr, "\n");

    system("pause");
	exit(1);
}
