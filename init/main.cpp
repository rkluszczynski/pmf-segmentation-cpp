#include <cstdio>
#include <iostream>

#include "MosaicPMF.hpp"
#include "MosaicGraph.hpp"
#include "../cmd/grayimage.h"
#include "../init/MosaicConstants.hpp"

using namespace std;


void print_usage(char *, bool);


int main(int argc, char ** argv)
{
    double  width = MosaicConstants::GetPmfWidth();
    double height = MosaicConstants::GetPmfHeight();
    const char * image = MosaicConstants::GetImageFile();

    //pmf::GrayscaleImage gimg("input/3x3.png");
    //pmf::GrayscaleImage gimg("input/30x30.png");
    pmf::GrayscaleImage gimg(image);
    /*
    ofstream fout("output/ascii.txt");
    for(int i = 0; i < gimg.GetHeight(); ++i)
    {
        for(int k = 0; k < gimg.GetWidth(); ++k)
            fout << ((gimg[i][k][0] < 128) ? 'X' : '.');
        fout << endl;
    }
    fout.close();
    return 0;
    // */
    unsigned lines = 100;
    srand(13);
    //srand(time(NULL));
    MosaicLinesDeterminer mld;
    mld.GenerateRandomSegmentsByPolarParameters(lines, width, height);

    MosaicPMF * ipmf = new MosaicPMF(width, height, mld, gimg);

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
