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

#include "PMF.hpp"
#include "edgepoints.hpp"


void print_usage(char *prog_name, bool cond = false)
{
    if (cond) {
		fprintf(stderr, "\n");
        fprintf(stderr, "[ ERROR ] : Parameters: -s or -l, and -u are mandatory !\n");
    }

	fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -i file with initial configuration ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	fprintf(stderr, "       \t\t  [ -b size of blocks ]\n");
	fprintf(stderr, "       \t\t  [ -p points for edges ]\n");
	//fprintf(stderr, "       \t\t  [ -a direction of adding ]\n");
	fprintf(stderr, "\n");

    system("pause");
	exit(1);
}


#define REAL double
int main ()
{
	double    sizeArak = 7.0;
	char * initialFile = NULL;
	char *  outputFile = "output/pmf1.txt";
	const char *  pointsFile = "output/edges.txt";
	double   blockSize = 0.0;
	//double       angle = 0.0;
    //long       pointId = 0;
	time_t        seed = 0;


	/* -------------------------------------------------------------------- */
	/*   Check existence of required parameters for the program.            */
	/* -------------------------------------------------------------------- */
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


        EdgePoints<double> ep;
        ep.LoadData("output/edges.txt");
        ep.PrintData();

		/*
		PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
		pmf->SetSeed(seed);
//        pmf->LoadConfiguration(initialFile);
        pmf->Generate(blockSize);

        //ofstream fout("output/log-upd-rot.txt");
        //out.rdbuf(fout.rdbuf());
        ftime(&tbeg);

        PMF<REAL> * pmf2 = pmf->Clone();
        pmf2->SaveConfiguration("output/pmf2.txt");
        delete pmf2;

	    ftime(&tend);
	    //fout.close();

		pmf->SaveConfiguration(outputFile);
		delete pmf;

        double modTime = tend.time - tbeg.time;
        modTime += ((tend.millitm - tbeg.millitm) * 0.001);
		fprintf(stderr, "\n[ DONE ] : modification time = %.3lf sec.\n", modTime);
        //*/
    return(0);
}
#undef REAL
