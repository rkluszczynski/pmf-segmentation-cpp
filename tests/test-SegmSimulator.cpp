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

#include "sim-segm.cpp"


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
	fprintf(stderr, "       \t\t  [ -b size of blocks ]\n");
    fprintf(stderr, "       \t\t  [ -r misclassification rate ]\n");
	fprintf(stderr, "       \t\t  [ -n number of iterations ]\n");
	fprintf(stderr, "       \t\t  [ -p picture to simulate ]\n");
	fprintf(stderr, "\n");

    system("pause");
	exit(1);
}


#define REAL double
int main (int argc, char *argv[])
{
	long           opt = 0x0;
	double    sizeArak = 3.0;
	char * initialFile = "input/qq.cf";
	char *  outputFile = "output/segm-sim-test.cf";
	char * pictureFile = "input/ring-spread.bmp";
	double   blockSize = 0.0;
    long    iterations = 0;
    double     pmrStop = .05;
	time_t        seed = 1;
	char    c, *endptr;

    struct timeb tbeg, tend;
    //fprintf(stderr, "TOTAL SUCCES (%x) !!\n", opt);
    fprintf(stderr, "[ INFO ] :   Field Size (-s) = %.2lf\n", sizeArak);
    fprintf(stderr, "[ INFO ] : Initial File (-i) = '%s'\n" , initialFile);
    fprintf(stderr, "[ INFO ] :  Output File (-o) = '%s'\n" , outputFile);
    fprintf(stderr, "[ INFO ] :         Seed (-e) = %li\n"  , seed);
    fprintf(stderr, "[ INFO ] :  Blocks Size (-b) = %.2lf\n", blockSize);
    fprintf(stderr, "[ INFO ] : Picture File (-p) = '%s'\n" , pictureFile);
    fprintf(stderr, "[ INFO ] :   Iterations (-n) = %li\n"  , iterations);
    fprintf(stderr, "[ INFO ] : Stopping PMR (-r) = %.2lf\n", pmrStop);
    fprintf(stderr, "\n");

    // * Determining starting configuration. *
    PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
    pmf->SetSeed(seed);
    //pmf->LoadConfiguration(initialFile);
    pmf->Generate(blockSize);
    pmf->TestConfigurationPoints();
    pmf->SaveConfiguration("output/test-ring.txt");

#if pmf_LOG_ADD
    ofstream fout("output/tmp.txt");
    out.rdbuf(fout.rdbuf());
#endif
    // * Do the fun staff (simulation). *
    ftime(&tbeg);
    SimulateBinarySegmentation(pictureFile, pmf, iterations, pmrStop, blockSize);
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

    return(0);
}
#undef REAL
