#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>

#include "generate.cpp"

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
	ConfigurationList<REAL> * pmf2 = new ConfigurationList<REAL>(sizeArak, sizeArak);
	//pmf_add_point(pmf, pmf2, 1.0, 1.0);

    return(0);
}
#undef REAL
