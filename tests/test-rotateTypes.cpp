#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "generate.cpp"
#include "rotate.cpp"

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

	/* Generating Polygonal Markov Field. */
	ConfigurationList<REAL> * pmf = pmf_generate ( sizeArak, sizeArak, outputFile, seed );
	REAL alpha = M_PI_2;
	REAL ssinL = sin(alpha);
	REAL ccosL = cos(alpha);
	pmf_rotate_point_types (pmf, ssinL, ccosL);

    cerr << "[ SAVE ] : saving rotated configuration to a file" << endl;
    ofstream fout("output/PMF-R.txt");
    pmf->set_points_ids();
    pmf->save_configuration(fout);
    fout.close();

	delete pmf;

    return(0);
}
#undef REAL
