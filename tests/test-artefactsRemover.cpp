#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include <iostream>
#include <ostream>
#include <sstream>

#define CHECK_ASSERTIONS 1
#define pmf_LOG_ADD 0
#define DEL_PATH_LOG 0

#include "PMF.hpp"

#define REAL double
int main (int argc, char *argv[])
{
	PMF<REAL> * pmf = new PMF<REAL>(0.0, 0.0);
	pmf->SetSeed(0);
    pmf->LoadConfiguration("input/artefacts.cf");

    pmf->BorderArtefactsRemover();
    std::cout << " Is there intersection? : " << (pmf->IsThereIntersection() ? "Yes" : "No") << std::endl;

    pmf->SaveConfiguration("output/artefacts-none.txt");
	delete pmf;

    return(0);
}
#undef REAL
