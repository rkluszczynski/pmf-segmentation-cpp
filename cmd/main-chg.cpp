#include "../cmd/pmf.hpp"


void print_usage(char *prog_name, bool cond = false)
{
    if (cond)
    {
		fprintf(stderr, "\n");
        fprintf(stderr, "[ ERROR ] : Parameters: -s or -i, and -p are mandatory !\n");
    }
	fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -i file with initial configuration ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	fprintf(stderr, "       \t\t  [ -p id of point to change ]\n");
	fprintf(stderr, "       \t\t  [ -a direction of adding ]\n");
	fprintf(stderr, "\n");
    //system("PAUSE");
	exit(1);
}


int main (int argc, char *argv[])
{
	long           opt = 0x0;
	double    sizeArak = 0.0;
	char * initialFile = NULL;
	char *  outputFile = NULL;
	time_t        seed = time(NULL);
	double       angle = 0.0;
    long       pointId = 0;
	char    c, *endptr;


	/* -------------------------------------------------------------------- */
	/*   Getting values of parameters to the program.                       */
	/* -------------------------------------------------------------------- */
	while ((c = getopt(argc, argv, "s:e:fo:p:a:i:")) != EOF)
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
			case 'p': 	/* Id of point to change. */
				opt |= 0x020;
				pointId = strtol(optarg, &endptr, 10);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'a': 	/* Angle of direction. */
				opt |= 0x080;
				angle = strtod(optarg, &endptr);
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
    }
	if((opt & 0x020) && (pointId <= 0))
	{
		fprintf(stderr, "[ ERROR ] : Id of point must be positive !\n");
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

		fprintf(stderr, "[ INFO ] :   Field Size (-s) = %.2lf\n", sizeArak);
		fprintf(stderr, "[ INFO ] : Initial File (-i) = '%s'\n", initialFile);
		fprintf(stderr, "[ INFO ] :  Output File (-o) = '%s'\n", outputFile);
		fprintf(stderr, "[ INFO ] :         Seed (-e) = %li\n", seed);
		fprintf(stderr, "\n");
		fprintf(stderr, "[ INFO ] :  Id of point (-p) = %li\n", pointId);
		fprintf(stderr, "[ INFO ] :        Angle (-a) = %.3lf\n", angle);
		fprintf(stderr, "\n");

		/* Getting Polygonal Markov Field realization. */
        using pmf::DoublePMF;

        DoublePMF * pmf = new DoublePMF (sizeArak, sizeArak);
        pmf->SetSeed (seed);
		if (opt & 0x100)
            pmf->LoadPMF (initialFile);
		else
            pmf->GenerateField ();

        ftime(&tbeg);
#if defined(__PMF_UPDATE_POINT)
        /* Updating an update site in PMF. */
        pmf->UpdatePointVelocity (pointId, angle);
#elif defined(__PMF_REMOVE_POINT)
        /* Removing a birth site from PMF. */
        pmf->RemoveBirthPoint (pointId, angle);
#else
    #error "Define preprocesor directive you want (__PMF_UPDATE_POINT or __PMF_REMOVE_POINT)."
#endif
	    ftime(&tend);

        if (outputFile) pmf->SavePMF (outputFile);
        delete pmf;

        /* Calculating modification time. */
        double modTime = tend.time - tbeg.time;
        modTime += ((tend.millitm - tbeg.millitm) * 0.001);
		fprintf(stderr, "\n[ DONE ] : modification time = %.3lf sec.\n", modTime);
	}
	else { print_usage(argv[0], true); }

    return(0);
}