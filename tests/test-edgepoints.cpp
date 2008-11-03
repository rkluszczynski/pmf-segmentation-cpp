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
        fprintf(stderr, "[ ERROR ] : Parameters: -s or -l, and -u are mandatory !\n");
    }
	fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	fprintf(stderr, "       \t\t  [ -i file with initial configuration ]\n");
	fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	fprintf(stderr, "       \t\t  [ -b size of blocks ]\n");
	fprintf(stderr, "       \t\t  [ -p file with points for edges ]\n");
	fprintf(stderr, "\n");

    system("pause");
	exit(1);
}


#define REAL double
int main ()
{
	double    sizeArak = 7.0;
	char * initialFile = NULL;
	const char *  outputFile = "output/pmf1.txt";
	const char *  pointsFile = "output/edges.txt";
	double   blockSize = 0.0;
	time_t        seed = 0;

	/* -------------------------------------------------------------------- */
	/*   Check existence of required parameters for the program.            */
	/* -------------------------------------------------------------------- */
	//fprintf(stderr, "TOTAL SUCCES (%x) !!\n", opt);
	fprintf(stderr, "[ INFO ] :   Field Size (-s) = %.2lf\n", sizeArak);
	fprintf(stderr, "[ INFO ] : Initial File (-i) = '%s'\n", initialFile);
	fprintf(stderr, "[ INFO ] :  Output File (-o) = '%s'\n", outputFile);
	fprintf(stderr, "[ INFO ] :         Seed (-e) = %li\n", seed);
	fprintf(stderr, "[ INFO ] :  Blocks Size (-b) = %.2lf\n", blockSize);
	//fprintf(stderr, "\n");
	fprintf(stderr, "[ INFO ] :  Points File (-p) = '%s'\n", pointsFile);
	fprintf(stderr, "\n");

    struct timeb tbeg, tend;
    ofstream fout("output/log-seg.txt");
    out.rdbuf(fout.rdbuf());

	PMF<double> * pmf = new PMF<double>(sizeArak, sizeArak);
	pmf->SetSeed(seed);
	if (initialFile)
        pmf->LoadConfiguration(initialFile);
    else
        pmf->Generate(blockSize);

    ftime(&tbeg);
    SimulateEdgesApplier ( (char *) pointsFile, pmf, blockSize );
    ftime(&tend);

	pmf->SaveConfiguration(outputFile);
	delete pmf;
	fout.close();

    double modTime = tend.time - tbeg.time;
    modTime += ((tend.millitm - tbeg.millitm) * 0.001);
	fprintf(stderr, "\n[ DONE ] : modification time = %.3lf sec.\n", modTime);

    return(0);
}
#undef REAL
