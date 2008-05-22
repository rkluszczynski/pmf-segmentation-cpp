#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include <iostream>
#include <ostream>
#include <sstream>

#define CHECK_ASSERTIONS 1
#define pmf_LOG_ADD 0
#define DEL_PATH_LOG 0

#include "PMF.hpp"

#define REAL double
int main (int argc, char *argv[])
{
	int           opt = 0x0;
	double   sizeArak = 7.0;
	char * outputFile = NULL;
	time_t       seed = 0;//time(NULL);

	fprintf(stderr, "[ INFO ] :  Field Size (-s) = %.2lf\n", sizeArak);
	fprintf(stderr, "[ INFO ] : Output File (-o) = '%s'\n", outputFile);
	fprintf(stderr, "[ INFO ] :        Seed (-e) = %li\n", seed);
    fprintf(stderr, "\n");

	PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
	pmf->SetSeed(seed);
    pmf->Generate();
    pmf->SaveConfiguration("output/PMF-before-load.txt");
	fprintf(stderr, "[ SAVE ] : PMF before load");
	delete pmf;

	PMF<REAL> * pmf2 = new PMF<REAL>(sizeArak+1.0, sizeArak+1.0);
    pmf2->LoadConfiguration("output/PMF-before-load.txt");
    pmf2->SaveConfiguration("output/PMF-after-load.txt");
	fprintf(stderr, "[ SAVE ] : PMF after load");
    delete pmf2;

    return(0);
}
#undef REAL
