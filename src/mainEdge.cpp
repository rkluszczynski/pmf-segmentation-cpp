#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <sys/timeb.h>

#define CHECK_ASSERTIONS 1
#define pmf_LOG_ADD 1
#define DEL_PATH_LOG 1

#include "sim-edge.cpp"


void print_usage(char *prog_name, bool cond = false)
{
    if (cond) {
		fprintf(stderr, "\n");
        fprintf(stderr, "[ ERROR ] : Parameters: -s or -l, and -p are mandatory !\n");
    }

	fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -i file with initial configuration ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	fprintf(stderr, "       \t\t  [ -b size of blocks ]\n");
	fprintf(stderr, "       \t\t  [ -p points for edges ]\n");
	fprintf(stderr, "\n");

    system("pause");
	exit(1);
}


#define REAL double
int main (int argc, char *argv[])
{
	long           opt = 0x0;
	double    sizeArak = 0.0;
	char * initialFile = NULL;
	char *  outputFile = NULL;
	char *  pointsFile = NULL;
	double   blockSize = 0.0;
	time_t        seed;
	char    c, *endptr;


	/* -------------------------------------------------------------------- */
	/*   Getting values of parameters to the program.                       */
	/* -------------------------------------------------------------------- */
	while ((c = getopt(argc, argv, "s:e:b:fo:p:i:")) != EOF)
	{
		switch (c)
		{
			case 's': 	/* Size of PMF. */
				opt |= 0x001;
				sizeArak = strtod(optarg, &endptr);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'e': 	/* Random seed. */
				opt |= 0x002;
				seed = strtol(optarg, &endptr, 10);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'b': 	/* Size of blocks. */
				opt |= 0x004;
				blockSize = strtod(optarg, &endptr);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'f': 	/* Force the save. */
                opt |= 0x008;
                break;
			case 'o': 	/* Path to output file. */
				opt |= 0x010;
				outputFile = strdup(optarg);
				break;
			case 'p':
				opt |= 0x020;
				pointsFile = strdup(optarg);
				break;
			case 'i': 	/* Path to initial file. */
				opt |= 0x100;
				initialFile = strdup(optarg);
				break;
			default :
				print_usage(argv[0]);
		}
	}


	/* -------------------------------------------------------------------- */
	/*   Testing values of parameters.                                      */
	/* -------------------------------------------------------------------- */
	if((opt & 0x001) && (sizeArak <= 0.0))
	{
		fprintf(stderr, "[ ERROR ] : Size of the Polygonal Field must be positive !\n");
		print_usage(argv[0]);
	}
    if((opt & 0x010) && (access(outputFile, F_OK) == 0))
        if((opt & 0x008))
        {
            fprintf(stderr, "[ WARN ] : Forcing saving file '%s'!\n", outputFile);
            char * backupedFile = (char *) malloc( (strlen(outputFile)+5)*sizeof(char) );
            strcpy(backupedFile, outputFile);
            strcat(backupedFile, ".old");
            unlink(backupedFile);
            rename(outputFile, backupedFile);
            fprintf(stderr, "[ WARN ] : Backuped as a file '%s' ...\n", backupedFile);
            free(backupedFile);
        }
        else {
            fprintf(stderr, "[ ERROR ] : Output file '%s' exist !\n", outputFile);
            print_usage(argv[0]);
        }
	if(! (opt & 0x002)) { seed = time(NULL); }
	if((opt & 0x004) && (blockSize < 0.0))
	{
		fprintf(stderr, "[ ERROR ] : Size of blocks must be positive !\n");
		print_usage(argv[0]);
	}
    if((opt & 0x020) && (access(pointsFile, R_OK) != 0))
    {
        fprintf(stderr, "[ ERROR ] : Can't read points for edges ('%s') !\n", pointsFile);
        print_usage(argv[0]);
    }
    if((opt & 0x100) && (access(initialFile, R_OK) != 0))
    {
        fprintf(stderr, "[ ERROR ] : Can't read configuration file '%s' !\n", initialFile);
        print_usage(argv[0]);
    }


	/* -------------------------------------------------------------------- */
	/*   Check existence of required parameters for the program.            */
	/* -------------------------------------------------------------------- */
	if((opt & 0x101) && (opt & 0x020))
	{
        struct timeb tbeg, tend;

		//fprintf(stderr, "TOTAL SUCCES (%x) !!\n", opt);
		fprintf(stderr, "[ INFO ] :   Field Size (-s) = %.2lf\n", sizeArak);
		fprintf(stderr, "[ INFO ] : Initial File (-i) = '%s'\n", initialFile);
		fprintf(stderr, "[ INFO ] :  Output File (-o) = '%s'\n", outputFile);
		fprintf(stderr, "[ INFO ] :         Seed (-e) = %li\n", seed);
		fprintf(stderr, "[ INFO ] :  Blocks Size (-b) = %.2lf\n", blockSize);
		//fprintf(stderr, "\n");
		fprintf(stderr, "[ INFO ] :  Points File (-p) = '%s'\n", pointsFile);
		fprintf(stderr, "\n");

        // * Determining starting configuration. *
		PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
		pmf->SetSeed(seed);
		if (opt & 0x100)
            pmf->LoadConfiguration(initialFile);
		else
            pmf->Generate(blockSize);
#if pmf_LOG_ADD
        ofstream fout("output/tmp.txt");
        out.rdbuf(fout.rdbuf());
#endif
        // * Do the fun staff (simulation). *
        ftime(&tbeg);
        SimulateEdgesApplier ( (char *) pointsFile, pmf, blockSize, EDGE_STRATEGY_TRY_FIRST_WRONG_FOREVER );
	    ftime(&tend);
#if pmf_LOG_ADD
	    fout.close();
#endif
        // * Saving the results (or not). *
		if (outputFile) pmf->SaveConfiguration(outputFile);
		delete pmf;

        // * How long was I unconscious? *
        double simTime = tend.time - tbeg.time;
        simTime += ((tend.millitm - tbeg.millitm) * 0.001);
		fprintf(stderr, "\n[ DONE ] : simulation time = %.3lf sec.\n", simTime);
	}
	else { print_usage(argv[0], true); }

    return(0);
}
#undef REAL
