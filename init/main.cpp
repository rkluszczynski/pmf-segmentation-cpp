#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <sys/timeb.h>

#include <iostream>

#include "MosaicPMF.hpp"
#include "MosaicGraph.hpp"
#include "../cmd/grayimage.h"
#include "../init/MosaicConstants.hpp"

using namespace std;


void print_usage(char *, bool = false);


int main(int argc, char ** argv)
{
	long          opt = 0x0;
	char * configFile = NULL;
	time_t       seed = 655;//15;//time(NULL);
	char   c, *endptr;

    seed = time(NULL);
	/* -------------------------------------------------------------------- */
	/*   Getting values of parameters to the program.                       */
	/* -------------------------------------------------------------------- */
	while ((c = getopt(argc, argv, "e:fc:")) != EOF)
	{
		switch (c)
		{
			case 'e': 	/* Random seed. */
				opt |= 0x001;
				seed = strtol(optarg, &endptr, 10);
				if(*endptr == '\0') break;
				else print_usage(argv[0]);
			case 'f': 	/* Force the save. */
                opt |= 0x002;
                break;
			case 'c': 	/* Path to config file. */
				opt |= 0x004;
				configFile = strdup(optarg);
				break;
			default :
				print_usage(argv[0]);
		}
	}
	if (! configFile)  configFile = strdup("input/mosaic.txt");

	/* -------------------------------------------------------------------- */
	/*   Testing values of parameters.                                      */
	/* -------------------------------------------------------------------- */
    if((opt & 0x004) && (access(configFile, R_OK) != 0))
    {
        fprintf(stderr, "[ ERROR ] : Can't read config file ('%s') !\n", configFile);
        print_usage(argv[0]);
    }
    /*
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
    // */

    MosaicConstants::SetupFile(std::string(configFile));
    cout << MosaicConstants::Instance()->GetTimeStamp() << endl;

    double  width = MosaicConstants::GetPmfWidth();
    double height = MosaicConstants::GetPmfHeight();
    const char * image = MosaicConstants::GetImageFile();

    //pmf::GrayscaleImage gimg("input/3x3.png");
    //pmf::GrayscaleImage gimg("input/30x30.png");
    pmf::GrayscaleImage gimg(image);
    //pmf::GrayscaleImage gimg("input/qq-0B.png");

    srand(seed);
    MosaicLinesDeterminer mld(true);
    int linesGenerationType = atoi(MosaicConstants::GetValueOfKey("lines-type"));
    switch (linesGenerationType)
    {
        case 0 :
                {
                    unsigned lines = atoi(MosaicConstants::GetValueOfKey("lines-amount"));
                    mld.GenerateRandomSegmentsByPolarParameters(lines, width, height);
                    break;;
                }
        case 1 :
                {
                    const char * linesFile = MosaicConstants::GetValueOfKey("lines-file");
                    mld.LoadSegmentsFromFile(linesFile);
                    break;;
                }
        default :
                {
                    fprintf(stderr, "WRONG 'lines-type' argument (only 0 or 1)\n");
                    exit(1);
                }
    }
    pmf::NumericalParameters np( MosaicConstants::GetEpsilon() );
    MosaicPMF * ipmf = new MosaicPMF(width, height, mld, gimg, np);
    //cout << ipmf << endl;
    cout << MosaicConstants::Instance()->GetTimeStamp() << endl;
    cout << MosaicConstants::Instance()->GetTimeStamp(true) << endl;

    return 0;
}


void print_usage(char * prog_name, bool cond)
{
    if (cond) {
		fprintf(stderr, "\n");
        //fprintf(stderr, "[ ERROR ] : Parameters: -s, -n or -r and -p are mandatory !\n");
    }
	fprintf(stderr, "\n[ USAGE ] :  %s  [-f]\n", prog_name);
	//fprintf(stderr, "       \t\t  [ -s size of field ]\n");
	//fprintf(stderr, "       \t\t  [ -i file with initial segments ]\n");
	//fprintf(stderr, "       \t\t  [ -o file where to save final configuration ]\n");
	fprintf(stderr, "       \t\t  [ -e random seed ]\n");
	//fprintf(stderr, "       \t\t  [ -p picture to simulate ]\n");
	fprintf(stderr, "       \t\t  [ -c config file ]\n");
	fprintf(stderr, "\n");

	exit(1);
}
