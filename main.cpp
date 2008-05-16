#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>

#include "PMF.hpp"


void print_usage(char *prog_name)
{
	fprintf(stderr, "\n[ USAGE ] :  %s\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	fprintf(stderr, "\n");

    system("PAUSE");
	exit(1);
}


#define REAL double
int main (int argc, char *argv[])
{
	int           opt = 0x0;
	double   sizeArak = 3.0;
	char * outputFile = NULL;
	time_t     seed;
	char c, *endptr;

	/* -------------------------------------------------------------------- */
	/*   Getting values of parameters to the program.                       */
	/* -------------------------------------------------------------------- */
	while ((c = getopt(argc, argv, "s:o:e:")) != EOF)
	{
		switch (c)
		{
			case 's': 	/* Size of PMF. */
				opt |= 0x01;
				sizeArak = strtod(optarg, &endptr);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'o': 	/* Path to output file. */
				opt |= 0x10;
				outputFile = strdup(optarg);
				break;
			case 'e': 	/* Random seed. */
				opt |= 0x02;
				seed = strtol(optarg, &endptr, 10);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			default :
				print_usage(argv[0]);
		}
	}

	/* -------------------------------------------------------------------- */
	/*   Testing values of parameters.                                      */
	/* -------------------------------------------------------------------- */
	if((opt & 0x01) && (sizeArak <= 0.0))
	{
		fprintf(stderr, "[ ERROR ] : Size of the Polygonal Field must be positive !\n");
		print_usage(argv[0]);
	}
	if((opt & 0x10) && (access(outputFile, F_OK) == 0))
	{
		fprintf(stderr, "[ ERROR ] : Output file '%s' exist !\n", outputFile);
		print_usage(argv[0]);
	}
	if(! (opt & 0x02)) { seed = time(NULL); }


	/* -------------------------------------------------------------------- */
	/*   Check existence of required parameters for the program.            */
	/* -------------------------------------------------------------------- */
	if((opt & 0x01))
	{
		//fprintf(stderr, "TOTAL SUCCES (%x) !!\n", opt);
		fprintf(stderr, "[ INFO ] :  Field Size (-s) = %.2lf\n", sizeArak);
		fprintf(stderr, "[ INFO ] : Output File (-o) = '%s'\n", outputFile);
		fprintf(stderr, "[ INFO ] :        Seed (-e) = %li\n", seed);

		/* Generating Polygonal Markov Field. */
		PMF<REAL> * pmf = new PMF<double>(sizeArak, sizeArak);
		pmf->SetSeed(seed);
		pmf->Generate(1.1);
		//pmf_generate ( sizeArak, sizeArak, outputFile, seed );
	}
	else { print_usage(argv[0]); }

    return(0);
}
#undef REAL
