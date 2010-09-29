#include "segmentation.h"
#include "MultiCoreSegmentation.h"


void print_usage(char * prog_name, bool cond = false)
{
    if (cond) {
		fprintf(stderr, "\n");
        fprintf(stderr, "[ ERROR ] : Parameters: -s or -i, -n or -r and -p are mandatory !\n");
    }
	fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -i file with initial configuration ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
    fprintf(stderr, "       \t\t  [ -r misclassification rate ]\n");
	fprintf(stderr, "       \t\t  [ -n number of iterations ]\n");
	fprintf(stderr, "       \t\t  [ -p picture to simulate ]\n");
	fprintf(stderr, "\n");

    system("pause");
	exit(1);
}

namespace pmf
{
    namespace Geometry
    {
            bool qq = false;
    }
}


int _tmp_seed;

#define REAL double
int main (int argc, char *argv[])
{
    if (argc == 2) _tmp_seed = atoi(argv[1]);

    MultiCoreSegmentation mcs(2);
    mcs.SimulateOnMultiCore();
    return 0;



	long           opt = 0x0;
	double    sizeArak = 0.0;
	char * initialFile = NULL;
	char *  outputFile = NULL;
	char * pictureFile = NULL;
    long    iterations = 0;
    double     pmrStop = 0.0;
	time_t        seed = time(NULL);
	char    c, *endptr;


	/* -------------------------------------------------------------------- */
	/*   Getting values of parameters to the program.                       */
	/* -------------------------------------------------------------------- */
	while ((c = getopt(argc, argv, "s:e:fo:p:i:r:n:")) != EOF)
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
			case 'f': 	/* Force the save. */
                opt |= 0x008;
                break;
			case 'o': 	/* Path to output file. */
				opt |= 0x010;
				outputFile = strdup(optarg);
				break;
			case 'p':   /* Path to picture file. */
				opt |= 0x020;
				pictureFile = strdup(optarg);
				break;
			case 'r': 	/* Stopping PMR. */
				opt |= 0x040;
				pmrStop = strtod(optarg, &endptr);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'n': 	/* Number of iterations. */
				opt |= 0x080;
				iterations = strtol(optarg, &endptr, 10);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
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
    {
        if((opt & 0x008))
        {
            fprintf(stderr, "[ WARN ] : Forcing saving file '%s' !\n", outputFile);
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
    }
    if((opt & 0x020) && (access(pictureFile, R_OK) != 0))
    {
        fprintf(stderr, "[ ERROR ] : Can't read picture file ('%s') !\n", pictureFile);
        print_usage(argv[0]);
    }
	if((opt & 0x040) && (pmrStop <= 0.0 || pmrStop > 1.0))
	{
		fprintf(stderr, "[ ERROR ] : PMR must be beetween 0.0 and 1.0 !\n");
		print_usage(argv[0]);
	}
	if((opt & 0x080) && (iterations <= 0))
	{
		fprintf(stderr, "[ ERROR ] : Number of iterations should be positive !\n");
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
	if((opt & 0x101) && (opt & 0x020) && (opt & 0x0C0))
	{
        struct timeb tbeg, tend;
		//fprintf(stderr, "TOTAL SUCCES (%x) !!\n", opt);
		fprintf(stderr, "[ INFO ] :   Field Size (-s) = %.2lf\n", sizeArak);
		fprintf(stderr, "[ INFO ] : Initial File (-i) = '%s'\n" , initialFile);
		fprintf(stderr, "[ INFO ] :  Output File (-o) = '%s'\n" , outputFile);
		fprintf(stderr, "[ INFO ] :         Seed (-e) = %li\n"  , seed);
		fprintf(stderr, "[ INFO ] : Picture File (-p) = '%s'\n" , pictureFile);
		fprintf(stderr, "[ INFO ] :   Iterations (-n) = %li\n"  , iterations);
		fprintf(stderr, "[ INFO ] : Stopping PMR (-r) = %.2lf\n", pmrStop);
		fprintf(stderr, "\n");

        pmf::BinarySegmentation simulation( sizeArak, sizeArak, initialFile, outputFile, seed, pictureFile, iterations, pmrStop );
        ftime(&tbeg);
        simulation.RunTheSimulation();
	    ftime(&tend);

        // * How long was I unconscious? *
        double simTime = tend.time - tbeg.time;
        simTime += ((tend.millitm - tbeg.millitm) * 0.001);
		fprintf(stderr, "\n[ DONE ] : simulation time = %.3lf sec.\n", simTime);
	}
	else { print_usage(argv[0], true); }

    return(0);
}
