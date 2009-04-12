#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <sys/timeb.h>

#define CHECK_ASSERTIONS 1

#include "grayscaleimage.hpp"
#include "PMF.hpp"


#define REAL double
int main (int argc, char *argv[])
{
	double    sizeArak = 0.0;
	char * initialFile = "input/qq.cf";
	//char * pictureFile = "input/qq-0B.bmp";
	char * pictureFile = "input/ring-spread.bmp";

    // * Determining starting configuration. *
    PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
    pmf->SetSeed(0);
    pmf->LoadConfiguration(initialFile);
    //pmf->Generate(blockSize);

#if pmf_LOG_ADD
    ofstream fout("output/tmp.txt");
    out.rdbuf(fout.rdbuf());
#endif

    GrayScaleImage gsimg(pictureFile);
    double eng = gsimg.CalculateScanLinesEnergy(pmf);

    fprintf(stderr, "[%.2lf]\n", eng);



#if pmf_LOG_ADD
    fout.close();
#endif
    return(0);
}
#undef REAL
