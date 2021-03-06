#include "segmentation.h"
#include "MultiCoreSegmentation.h"

#include "NumericalParameters.h"

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

#include "PrallelDoublePRNG.h"
#include <omp.h>

void testParallelRandom_init(pmf::DoublePRNG * * dprngs)
{
    pmf::DoublePRNG * * dps = dprngs;
    printf("dps[0] = %p\n", dps[0]);
#pragma omp parallel default(none) shared(dps)
    {
        int id = omp_get_thread_num();
        pmf::DoublePRNG dp(id);
        //dps[id] = &dp;
        printf("-%i-\n", id);
        printf("-%i-> %i\n", id, int(dps[id]->GetUniform() * 10));
        printf("-%i-\n", id);
        printf("dps[0] = %p\n", dps[0]);
    }
    printf("dps[0] = %p\n", dps[0]);
}

void testParallelRandom_calculate(pmf::DoublePRNG * * dprngs)
{
    const long NUM = 100000000L;
    pmf::DoublePRNG * * dps = dprngs;
#pragma omp parallel default(none) shared(dps)
    {
        int id = omp_get_thread_num();
        printf("dps[0] = %p\n", dps[0]);

        pmf::DoublePRNG dp(id);
        //dps[id] = &dp;
        ///std::cout << "id = " << id << std::endl;
        //bool sharedPRNG = false;
        long cnt = 0;
        for (long i = 1L; i < NUM; ++i)
        {
            //double x = sharedPRNG ? prng->GetUniform(id) : dp.GetUniform();
            //double y = sharedPRNG ? prng->GetUniform(id) : dp.GetUniform();
            double x = dps[id]->GetUniform();
            double y = dps[id]->GetUniform();
            if (x * x + y * y < 1.0)  ++cnt;
        }
        double pi = double(cnt << 2) / double(NUM);
        printf("[ %2i ] : %.8lf\n", id, pi);
    }
}

void testParallelRandom()
{
    const unsigned THS = 4;

    //PrallelDoublePRNG * prng = NULL;
    //prng = new PrallelDoublePRNG(THS);
    //printf("PRNG created!\n");
    pmf::DoublePRNG * dps[THS];
    for(unsigned i = 0; i < THS; ++i) dps[i] = new pmf::DoublePRNG(i);

    omp_set_num_threads(THS);
    testParallelRandom_init(dps);
    printf("[ sync ]\n");
    testParallelRandom_calculate(dps);
    printf("[ done ]\n");
    exit(0);
}

void testRandom()
{
    const long NUM = 100000000L;
    pmf::DoublePRNG dp(0);

    long cnt = 0;
    for (long i = 0; i < NUM; i++)
    {
        double x = dp.GetUniform();
        double y = dp.GetUniform();
        if (x * x + y * y < 1.0)  ++cnt;
    }
    double pi = double(cnt << 2) / double(NUM );
    printf("%.8lf\n", pi);
    exit (0);
}


void testImageNewEnergy()
{
        pmf::GrayscaleImage img("input/square30x20.png");
        for (int r = -1; r <= img.GetHeight(); ++r)
        {
            for (int c = -1; c <= img.GetWidth(); ++c)
            {
                cout << " " << long(img.GetGreen(c,r)) << "/" << img.GetSquarePrefixSum(r, c);
            }
            cout << endl;
        }
        cout << "--" << endl;

        cout << int( img.GetGreen(0, 0) ) << " -> " << img.GetSquarePrefixSum(0, 0) << endl;
        cout << int( img.GetGreen(15, 0) ) << " -> " << img.GetSquarePrefixSum(15, 0) << endl;
        cout << int( img.GetGreen(0, 15) ) << " -> " << img.GetSquarePrefixSum(0, 15) << endl;
        cout << int( img.GetGreen(15, 15) ) << " -> " << img.GetSquarePrefixSum(15, 15) << endl;

        for (int R = 0; R < img.GetHeight(); ++R)
        {
            for (int C = 0; C < img.GetWidth(); ++C)
            {
                long val = 0L;
                for (int r = 0; r <= R; ++r)
                    for (int c = 0; c <= C; ++c)
                        val += long(img.GetGreen(c, r));

                assert(img.GetSquarePrefixSum(R, C) == val);
                //cout << '.';
            }
            cout << '*';
        }
        cout << endl;

    pmf::BinarySegmentation bs;

    exit(0);
}

int _tmp_seed, _tmp_cores;


#define REAL double
int main (int argc, char *argv[])
{
    //testImageNewEnergy();
    //testParallelRandom();
    //testRandom();

    /// tmp usage :  argv[0]  threads  seed

    _tmp_cores = 2;
    _tmp_seed = 7217;
    char * tmp_imgfile = "input/tmp/szara-wisienka-do-segm.png";
    tmp_imgfile = NULL;

    if (argc > 1  and  argc <= 4) _tmp_cores = atoi(argv[1]);
    if (argc > 2  and  argc <= 4) _tmp_seed = atoi(argv[2]);
    if (argc == 4) tmp_imgfile = argv[3];


    SegmentationParameters sparam("input/sim-core-params.txt");
    sparam.SetFieldHeight (3.0);
    sparam.SetFieldWidth (3.0);
    sparam.SetSeed (7217);
    //sparam.SetSeed (13);
    sparam.SetSeed (_tmp_seed);

    //sparam.SetInitialFile ("output/_shaked-pmf.txt");
    sparam.SetInitialFile ("output/_shaked-pmf.txt");

    sparam.SetPictureFile (tmp_imgfile);
    //sparam.SetPictureFile ("output/grzybek2.png");
    //sparam.SetPictureFile ("input/moj-grzybek-to-simulate.png");
    //sparam.SetPictureFile ("output/segm_kruki.png");
    //sparam.SetPictureFile ("input/bush-gauss-histogramcurvation.png");
    //char * pictureFile = "input/ring-spread.png";

    sparam.SetOutputDirectory ("output/");
    sparam.SetOutputFile ("output-test-file.txt");

    long iterationsNumber = 0L;
printf("QQ\n");
    if (sparam.IsParameter(std::string("pmr.iterations"))) {
	iterationsNumber = atol(sparam.GetParameter(std::string("pmr.iterations")).c_str());
printf("QQ\n");
    }
    sparam.SetIterationsNumber (iterationsNumber);
printf("QQ\n");
    //sparam.SetPMRRate (.02);
    if (sparam.IsParameter(std::string("pmr.rate"))) {
        double pmrRate = sparam.GetParameterAsDouble(std::string("pmr.rate"));
        sparam.SetPMRRate(pmrRate);
    }
    else {
        sparam.SetPMRRate (.018);
    }
    sparam.SetPMFEpsilon (1e-7);

    MultiCoreSegmentation mcs (sparam, _tmp_cores);
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
