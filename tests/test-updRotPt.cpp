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


#define REAL double
int main ()
{
	double    sizeArak = 7.0;
	double   blockSize = 0.0;
	double       angle = 0.0;
    long       pointId = 223;
	time_t        seed = 0;
    struct timeb tbeg, tend;
    double       bSize = 0.0;

	// 93(id=187), 109(id=223)
    //pointId = 187;
    bSize = 1.3;

		fprintf(stderr, "[ INFO ] :   Field Size (-s) = %.2lf\n", sizeArak);
		fprintf(stderr, "[ INFO ] :         Seed (-e) = %li\n", seed);
		fprintf(stderr, "[ INFO ] :  Blocks Size (-b) = %.2lf\n", blockSize);
		fprintf(stderr, "\n");
		fprintf(stderr, "[ INFO ] :  Id of point (-u) = %li\n", pointId);
		fprintf(stderr, "[ INFO ] :        Angle (-a) = %.3lf\n", angle);
		fprintf(stderr, "\n");

    ofstream fout("output/log-upd-rot.txt");
    out.rdbuf(fout.rdbuf());

    /* Generating Polygonal Markov Field. */
	PMF<REAL> * pmf = new PMF<REAL>(sizeArak, sizeArak);
	pmf->SetSeed(seed);
    //pmf->LoadConfiguration(initialFile);
    pmf->Generate(blockSize);

    pmf->SaveConfiguration("output/PMF-before-upd.txt");

	fprintf(stderr, "[ INFO ] : changing point direction\n");
    ftime(&tbeg);
    pmf->UpdatePointVelocity(pointId, angle, bSize);
	ftime(&tend);

    cerr << "[ SAVE ] : saving modified configuration to a file" << endl;
    pmf->SaveConfiguration("output/PMF-after-upd.txt");

	delete pmf;
    fout.close();

    double modTime = tend.time - tbeg.time;
    modTime += ((tend.millitm - tbeg.millitm) * 0.001);
    fprintf(stderr, "\n[ DONE ] : modification time = %.3lf sec.\n", modTime);

    return(0);
}
#undef REAL
