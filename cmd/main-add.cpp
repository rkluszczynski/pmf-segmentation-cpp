#include "../headers/macros.hpp"


void print_usage(char *prog_name)
{
    fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -i file with initial configuration ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	fprintf(stderr, "       \t\t  [ -x coordinate X of point ]\n");
	fprintf(stderr, "       \t\t  [ -y coordinate Y of point ]\n");
	fprintf(stderr, "       \t\t  [ -d direction of adding ]\n");
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
	double        x, y;
	char    c, *endptr;


	/* -------------------------------------------------------------------- */
	/*   Getting values of parameters to the program.                       */
	/* -------------------------------------------------------------------- */
	while ((c = getopt(argc, argv, "s:i:o:e:x:y:d:f")) != EOF)
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
			case 'x': 	/* Coordinate of point. */
				opt |= 0x020;
				x = strtod(optarg, &endptr);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'y': 	/* Coordinate of point. */
				opt |= 0x040;
				y = strtod(optarg, &endptr);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'd': 	/* Angle of direction. */
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
	if(!(opt & 0x100) && (sizeArak <= 0.0))
	{
		fprintf(stderr, "[ ERROR ] : Size of the Polygonal Markov Field must be positive !\n");
		print_usage(argv[0]);
	}
    if((opt & 0x010) && (access(outputFile, F_OK) == 0))
    {
        if((opt & 0x008))
        {
            fprintf(stderr, "[ WARN ] : Forcing saving to file '%s'!\n", outputFile);
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
	if(! ((opt & 0x020) && (opt & 0x040)))
	{
		fprintf(stderr, "[ ERROR ] : New point must have coordinates X and Y!\n");
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
	if((opt & 0x101) && (opt & 0x020) && (opt & 0x040))
	{
        struct timeb tbeg, tend;

		fprintf(stderr, "[ INFO ] :   Field Size (-s) = %.2lf\n", sizeArak);
		fprintf(stderr, "[ INFO ] : Initial File (-i) = '%s'\n", initialFile);
		fprintf(stderr, "[ INFO ] :  Output File (-o) = '%s'\n", outputFile);
		fprintf(stderr, "[ INFO ] :         Seed (-e) = %li\n", seed);
		fprintf(stderr, "\n");
		fprintf(stderr, "[ INFO ] : Coordinate X (-x) = %.3lf\n", x);
		fprintf(stderr, "[ INFO ] : Coordinate Y (-y) = %.3lf\n", y);
		fprintf(stderr, "[ INFO ] :        Angle (-d) = %.3lf\n", angle);
		fprintf(stderr, "\n");

        pmf::GetTheFieldAndDoTheModification (sizeArak, sizeArak, seed, initialFile, outputFile, x, y, angle, &tbeg, &tend);

        /* Calculating modification time. */
        double modTime = tend.time - tbeg.time;
        modTime += ((tend.millitm - tbeg.millitm) * 0.001);
		fprintf(stderr, "\n[ DONE ] : modification time = %.3lf sec.\n", modTime);
	}
	else { print_usage(argv[0]); }

    return(0);
}
