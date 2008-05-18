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
#define pmf_LOG_ADD 1
#define DEL_PATH_LOG 1

#include "PMF.hpp"

#define REAL double
int main (int argc, char *argv[])
{
	int           opt = 0x0;
	double   sizeArak = 7.0;
	char * outputFile = NULL;
	time_t       seed = 0;//time(NULL);
    double x = 4.265;
    double y = 2.28;
    double angle = 3.511499;

	fprintf(stderr, "[ INFO ] :  Field Size (-s) = %.2lf\n", sizeArak);
	fprintf(stderr, "[ INFO ] : Output File (-o) = '%s'\n", outputFile);
	fprintf(stderr, "[ INFO ] :        Seed (-e) = %li\n", seed);
    fprintf(stderr, "\n");
	fprintf(stderr, "[ INFO ] : Coordinate X (-x) = %.3lf\n", x);
	fprintf(stderr, "[ INFO ] : Coordinate Y (-y) = %.3lf\n", y);
	fprintf(stderr, "[ INFO ] :        Angle (-a) = %.3lf\n", angle);
	fprintf(stderr, "\n");

    ofstream fout("output/log-add-rot.txt");
    out.rdbuf(fout.rdbuf());

	/* Generating Polygonal Markov Field. */
	PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
	pmf->SetSeed(seed);
    pmf->Generate();

    pmf->SaveConfiguration("output/PMF-before-rot.txt");

	fprintf(stderr, "[ INFO ] : adding point to generated configuration\n");
    pmf->AddBirthPoint(x, y, angle);

    cerr << "[ SAVE ] : saving modified configuration to a file" << endl;
    pmf->SaveConfiguration("output/PMF-after-rot.txt");

	delete pmf;
	fout.close();
    return(0);
}
#undef REAL
