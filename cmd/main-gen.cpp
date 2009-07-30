#include "../cmd/pmf.hpp"


void print_usage(char *prog_name)
{
    fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
    fprintf(stderr, "       \t\t  [ -s size of field ]\n");
    fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
    fprintf(stderr, "       \t\t  [ -e random seed ]\n");
    fprintf(stderr, "\n");
    //system("PAUSE");
    exit(1);
}


int main (int argc, char *argv[])
{
    int           opt = 0x0;
    double   sizeArak = 3.0;
    char * outputFile = NULL;
    time_t       seed = time(NULL);
    char   c, *endptr;

    /* -------------------------------------------------------------------- */
    /*   Getting values of parameters to the program.                       */
    /* -------------------------------------------------------------------- */
    while ((c = getopt(argc, argv, "s:o:e:f")) != EOF)
    {
        switch (c)
        {
        case 's': 	/* Size of PMF. */
            opt |= 0x01;
            sizeArak = strtod(optarg, &endptr);
            if (*endptr == '\0') break;
            else print_usage(argv[0]);
        case 'o': 	/* Path to output file. */
            opt |= 0x10;
            outputFile = strdup(optarg);
            break;
        case 'e': 	/* Random seed. */
            opt |= 0x02;
            seed = strtol(optarg, &endptr, 10);
            if (*endptr == '\0') break;
            else print_usage(argv[0]);
        case 'f': 	/* Force the save. */
            opt |= 0x08;
            break;
        default :
            print_usage(argv[0]);
        }
    }

    /* -------------------------------------------------------------------- */
    /*   Testing values of parameters.                                      */
    /* -------------------------------------------------------------------- */
    if ((opt & 0x01) && (sizeArak <= 0.0))
    {
        fprintf(stderr, "[ ERROR ] : Size of the Polygonal Field must be positive !\n");
        print_usage(argv[0]);
    }
    if ((opt & 0x10) && (access(outputFile, F_OK) == 0))
    {
        if ((opt & 0x08))
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
        else
        {
            fprintf(stderr, "[ ERROR ] : Output file '%s' exist !\n", outputFile);
            print_usage(argv[0]);
        }
    }

    /* -------------------------------------------------------------------- */
    /*   Check existence of required parameters for the program.            */
    /* -------------------------------------------------------------------- */
    if ((opt & 0x01))
    {
        struct timeb tbeg, tend;
        //fprintf(stderr, "TOTAL SUCCES (%x) !!\n", opt);
        fprintf(stderr, "[ INFO ] :  Field Size (-s) = %.2lf\n", sizeArak);
        fprintf(stderr, "[ INFO ] : Output File (-o) = '%s'\n", outputFile);
        fprintf(stderr, "[ INFO ] :        Seed (-e) = %li\n", seed);
        fprintf(stderr, "\n");

        /* -------------------------------------- */
        /*   Generating Polygonal Markov Field.   */
        /* -------------------------------------- */
        using pmf::DoublePMF;

        DoublePMF * pmf = new DoublePMF (sizeArak, sizeArak);
        pmf->SetSeed (seed);

        ftime(&tbeg);
        pmf->GenerateField ();
        ftime(&tend);

        if (outputFile) pmf->SavePMF (outputFile);
        delete pmf;

        /* Calculating modification time. */
        double genTime = tend.time - tbeg.time;
        genTime += ((tend.millitm - tbeg.millitm) * 0.001);
        fprintf(stderr, "\n[ DONE ] : generating time = %.3lf sec.\n", genTime);
    }
    else print_usage(argv[0]);

    return(0);
}
