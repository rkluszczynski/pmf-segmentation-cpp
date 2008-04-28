#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "generate.cpp"
#include "add-rot.cpp"


#define REAL double
int main (int argc, char *argv[])
{
	int           opt = 0x0;
	double   sizeArak = 5.0;
	char * outputFile = NULL;
	time_t       seed = 0;//time(NULL);

	fprintf(stderr, "[ INFO ] :  Field Size (-s) = %.2lf\n", sizeArak);
	fprintf(stderr, "[ INFO ] : Output File (-o) = '%s'\n", outputFile);
	fprintf(stderr, "[ INFO ] :        Seed (-e) = %li\n", seed);

	/* Generating Polygonal Markov Field. */
	ConfigurationList<REAL> * pmf = pmf_generate ( sizeArak, sizeArak, outputFile, seed );
	ConfigurationList<REAL> * pmf2 = new ConfigurationList<REAL>(sizeArak, sizeArak);

	fprintf(stderr, "[ INFO ] : adding point to generated configuration\n");
    pmf_add_rotated_point(pmf, pmf2, 0.2, 2.0);
//*
    cerr << "[ SAVE ] : saving modified configuration to a file" << endl;
    ofstream fout("output/PMF-rot.txt");
    pmf2->set_points_ids();
    pmf2->save_configuration(fout);
    //pmf2->save_svg(fout);
    fout.close();
//*/
	delete pmf;
	delete pmf2;
    return(0);
}
#undef REAL
