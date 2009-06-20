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

bool saveOp = false;

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
	long             opt = 0x0;
	double      sizeArak = 3.0;
	char initialFile[64] = "input/qq.cf";
	char  outputFile[64];
	char pictureFile[64] = "input/ring-spread.bmp";
	double     blockSize = 0.0;
    long      iterations = 0;
    double       pmrStop = .05;
	time_t          seed = 7;
	char      c, *endptr;

    struct timeb tbeg, tend;
    sprintf(outputFile, "output/segm-sim-test-seed%li.cf", seed);

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

    // 6.7328954911349825e-017;0.98332639742653516
    // 1.0393420339183721;0.84079215381566141
    /*
    double xp, yp, xq, yq, xs, ys, xr, yr;
    xp = 6.7328954911349825e-017;  yp = 0.98332639742653516;  // 122
    xq = 1.0393420339183721;  yq = 0.84079215381566141;
    //xq = -0.024489601142248879; yq = 0.7875908325595895;
    xr = 0.0;  yr = 0.0;
    xs = 0.0;  ys = 3.0;
    printf("%.7lf\n", xp);

  int sgnDetPQR, sgnDetPQS, sgnDetRSP, sgnDetRSQ;

  sgnDetPQR = sgnDet(xp, yp, xq, yq, xr, yr);
  sgnDetPQS = sgnDet(xp, yp, xq, yq, xs, ys);
  sgnDetRSP = sgnDet(xr, yr, xs, ys, xp, yp);
  sgnDetRSQ = sgnDet(xr, yr, xs, ys, xq, yq);

    cout << sgnDetPQR << endl;
    cout << sgnDetPQS << endl;
    cout << sgnDetRSP << endl;
    cout << sgnDetRSQ << endl;

    cout << " # " << isOnSegment(xr, yr, xs, ys, xp, yp) << endl;

    cout << cross3(xp, yp, xq, yq, xr, yr, xs, ys) << endl;
    exit(0);
    //*/
    /*
    pmf_point<double> * q = new pmf_point<double>(1.0, 1.0, 1.0, 1.0, 1);
    cerr << pmf_point_counter << endl;
    delete q;
    cerr << pmf_point_counter << endl;
    exit(0);
    //*/

    // * Determining starting configuration. *
    PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
    pmf->SetSeed(seed);
    //pmf->LoadConfiguration(initialFile);
    pmf->Generate(blockSize);
    pmf->TestConfigurationPoints();
    pmf->SaveConfiguration("output/test-ring.txt");
    //*
    //cerr << pmf_point_counter << endl;
    //pmf->GetPMFConfiguration()->destroy();
    //cerr << *pmf->GetPMFConfiguration()->begin()->data << endl;
    delete pmf;
    //cerr << "   pmf_point_counter = " << pmf_point_counter << endl;
    //cerr << "     element_counter = " << element_counter << endl;
    //cerr << "crosselement_counter = " << crosselement_counter << endl;
    cerr << "crosselement_counter = " << CrosspointElement<double>::crosselement_counter << endl;
    exit(0);
    //*/
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
    cerr << "   pmf_point_counter = " << pmf_point<double>::pmf_point_counter << endl;
    cerr << "     element_counter = " << Element<double>::element_counter << endl;
    cerr << "crosselement_counter = " << CrosspointElement<double>::crosselement_counter << endl;

    return(0);
}
#undef REAL
